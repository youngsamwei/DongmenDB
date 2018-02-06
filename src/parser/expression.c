//
// Created by Sam on 2018/1/17.
//

#include "dongmengsql.h"

char *printExpression(char *exprs, Expression *expr) {
    Expression *expr0 = expr;
    while (expr0 != NULL) {
        char *desc = getExpressionDesc(expr0);
        strcat(exprs, desc);
        if (expr0->nextexpr != NULL) {
            strcat(exprs, " , ");
        }
        expr0 = expr0->nextexpr;
    }
    return exprs;
};

/**
 * @brief 从后序表达式产生中序表达式字符串
 * @param exprs 表达式字符串
 * @param expr 表达式
 * @return 需要继续处理的后序表达式
 */
Expression *printRNExpression(char *exprs, Expression *expr) {
    if (expr->term != NULL) {
        if (expr->term->t ==TERM_ID){
            strcat(exprs, expr->term->id);
        }else {
            strcat(exprs, expr->term->val->val.strval);
        }
        return expr;
    } else {
        int operrandNumber = operators[expr->opType].numbers;
        if ( operrandNumber== 1){
            strcat(exprs, "(");
            strcat(exprs, getExpressionDesc(expr));
            strcat(exprs, " ");
            Expression *expr0 = printRNExpression(exprs, expr->nextexpr);
            strcat(exprs, " )");
            return expr0;
        } else if (operrandNumber== 2){
            strcat(exprs, "(");
            Expression *expr0 = printRNExpression(exprs, expr->nextexpr);
            strcat(exprs, " ");
            strcat(exprs, getExpressionDesc(expr));
            strcat(exprs, " ");
            expr0 = printRNExpression(exprs, expr0->nextexpr);
            strcat(exprs, ")");
            return expr0;
        }
    }
}

char *getExpressionDesc(Expression *expr) {
    switch (expr->opType) {
        case TOKEN_NOT:
            return "NOT";
        case TOKEN_AND:
            return "AND";
        case TOKEN_OR:
            return "OR";
        case TOKEN_PLUS:
            return "+";
        case TOKEN_MINUS:
            return "-";
        case TOKEN_MULTIPLY:
            return "*";
        case TOKEN_DIVIDE:
            return "/";
        case TOKEN_GT:
            return ">";
        case TOKEN_LT:
            return "<";
        case TOKEN_LE:
            return "<=";
        case TOKEN_GE:
            return ">=";
        case TOKEN_EQ:
            return "=";
        case TOKEN_NOT_EQUAL:
            return "!=";
        case TOKEN_COMMA:
            return ",";
        case TOKEN_ASSIGNMENT:
            return "=";
        case TOKEN_LIKE:
            return "LIKE";
        case TOKEN_IN:
            return "IN";
        case TOKEN_FUN:
            return expr->term->id;
        case TOKEN_WORD:
            return expr->term->id;
        case TOKEN_DECIMAL:
        case TOKEN_STRING:
        case TOKEN_CHAR: {
            return expr->term->val->val.strval;
        }
        default:
            return "UNKNOWN";
    }

};

int expression_free(Expression *expr){};
int expression_free_list(Expression *expr){};
int expression_print(Expression *expr){};
int expression_print_list(Expression *expr){};
