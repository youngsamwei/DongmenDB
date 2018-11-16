//
// Created by Sam on 2018/1/17.
//

#include <utils/utils.h>
#include "dongmensql/dongmensql.h"


/**
 * @brief 从后序表达式产生中序表达式字符串
 * @param exprs 表达式字符串
 * @param expr 表达式
 * @return 需要继续处理的后序表达式
 */
int printTermExpression(char *exprs, TermExpr *term) {
    if (term != NULL) {
        if (term->t == TERM_ID) {
            strcat(exprs, term->id);
        } else if (term->t == TERM_COLREF) {
            strcat(exprs, term->ref->allName);
        } else if (term->t == TERM_LITERAL){
            Literal_t *val = term->val;
            if (val->t == DATA_TYPE_CHAR || val->t == DATA_TYPE_TEXT) {
                strcat(exprs,val->val.strval);
            }else if (val->t == DATA_TYPE_INT || val->t == DATA_TYPE_DOUBLE){
                char *ival = (char *)calloc(20,1);
                itoa(val->val.ival, ival, 10);
                strcat(exprs, ival);
            }
        } else if(term->t == TERM_FUNC){

        }
        return 1;
    }
    return 0;
}

const char *getOperatorDesc(TokenType type) {
    switch (type) {
        case TOKEN_NOT:
            return " NOT ";
        case TOKEN_AND:
            return " AND ";
        case TOKEN_OR:
            return " OR ";
        case TOKEN_PLUS:
            return " + ";
        case TOKEN_MINUS:
            return " - ";
        case TOKEN_MULTIPLY:
            return " * ";
        case TOKEN_DIVIDE:
            return " / ";
        case TOKEN_GT:
            return " > ";
        case TOKEN_LT:
            return " < ";
        case TOKEN_LE:
            return " <= ";
        case TOKEN_GE:
            return " >= ";
        case TOKEN_EQ:
            return " = ";
        case TOKEN_NOT_EQUAL:
            return " != ";
        case TOKEN_COMMA:
            return ", ";
        case TOKEN_ASSIGNMENT:
            return " = ";
        case TOKEN_LIKE:
            return " LIKE ";
        case TOKEN_IN:
            return " IN ";
        default:
            return " UNKNOWN ";
    }

};

Expression *newExpression(TokenType type, Expression *nextexpr) {
    Expression *expr = (Expression *) calloc(sizeof(Expression), 1);
    expr->opType = type;
    expr->nextexpr = nextexpr;
    expr->term = NULL;
    expr->alias = NULL;
}

TermExpr *newTermExpr() {
    TermExpr *expr = (TermExpr *) calloc(sizeof(TermExpr), 1);
    expr->t = TERM_UNKNOWN;
    return expr;
}

char *getExpressionNamesTitle(arraylist *exprs){
    char *title = (char *)calloc(1024,1);

    for (int i = 0; i <= exprs->size - 1;i++){
        Expression *expr = arraylist_get(exprs, i);
        expression_print(expr, title);
        strcat(title, "\t");
    }
    return title;
};

int expression_free(Expression *expr) {};

int expression_free_list(arraylist *expr) {};

Expression *expression_print(Expression *expr, char *desc) {
    if (!expr) return NULL;

    Expression *result;
    if (expr->term) {
        if (desc) {
            printTermExpression(desc, expr->term);
        } else {
            char *t = new_id_name();
            printTermExpression(t, expr->term);
            printf(t);
        }
        result = expr->nextexpr;
    } else {
        if (desc) {
            strcat(desc, "(");
        } else {
            printf("(");
        }
        OPERATOR op = operators[expr->opType];

        if (op.numbers == 1) {
            if (desc) {
                strcat(desc, getOperatorDesc(expr->opType));
            } else {
                printf(getOperatorDesc(expr->opType));
            }
            result = expression_print(expr->nextexpr, desc);

        } else if (op.numbers == 2) {
            result = expression_print(expr->nextexpr, desc);
            if (desc) {
                strcat(desc, getOperatorDesc(expr->opType));
            } else {
                printf(getOperatorDesc(expr->opType));
            }
            result = expression_print(result, desc);
        }
        if (desc) {
            strcat(desc, ")");
        } else {
            printf(")");
        }
    }

    if (expr->alias) {
        if (desc) {
            strcat(desc, " as ");
            strcat(desc, expr->alias);
        } else {
            printf(" as %s", expr->alias);
        }
    }
    return result;
};

int expression_print_list(arraylist *exprlist) {
    Expression *expr1 = arraylist_get(exprlist, 0);
    printf("[");
    expression_print(expr1, NULL);

    for (int i = 1; i <= exprlist->size - 1; i++) {
        expr1 = arraylist_get(exprlist, i);
        printf(", ");
        expression_print(expr1, NULL);
    }
    printf("]");
};

/*使用递归下降法解析表达式
 *
 * 支持：
 * 算术运算 + - * /
 * 比较运算: > < != <= >=
 * 逻辑运算: NOT AND OR
 * 函数:  round, ltrim （仅作为例子，没有实现）
 * 字段变量: 类似student.sname
 * 支持简单的语法检查。
 * 支持的数据类型:数值，字符串
 *
 * 尚未支持的运算符： like， between..and 等
 *
 * 此功能属于语法检查，尚不支持语义检查。比如函数操作符操作数类型匹配，函数传参类型匹配检查等。
 *
 * 入口函数：parseExpressionRD
 * */


