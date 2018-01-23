//
// Created by Sam on 2018/1/22.
//

#include <malloc.h>
#include <mem.h>
#include <stdio.h>
#include "parseExpressionRD.h"
#include "parseExpression.h"

Expression *parseExpressionRD(ParserT *parser) {
    Expression *expr = parseReadBooleanOr(parser);
    if (parser->parserStateType != PARSER_WRONG && parser->currToken != NULL
        && parser->currToken->type != TOKEN_RESERVED_WORD) {
        /*表达式没有处理完，比如只有右括号的情况。*/
        char message[PARSER_MESSAGE_LENTTH];
        sprintf(message, "syntax error: %s.", parser->currToken->text);
        return parseError(parser, message);
    }

    return expr;
};

/*由parseReadDouble修改而来*/
Expression *parseReadLiteral(ParserT *parser) {
    Expression *expr0 = NULL;
    TokenT *token = parseNextToken(parser);
    if (token == NULL) {
        return parseError(parser, "syntax error: missing number.");
    }
    if ((token->type == TOKEN_FLOAT || token->type == TOKEN_DECIMAL
         || token->type == TOKEN_EXP_FLOAT)) {
        /*处理double数据*/
        expr0 = newExpression(token->type, NULL);
        TermExpr *term = newTermExpr();
        term->t = TERM_LITERAL;
        Literal *literal
                = newLiteral(TYPE_DOUBLE);
        literal->val.dval = atof(token->text);
        literal->val.strval = token->text;
        term->val = literal;
        expr0->term = term;
        parseEatToken(parser);
        return expr0;
    } else if (token->type == TOKEN_STRING || token->type == TOKEN_CHAR) {
        /*处理string和char,都转换为string*/
        expr0 = newExpression(TOKEN_STRING, NULL);
        TermExpr *term = newTermExpr();
        term->t = TERM_LITERAL;
        Literal *literal
                = newLiteral(TYPE_TEXT);
        literal->val.strval = token->text;
        term->val = literal;
        expr0->term = term;
        parseEatToken(parser);
        return expr0;
    } else {
        char message[PARSER_MESSAGE_LENTTH];
        sprintf(message, "syntax error: unenabled data type :%s.", token->text);
        return parseError(parser, message);
    }
};

/**
 @brief reads arguments for the builtin functions, auxilliary function for
 parseRead_builtin()
 @param[in] parser input ParserT structure to operate upon
 @return expr of the argument that was read
 */
Expression *parseReadArgument(ParserT *parser) {
    Expression *expr0 = NULL;
    TokenT *token = parseNextToken(parser);
    if (token == NULL) {
        return parseError(parser, "syntax error: missing argument.");
    }
    if (token->type == TOKEN_COMMA) {
        parseEatToken(parser);
    }
    expr0 = parseReadExpr(parser);
    return expr0;
};

/**
 @brief reads and calls built-in functions, like sqrt(.), pow(.), etc.
 @param[in] parser input ParserT structure to operate upon
 @return resulting expr
*/
Expression *parseReadBuiltin(ParserT *parser) {
    Expression *expr0 = NULL, *expr1 = NULL;
    TokenT *token = parseNextToken(parser);
    if (token == NULL) {
        return parseError(parser, "syntax error: missing id or number.");
    }
    if (token->type == TOKEN_WORD) {
        char *text = token->text;
        token = parseEatAndNextToken(parser);

        if (token != NULL && token->type == TOKEN_OPEN_PAREN) {
            if (stricmp(text, "ltrim") == 0) {
                /*函数参数个数的检测*/
                token = parseEatAndNextToken(parser);
                /*如果紧跟着右括号，则丢失参数*/
                if (token->type == TOKEN_CLOSE_PAREN){
                    char message[PARSER_MESSAGE_LENTTH];
                    sprintf(message, "syntax error: function %s missing argument.", text);
                    return parseError(parser,message);
                }
                Expression *param0 = parseReadArgument(parser);
                token = parseNextToken(parser);
                /*如果解析参数完毕，接下来是逗号，则太多参数*/
                if (token->type == TOKEN_COMMA){
                    char message[PARSER_MESSAGE_LENTTH];
                    sprintf(message, "syntax error: function %s too many  argument.", text);
                    return parseError(parser,message);
                }
                expr0 = newExpression(TOKEN_FUN, param0);
            } else if (stricmp(text, "round") == 0) {
                /*函数参数个数的检测*/
                token = parseEatAndNextToken(parser);
                if (token->type == TOKEN_CLOSE_PAREN){
                    char message[PARSER_MESSAGE_LENTTH];
                    sprintf(message, "syntax error: function %s missing  argument.", text);
                    return parseError(parser,message);
                }
                Expression *param0 = parseReadArgument(parser);
                token = parseNextToken(parser);
                if (token->type != TOKEN_COMMA){
                    char message[PARSER_MESSAGE_LENTTH];
                    sprintf(message, "syntax error: function %s missing  argument.", text);
                    return parseError(parser,message);
                }
                Expression *param1 = parseReadArgument(parser);
                token = parseNextToken(parser);
                if (token->type == TOKEN_COMMA){
                    char message[PARSER_MESSAGE_LENTTH];
                    sprintf(message, "syntax error: function %s too many  argument.", text);
                    return parseError(parser,message);
                }
                expr0 = concatExpression(param0, param1);
                expr0 = newExpression(TOKEN_FUN, expr0);
            } else {
                char message[PARSER_MESSAGE_LENTTH];
                sprintf(message, "syntax error: unsupported function  :%s.", text);
                return parseError(parser, message);
            }
            token = parseNextToken(parser);

            if (token == NULL || token->type != TOKEN_CLOSE_PAREN) {
                return parseError(parser, "syntax error: syntax error: missing ')'.");
            } else {
                /*处理掉右括号*/
                parseEatToken(parser);
            }
            return expr0;
        } else {
            /*此处是标识符处理*/
            expr0 = newExpression(TOKEN_WORD, NULL);
            TermExpr *term = newTermExpr();
            term->val = NULL;
            term->t = TERM_ID;
            term->id = text;
            expr0->term = term;
        }
    } else {
        expr0 = parseReadLiteral(parser);
    }
    return expr0;
};

