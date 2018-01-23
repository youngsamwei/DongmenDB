//
// Created by Sam on 2018/1/19.
//

#include <malloc.h>
#include <expression.h>
#include <stdio.h>
#include <mem.h>
#include "parser.h"
#include "parseExpression.h"
#include "parseExpressionRPN.h"

/*使用逆波兰(RPN)法解析表达式:优点是能快速解析正确的表达式，但对错误的表达式尚不能很好处理。*/
Expression *parseExpressionRPN(ParserT *parser) {
    typedef enum {
        EET_NONE,
        EET_OPERATOR,
        EET_OPERAND
    } ExprElementType;
    op_stack *opstack = (op_stack *) malloc(sizeof(op_stack));
    /*以TOKEN_NULL做为栈底*/
    opstack->operatorType = TOKEN_NULL;
    opstack->next = NULL;
    Expression *rootexpr = NULL;
    ExprElementType eetype = EET_NONE;

    while ( parser->parserStateType != PARSER_WRONG) {
        TokenT *token = parseNextToken(parser);

        if (token == NULL) {
            break;
        }
        switch (token->type) {
            case TOKEN_OPEN_PAREN:
                opstack = stackPush(opstack, token->type);
                eetype = EET_NONE;
                break;
            case TOKEN_CLOSE_PAREN: {
                eetype = EET_NONE;
                /*退栈，直到遇到TOKEN_OPEN_PAREN*/
                TokenType type = opstack->operatorType;
                opstack = stackPop(opstack);
                while (type != TOKEN_OPEN_PAREN) {
                    Expression *expr = newExpression(type, NULL);
                    if (rootexpr != NULL) {
                        expr->nextexpr = rootexpr;
                    }
                    rootexpr = expr;
                    if (opstack->operatorType == TOKEN_NULL) {
                        sprintf(parser->parserMessage, "error: pos:%i, token: %s \n", parser->tokenizer->offset, token->text);
                        parser->parserStateType = PARSER_WRONG;
                        break;
                    }
                    type = opstack->operatorType;
                    opstack = stackPop(opstack);

                }
                break;
            }
            case TOKEN_WORD:
            case TOKEN_STRING:
            case TOKEN_CHAR:
            case TOKEN_EXP_FLOAT:
            case TOKEN_FLOAT:
            case TOKEN_OCTAL:
            case TOKEN_HEX:
            case TOKEN_DECIMAL:
            case TOKEN_ZERO:
            case TOKEN_NULL: {
                /*简单的语法检测，操作数不能相邻*/
                if (eetype == EET_OPERAND) {
                    sprintf(parser->parserMessage, "error: pos:%i, token: %s \n", parser->tokenizer->offset, token->text);
                    parser->parserStateType = PARSER_WRONG;
                    break;
                } else {
                    eetype = EET_OPERAND;
                }
                /*终结符*/
                Expression *expr = newExpression(token->type, NULL);
                TermExpr *term = newTermExpr();
                switch (token->type) {
                    case TOKEN_WORD:
                        term->t = TERM_ID;
                        term->id = token->text;
                        break;
                    case TOKEN_STRING:
                    case TOKEN_CHAR: {

                        term->t = TERM_LITERAL;
                        Literal *literal
                                = newLiteral(TYPE_TEXT);
                        literal->val.strval = token->text;
                        term->val = literal;
                    }
                        break;
                    case TOKEN_NULL:
                        term->t = TERM_NULL;
                        break;
                    case TOKEN_EXP_FLOAT:
                    case TOKEN_FLOAT:
                    case TOKEN_DECIMAL: {
                        term->t = TERM_LITERAL;
                        Literal *literal
                                = newLiteral(TYPE_DOUBLE);
                        literal->val.dval = atof(token->text);
                        literal->val.strval = token->text;
                        term->val = literal;
                    }
                        break;
                    case TOKEN_OCTAL:
                    case TOKEN_HEX:
                    case TOKEN_ZERO: {
                        term->t = TERM_LITERAL;
                        Literal *literal
                                = newLiteral(TYPE_INT);
                        term->val = literal;
                        literal->val.dval = atoi(token->text);
                    }
                        break;
                    default:
                        sprintf(parser->parserMessage, "error: pos:%i, token: %s \n", parser->tokenizer->offset, token->text);
                        parser->parserStateType = PARSER_WRONG;
                        /*出错*/;
                }
                expr->term = term;
                if (rootexpr != NULL) {
                    expr->nextexpr = rootexpr;
                }
                rootexpr = expr;
                break;
            }
            case TOKEN_INVALID:
            case TOKEN_UNENDED_SRING:
            case TOKEN_INCOMPLETE_CHAR:
            case TOKEN_INVALID_CHAR:
                /*出错*/
                sprintf(parser->parserMessage, "invalid token or string: pos:%i, token: %s \n", parser->tokenizer->offset, token->text);
                parser->parserStateType = PARSER_WRONG;
                break;
            case TOKEN_SEMICOLON:
            case TOKEN_RESERVED_WORD:
                /*需要判断一下操作符栈里是否还有操作，如果有则出错，若没有则正常返回。*/
                sprintf(parser->parserMessage, "PARSER_RESERVED_WORD: pos:%i, token: %s \n", parser->tokenizer->offset, token->text);
                parser->parserStateType = PARSER_RESERVED_WORD;
                break;
            case TOKEN_NOT:
            case TOKEN_AND:
            case TOKEN_OR:
            case TOKEN_PLUS:
            case TOKEN_MINUS:
            case TOKEN_MULTIPLY:
            case TOKEN_DIVIDE:
            case TOKEN_GT:
            case TOKEN_LT:
            case TOKEN_LE:
            case TOKEN_GE:
            case TOKEN_EQ:
            case TOKEN_NOT_EQUAL:
            case TOKEN_COMMA:
            case TOKEN_ASSIGNMENT:
            case TOKEN_LIKE:
            case TOKEN_IN:
            case TOKEN_FUN:
                /*简单的语法检测，操作数不能相邻,但fun特殊*/
                if (eetype == EET_OPERATOR) {
                    if (token->type == TOKEN_FUN) {
                        eetype = EET_OPERAND;
                    } else {
                        sprintf(parser->parserMessage, "error: pos:%i, token: %s \n", parser->tokenizer->offset, token->text);
                        parser->parserStateType = PARSER_WRONG;
                        break;
                    }
                } else {
                    eetype = EET_OPERATOR;
                }
                /*操作符：与栈里的操作符比较优先级，
                * 如果大于栈里的操作符优先级，则进栈，
                * 若小于等于，则出栈直到栈里的操作符优先级大于当前操作符，
                * */;
                OPERATOR currop = operators[token->type];
                if ((opstack->operatorType == TOKEN_NULL)) {
                    /*栈中没有操作符或者栈顶是左括号,则直接进栈*/
                    opstack = stackPush(opstack, token->type);
                } else {
                    OPERATOR stackop = operators[opstack->operatorType];

                    while ((currop.icp <= stackop.icp) && (opstack->operatorType != TOKEN_OPEN_PAREN)) {
                        Expression *expr = newExpression(opstack->operatorType, NULL);
                        if (rootexpr != NULL) {
                            expr->nextexpr = rootexpr;
                        }
                        rootexpr = expr;
                        opstack = stackPop(opstack);
                        /*如果是栈底*/
                        if (opstack->operatorType == TOKEN_NULL) {
                            break;
                        } else {
                            stackop = operators[opstack->operatorType];
                        }
                    }
                    opstack = stackPush(opstack, token->type);
                }
                break;
            default:
                /*出错*/
                sprintf(parser->parserMessage, "error: pos:%i, token: %s \n", parser->tokenizer->offset, token->text);
                parser->parserStateType = PARSER_WRONG;

        }

    }

    TokenType type = opstack->operatorType;
    while (type != TOKEN_NULL) {
        if (type == TOKEN_OPEN_PAREN) {
            strcpy(parser->parserMessage , "error: missing right parenthesis.");
            parser->parserStateType = PARSER_WRONG;
            break;
        }
        opstack = stackPop(opstack);
        Expression *expr = newExpression(type, NULL);
        if (rootexpr != NULL) {
            expr->
                    nextexpr = rootexpr;
        }
        rootexpr = expr;
        type = opstack->operatorType;

    }
    return
            rootexpr;
};

