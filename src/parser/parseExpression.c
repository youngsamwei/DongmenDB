//
// Created by Sam on 2018/1/19.
//

#include <malloc.h>
#include "parseExpression.h"

/*使用逆波兰法解析表达式*/
Expression *parseExpression(TokenizerT *tk) {
    op_stack *opstack = (op_stack *) malloc(sizeof(op_stack));
    Expression *rootexpr = (Expression *) malloc(sizeof(Expression));
    int go = 1;
    while (go) {
        TokenT *token = TKGetNextToken(tk);
        Expression *expr;

        switch (token->type) {
            case TOKEN_OPEN_PAREN:
                stackPush(opstack, token->type);
                break;
            case TOKEN_CLOSE_PAREN: {
                /*退栈，直到遇到TOKEN_OPEN_PAREN*/
                op_stack *stack = stackPop(opstack);
                while (stack != NULL) {
                    if (stack->operatorType == TOKEN_OPEN_PAREN) {
                        /* 需要处理func*/
                    } else {
                        expr = (Expression *) malloc(sizeof(Expression));
                        expr->opType = stack->operatorType;
                        expr->nextexpr = rootexpr;
                        rootexpr = expr;
                    }
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
            case TOKEN_NULL:
                /*终结符*/
                expr->opType = oper_term;
                TermExpr *term = (TermExpr *) malloc((sizeof(TermExpr)));
                switch (token->type) {
                    case TOKEN_WORD:
                        term->t = TERM_ID;
                        term->id = token->text;
                        break;
                    case TOKEN_STRING:
                        term->t = TERM_LITERAL;
                        term->val->t = TYPE_TEXT;
                        term->val->val.strval = token->text;
                        break;
                    case TOKEN_CHAR:
                        term->t = TERM_LITERAL;
                        term->val->t = TYPE_CHAR;
                        term->val->val.cval = token->text;
                        break;
                    case TOKEN_NULL:
                        term->t = TERM_NULL;
                        break;
                    case TOKEN_EXP_FLOAT:
                    case TOKEN_FLOAT:
                    case TOKEN_DECIMAL:
                        term->t = TERM_LITERAL;
                        term->val->val.dval = strtof(token->text, "\0");
                        break;
                    case TOKEN_OCTAL:
                    case TOKEN_HEX:

                    case TOKEN_ZERO:
                        term->t = TERM_LITERAL;
                        term->val->val.ival = strtol(token->text, NULL, 10);
                        break;
                    default:
                        go = 0;
                        /*出错*/;
                }
                expr->term = term;
                expr->nextexpr = rootexpr;
                rootexpr = expr;
                break;
            case TOKEN_INVALID:
            case TOKEN_UNENDED_SRING:
            case TOKEN_INCOMPLETE_CHAR:
            case TOKEN_INVALID_CHAR:
                /*出错*/
                go = 0;
                break;
            case TOKEN_SEMICOLON:
            case TOKEN_RESERVED_WORD:
                /*需要判断一下操作符栈里是否还有操作，如果有则出错，若没有则正常返回。*/
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
                /*操作符：与栈里的操作符比较优先级，
                * 如果大于栈里的操作符优先级，则进栈，
                * 若小于等于，则出栈直到栈里的操作符优先级大于当前操作符，
                * */;
                op_stack *stack = stackPop(opstack);
                while (stack != NULL) {
                    if (stack->operatorType == TOKEN_OPEN_PAREN) {
                        /* 需要处理func*/
                    } else {
                        expr = (Expression *) malloc(sizeof(Expression));
                        expr->opType = stack->operatorType;
                        expr->nextexpr = rootexpr;
                        rootexpr = expr;
                    }
                }
                break;
            default:
                /*出错*/
                go = 0;

        }

    }
};

op_stack *stackPush(op_stack *opstack, operator_type opType) {
    op_stack *stack = (op_stack *) malloc(sizeof(op_stack));
    stack->operatorType = opType;
    stack->next = opstack->next;
    opstack->next = stack;
    return opstack;
}

operator_type stackPop(op_stack *opstack) {
    operator_type type = opstack->operatorType;
    op_stack *current = opstack;
    opstack = opstack->next;
    current->next = NULL;
    free(current);
}