/**
 @brief attempts to read an expression in parentheses, or failing that a literal value
 @param[in] parser input ParserT structure to operate upon
 @return expression/literal
 */
Expression *parseReadParen(ParserT *parser) {
    Expression *expr0 = NULL;
    TokenT *token = parseNextToken(parser);
    if (token == NULL) {
        return parseError(parser, "syntax error.");
    }
    if (token->type == TOKEN_OPEN_PAREN) {
        parseEatAndNextToken(parser);
        expr0 = parseReadBooleanOr(parser);
        token = parseNextToken(parser);
        if (token == NULL || (token != NULL && token->type != TOKEN_CLOSE_PAREN)) {
            return parseError(parser, "syntax error: missing ')'.");
        } else {
            parseEatToken(parser);
        }
    } else {
        expr0 = parseReadBuiltin(parser);
    }
    return expr0;
};

/**
 @brief attempts to read a unary operation, or failing that, a parenthetical or literal value
 @param[in] parser input ParserT structure to operate upon
 @return expression/literal
*/
Expression *parseReadUnary(ParserT *parser) {
    Expression *expr0 = NULL, *expr1 = NULL;
    TokenT *token = parseNextToken(parser);
    if (token == NULL) {
        return parseError(parser, "syntax error.");
    }
    if (token != NULL && (token->type == TOKEN_NOT || token->type == TOKEN_MINUS
                          || token->type == TOKEN_PLUS)) {
        parseEatAndNextToken(parser);
        expr1 = parseReadParen(parser);
        expr0 = newExpression(token->type, expr1);
    } else {
        expr0 = parseReadParen(parser);
    }
    return expr0;
};

/**
 @brief attempts to read an exponentiation operator, or failing that, a parenthetical expression
 @param[in] parser input ParserT structure to operate upon
 @return exponentiation
 */
Expression *parseReadPower(ParserT *parser) {
    Expression *expr0 = NULL, *expr1 = NULL;
    TokenT *token = parseNextToken(parser);
    expr0 = parseReadUnary(parser);
    token = parseNextToken(parser);

    if (token != NULL && token->type == TOKEN_POWER) {
        /*需要特殊处理*/
        token = parseEatAndNextToken(parser);
        expr1 = parseReadPower(parser);
    }
    /*整合为一个表达式*/
    return expr0;
};

/**
 @brief reads a term in an expression
 @param[in] parser input ParserT structure to operate on
 @return expr of the term
 */
Expression *parseReadTerm(ParserT *parser) {
    Expression *expr0 = NULL, *expr1 = NULL, *expr2 = NULL;
    expr0 = parseReadPower(parser);
    TokenT *token = parseNextToken(parser);

    while (token != NULL && (token->type == TOKEN_MULTIPLY || token->type == TOKEN_DIVIDE)) {
        /*需要处理多个连续的乘除*/
        parseEatToken(parser);
        /*获得乘法的右表达式*/
        expr1 = parseReadPower(parser);
        /*左右表达式合并为一个后缀表达式*/
        expr0 = concatExpression(expr0, expr1);
        /*在后缀表达式前增加乘法操作*/
        expr2 = newExpression(token->type, expr0);
        expr0 = expr2;
        token = parseNextToken(parser);
    }

    return expr0;
};