Expression *parseExpressionRD(ParserT *parser) {
    Expression *expr = parseReadBooleanOr(parser);
    if (parser->parserStateType != PARSER_WRONG && parser->currToken != NULL
        && parser->currToken->type != TOKEN_RESERVED_WORD && parser->currToken->type != TOKEN_COMMA &&
        parser->currToken->type != TOKEN_SEMICOLON) {
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

        /*处理数值,当前支持整型*/
        expr0 = newExpression(token->type, NULL);
        TermExpr *term = newTermExpr();
        term->t = TERM_LITERAL;
        Literal_t *literal
                = newLiteral(DATA_TYPE_INT);
        literal->val.ival = atoi(token->text);
        term->val = literal;
        expr0->term = term;
        parseEatToken(parser);
        return expr0;
    } else if (token->type == TOKEN_STRING || token->type == TOKEN_CHAR) {
        /*处理string和char,都转换为string*/
        expr0 = newExpression(TOKEN_STRING, NULL);
        TermExpr *term = newTermExpr();
        term->t = TERM_LITERAL;
        Literal_t *literal
                = newLiteral(DATA_TYPE_CHAR);

        /*去掉引号*/
        if (token->text[0] == '"') {
            int len = strlen(token->text) - 1;
            char *v = token->text + 1;
            char *value = (char *) calloc(len, 1);
            strcpy(value, v);
            value[len - 1] = '\0';
            literal->val.strval = value;
        }else{
            literal->val.strval = strdup(token->text);
        }

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

Expression *parseReadBuiltin(ParserT *parser) {
    Expression *expr0 = NULL, *expr1 = NULL;
    TokenT *token = parseNextToken(parser);
    if (token == NULL) {
        return parseError(parser, "syntax error: missing id or number.");
    }
    if (token->type == TOKEN_WORD) {
        char *text = strdup(token->text);
        token = parseEatAndNextToken(parser);

        if (token != NULL && token->type == TOKEN_OPEN_PAREN) {
            /*函数*/
            if (stricmp(text, "ltrim") == 0) {
                /*函数参数个数的检测*/
                token = parseEatAndNextToken(parser);
                /*如果紧跟着右括号，则丢失参数*/
                if (token->type == TOKEN_CLOSE_PAREN) {
                    char message[PARSER_MESSAGE_LENTTH];
                    sprintf(message, "syntax error: function %s missing argument.", text);
                    return parseError(parser, message);
                }
                Expression *param0 = parseReadArgument(parser);
                token = parseNextToken(parser);
                /*如果解析参数完毕，接下来是逗号，则太多参数*/
                if (token->type == TOKEN_COMMA) {
                    char message[PARSER_MESSAGE_LENTTH];
                    sprintf(message, "syntax error: function %s too many  argument.", text);
                    return parseError(parser, message);
                }
                expr0 = newExpression(TOKEN_FUN, param0);
            } else if (stricmp(text, "round") == 0) {
                /*函数参数个数的检测*/
                token = parseEatAndNextToken(parser);
                if (token->type == TOKEN_CLOSE_PAREN) {
                    char message[PARSER_MESSAGE_LENTTH];
                    sprintf(message, "syntax error: function %s missing  argument.", text);
                    return parseError(parser, message);
                }
                Expression *param0 = parseReadArgument(parser);
                token = parseNextToken(parser);
                if (token->type != TOKEN_COMMA) {
                    char message[PARSER_MESSAGE_LENTTH];
                    sprintf(message, "syntax error: function %s missing  argument.", text);
                    return parseError(parser, message);
                }
                Expression *param1 = parseReadArgument(parser);
                token = parseNextToken(parser);
                if (token->type == TOKEN_COMMA) {
                    char message[PARSER_MESSAGE_LENTTH];
                    sprintf(message, "syntax error: function %s too many  argument.", text);
                    return parseError(parser, message);
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
            /*如果是function，则在expr的term_id中保存函数名称*/
            TermExpr *term = newTermExpr();
            term->t = TERM_FUNC;
            term->id = text;
            expr0->term = term;
            return expr0;
        } else {
            /*此处是标识符处理: column_ref, 两种形式 student.*, student.sno,  sno */
            ColumnReference_t *columnReference = column_get_reference(text);
            expr0 = newExpression(TOKEN_WORD, NULL);
            TermExpr *term = newTermExpr();
            term->t = TERM_COLREF;
            term->ref = columnReference;
            expr0->term = term;
        }
    } else if (token->type == TOKEN_MULTIPLY) {
        /* 在select中的* */
        char *text = strdup(token->text);
        ColumnReference_t *columnReference = column_get_reference(text);
        expr0 = newExpression(TOKEN_WORD, NULL);
        TermExpr *term = newTermExpr();
        term->t = TERM_COLREF;
        term->ref = columnReference;
        expr0->term = term;
        parseEatAndNextToken(parser);
    } else {
        expr0 = parseReadLiteral(parser);
    }
    return expr0;
};

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
