//
// Created by sam on 2018/11/16.
//
#include <physicalplan/Scan.h>
#include <dongmensql/column.h>

Expression *Scan::evaluateExpression(Expression *expr, Scan *scan, variant *var) {
    if (!expr) return NULL;
    switch (expr->opType) {
        case TOKEN_NOT: {
            variant *var1 = (variant *) calloc(sizeof(variant), 1);
            Expression *nextexpr1 = evaluateExpression(expr->nextexpr, scan, var1);
            var->type = DATA_TYPE_BOOLEAN;
            var->booleanValue = !var1->booleanValue;
            return nextexpr1;
        }
        case TOKEN_AND: {
            variant *var1 = (variant *) calloc(sizeof(variant), 1);
            variant *var2 = (variant *) calloc(sizeof(variant), 1);
            Expression *nextexpr1 = evaluateExpression(expr->nextexpr, scan, var1);
            Expression *nextexpr2 = evaluateExpression(nextexpr1, scan, var2);
            var->type = DATA_TYPE_BOOLEAN;
            var->booleanValue = var1->booleanValue && var2->booleanValue;
            return nextexpr2;
        }
        case TOKEN_OR: {
            variant *var1 = (variant *) calloc(sizeof(variant), 1);
            variant *var2 = (variant *) calloc(sizeof(variant), 1);
            Expression *nextexpr1 = evaluateExpression(expr->nextexpr, scan, var1);
            Expression *nextexpr2 = evaluateExpression(nextexpr1, scan, var2);
            var->type = DATA_TYPE_BOOLEAN;
            var->booleanValue = var1->booleanValue || var2->booleanValue;
            return nextexpr2;
        }
        case TOKEN_PLUS: {
            variant *var1 = (variant *) calloc(sizeof(variant), 1);
            variant *var2 = (variant *) calloc(sizeof(variant), 1);
            Expression *nextexpr1 = evaluateExpression(expr->nextexpr, scan, var1);
            Expression *nextexpr2 = evaluateExpression(nextexpr1, scan, var2);
            var->type = DATA_TYPE_INT;
            var->intValue = var1->intValue + var2->intValue;
            return nextexpr2;
        }
        case TOKEN_MINUS: {
            variant *var1 = (variant *) calloc(sizeof(variant), 1);
            variant *var2 = (variant *) calloc(sizeof(variant), 1);
            Expression *nextexpr1 = evaluateExpression(expr->nextexpr, scan, var1);
            Expression *nextexpr2 = evaluateExpression(nextexpr1, scan, var2);
            var->type = DATA_TYPE_INT;
            var->intValue = var1->intValue - var2->intValue;
            return nextexpr2;
        }
        case TOKEN_MULTIPLY: {
            variant *var1 = (variant *) calloc(sizeof(variant), 1);
            variant *var2 = (variant *) calloc(sizeof(variant), 1);
            Expression *nextexpr1 = evaluateExpression(expr->nextexpr, scan, var1);
            Expression *nextexpr2 = evaluateExpression(nextexpr1, scan, var2);
            var->type = DATA_TYPE_INT;
            var->intValue = var1->intValue * var2->intValue;
            return nextexpr2;
        }
        case TOKEN_DIVIDE: {
            variant *var1 = (variant *) calloc(sizeof(variant), 1);
            variant *var2 = (variant *) calloc(sizeof(variant), 1);
            Expression *nextexpr1 = evaluateExpression(expr->nextexpr, scan, var1);
            Expression *nextexpr2 = evaluateExpression(nextexpr1, scan, var2);
            var->type = DATA_TYPE_INT;
            var->intValue = var1->intValue / var2->intValue;
            return nextexpr2;
        }
        case TOKEN_GT: {
            variant *var1 = (variant *) calloc(sizeof(variant), 1);
            variant *var2 = (variant *) calloc(sizeof(variant), 1);
            Expression *nextexpr1 = evaluateExpression(expr->nextexpr, scan, var1);
            Expression *nextexpr2 = evaluateExpression(nextexpr1, scan, var2);
            var->type = DATA_TYPE_BOOLEAN;
            var->booleanValue = 0;
            if (var1->type == DATA_TYPE_INT) {
                var->booleanValue = var1->intValue > var2->intValue;
            } else if (var1->type == DATA_TYPE_CHAR) {
                var->booleanValue = strcmp_ic(var1->strValue, var2->strValue);
            }
            return nextexpr2;
        }
        case TOKEN_LT: {
            variant *var1 = (variant *) calloc(sizeof(variant), 1);
            variant *var2 = (variant *) calloc(sizeof(variant), 1);
            Expression *nextexpr1 = evaluateExpression(expr->nextexpr, scan, var1);
            Expression *nextexpr2 = evaluateExpression(nextexpr1, scan, var2);
            var->type = DATA_TYPE_BOOLEAN;
            var->booleanValue = 0;
            if (var1->type == DATA_TYPE_INT) {
                var->booleanValue = var1->intValue < var2->intValue;
            } else if (var1->type == DATA_TYPE_CHAR) {
                var->booleanValue = strcmp_ic(var1->strValue, var2->strValue);
            }
            return nextexpr2;
        }
        case TOKEN_LE: {
            variant *var1 = (variant *) calloc(sizeof(variant), 1);
            variant *var2 = (variant *) calloc(sizeof(variant), 1);
            Expression *nextexpr1 = evaluateExpression(expr->nextexpr, scan, var1);
            Expression *nextexpr2 = evaluateExpression(nextexpr1, scan, var2);
            var->type = DATA_TYPE_BOOLEAN;
            var->booleanValue = 0;
            if (var1->type == DATA_TYPE_INT) {
                var->booleanValue = var1->intValue <= var2->intValue;
            } else if (var1->type == DATA_TYPE_CHAR) {
                var->booleanValue = strcmp_ic(var1->strValue, var2->strValue);
            }

            return nextexpr2;
        }
        case TOKEN_GE: {
            variant *var1 = (variant *) calloc(sizeof(variant), 1);
            variant *var2 = (variant *) calloc(sizeof(variant), 1);
            Expression *nextexpr1 = evaluateExpression(expr->nextexpr, scan, var1);
            Expression *nextexpr2 = evaluateExpression(nextexpr1, scan, var2);
            var->type = DATA_TYPE_BOOLEAN;
            var->booleanValue = 0;
            if (var1->type == DATA_TYPE_INT) {
                var->booleanValue = var1->intValue >= var2->intValue;
            } else if (var1->type == DATA_TYPE_CHAR) {
                var->booleanValue = strcmp_ic(var1->strValue, var2->strValue);
            }

            return nextexpr2;
        }
        case TOKEN_EQ: {
            variant *var1 = (variant *) calloc(sizeof(variant), 1);
            variant *var2 = (variant *) calloc(sizeof(variant), 1);
            Expression *nextexpr1 = evaluateExpression(expr->nextexpr, scan, var1);
            Expression *nextexpr2 = evaluateExpression(nextexpr1, scan, var2);
            var->type = DATA_TYPE_BOOLEAN;
            var->booleanValue = 0;
            if (var1->type == DATA_TYPE_INT) {
                var->booleanValue = var1->intValue == var2->intValue;
            } else if (var1->type == DATA_TYPE_CHAR) {
                var->booleanValue = !strcmp_ic(var1->strValue, var2->strValue);
            }
            return nextexpr2;
        }
        case TOKEN_NOT_EQUAL: {
            variant *var1 = (variant *) calloc(sizeof(variant), 1);
            variant *var2 = (variant *) calloc(sizeof(variant), 1);
            Expression *nextexpr1 = evaluateExpression(expr->nextexpr, scan, var1);
            Expression *nextexpr2 = evaluateExpression(nextexpr1, scan, var2);
            var->type = DATA_TYPE_BOOLEAN;
            var->booleanValue = 0;
            if (var1->type == DATA_TYPE_INT) {
                var->booleanValue = var1->intValue != var2->intValue;
            } else if (var1->type == DATA_TYPE_CHAR) {
                var->booleanValue = strcmp_ic(var1->strValue, var2->strValue);
            }
            return nextexpr2;
        }
        case TOKEN_COMMA:
            return expr->nextexpr;
        case TOKEN_ASSIGNMENT:
            return expr->nextexpr;
        case TOKEN_LIKE:
            return expr->nextexpr;
        case TOKEN_IN:
            return expr->nextexpr;
        case TOKEN_FUN:
            return expr->nextexpr;
        case TOKEN_WORD: {
            /*两种情况：函数和字段名*/
            TermType type = expr->term->t;
            if (type == TERM_COLREF) {
                string tableName, fieldName;

                if (expr->term->ref->tableName) {
                    tableName = expr->term->ref->tableName;
                } else {
                    tableName = "";
                }
                if (expr->term->ref->columnName) {
                    fieldName = expr->term->ref->columnName;
                } else {
                    fieldName = "";
                }

                FieldInfo *fi = scan->getField(tableName, fieldName);
                switch (fi->type) {
                    case DATA_TYPE_INT:
                    case DATA_TYPE_DOUBLE:
                        var->type = DATA_TYPE_INT;
                        var->intValue = scan->getInt(tableName, fieldName);
                        return expr->nextexpr;
                    case DATA_TYPE_CHAR:
                    case DATA_TYPE_TEXT: {
                        var->type = DATA_TYPE_CHAR;
                        var->strValue = (char *) calloc(fi->length, 1);
                        string v = scan->getString(tableName, fieldName);
                        strcpy(var->strValue, v.c_str());
                        return expr->nextexpr;
                    }
                    case DATA_TYPE_BOOLEAN:
                        var->type = DATA_TYPE_BOOLEAN;
                        var->booleanValue = scan->getInt(tableName, fieldName);
                        return expr->nextexpr;
                }
            } else if (type == TERM_FUNC) {
                /*函数*/
            }
            return expr->nextexpr;
        }
        case TOKEN_DECIMAL: {
            var->type = DATA_TYPE_INT;
            IntLiteral *il = (IntLiteral*)expr->term->val;
            var->intValue = il->value;
            return expr->nextexpr;
        }
        case TOKEN_STRING:
        case TOKEN_CHAR: {
            var->type = DATA_TYPE_CHAR;
            var->strValue = strdup(expr->term->val->original_value);
            return expr->nextexpr;
        }
        default:
            return expr->nextexpr;
    }
};