/*合并两个后缀表达式*/
Expression *concatExpression(Expression *expr0, Expression *expr1) {
    if (expr0 == NULL)
        return expr1;
    Expression *expr2 = expr0;
    while (expr2->nextexpr != NULL) {
        expr2 = expr2->nextexpr;
    }
    expr2->nextexpr = expr1;
    return expr0;
}

/**
 @brief attempts to read an expression
 @param[in] parser input ParserT structure
 @return expression
 */
Expression *parseReadExpr(ParserT *parser) {
    Expression *expr0 = NULL, *expr1 = NULL, *expr2 = NULL;
    TokenT *token = parseNextToken(parser);
    if (token == NULL) {
        return parseError(parser, "syntax error.");
    }
    if (token->type == TOKEN_PLUS || token->type == TOKEN_MINUS) {
        /*处理正负号*/
        parseEatToken(parser);
        expr0 = parseReadTerm(parser);
        expr0 = newExpression(token->type, expr0);
    } else {
        expr0 = parseReadTerm(parser);
    }
    /*需要递归处理，连续的+-操作*/
    token = parseNextToken(parser);

    while (token != NULL && (token->type == TOKEN_PLUS || token->type == TOKEN_MINUS)) {
        parseEatToken(parser);
        expr1 = parseReadTerm(parser);
        expr0 = concatExpression(expr0, expr1);
        expr2 = newExpression(token->type, expr0);
        expr0 = expr2;
        token = parseNextToken(parser);
    }
    return expr0;
};

/**
 @brief reads and performs a boolean comparison operations (<,>,<=,>=,==) if found
 @param[in] parser input ParserT structure
 @return sub-expression
 */
Expression *parseReadBooleanComparison(ParserT *parser) {
    Expression *expr0 = NULL, *expr1 = NULL, *expr2 = NULL;
    expr0 = parseReadExpr(parser);
    TokenT *token = parseNextToken(parser);

    if (token != NULL && (token->type == TOKEN_GE || token->type == TOKEN_LE
                          || token->type == TOKEN_GT || token->type == TOKEN_LT)) {
        parseEatAndNextToken(parser);
        expr1 = parseReadExpr(parser);
        expr0 = concatExpression(expr0, expr1);
        expr0 = newExpression(token->type, expr0);
    }
    return expr0;
};

Expression *parseReadBooleanEquality(ParserT *parser) {
    Expression *expr0 = NULL, *expr1 = NULL;
    TokenT *token = parseNextToken(parser);
    expr0 = parseReadBooleanComparison(parser);
    token = parseNextToken(parser);

    if (token != NULL && (token->type == TOKEN_NOT_EQUAL || token->type == TOKEN_EQ)) {
        parseEatAndNextToken(parser);
        expr1 = parseReadBooleanComparison(parser);
        expr0 = concatExpression(expr0, expr1);
        expr0 = newExpression(token->type, expr0);
    } else if (token != NULL && token->type == TOKEN_NOT) {
        expr0 = newExpression(token->type, expr0);
    }
    return expr0;
};

/**
 @brief reads and performs a boolean 'and' operation (if found)
 @param[in] parser input ParserT structure
 @return sub-expression
*/
Expression *parseReadBooleanAnd(ParserT *parser) {
    Expression *expr0 = NULL, *expr1 = NULL, *expr2 = NULL;
    TokenT *token = parseNextToken(parser);
    expr0 = parseReadBooleanEquality(parser);
    token = parseNextToken(parser);

    while (token != NULL && token->type == TOKEN_AND) {
        parseEatToken(parser);
        expr1 = parseReadBooleanEquality(parser);
        expr0 = concatExpression(expr0, expr1);
        expr2 = newExpression(token->type, expr0);
        expr0 = expr2;
        token = parseNextToken(parser);
    }
    return expr0;

};

/**
 @brief reads and performs a boolean or operation (if found)
 @param[in] parser input ParserT structure
 @return expression
*/
Expression *parseReadBooleanOr(ParserT *parser) {
    Expression *expr0, *expr1 = NULL, *expr2 = NULL;
    TokenT *token = parseNextToken(parser);
    expr0 = parseReadBooleanAnd(parser);
    token = parseNextToken(parser);

    while (token != NULL && token->type == TOKEN_OR) {
        parseEatToken(parser);
        expr1 = parseReadBooleanAnd(parser);
        expr0 = concatExpression(expr0, expr1);
        expr2 = newExpression(token->type, expr0);
        expr0 = expr2;
        token = parseNextToken(parser);
    }

    return expr0;
};
