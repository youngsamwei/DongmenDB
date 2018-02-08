//
// Created by Sam on 2018/2/7.
//

#include "physicalscan.h"

physical_scan *physical_scan_generate(dongmengdb *db, SRA_t *sra, transaction *tx) {
    if (!sra) return NULL;
    switch (sra->t) {
        case SRA_TABLE: {
            physical_scan *physicalScanTable = physical_scan_table_create(db, sra->table.ref->table_name, tx);
            return physicalScanTable;
        }
        case SRA_SELECT: {
            physical_scan *scan = physical_scan_generate(db, sra->select.sra, tx);
            physical_scan *physicalScanSelect = physical_scan_select_create(scan);
            physicalScanSelect->physicalScanSelect->cond = sra->select.cond;
            return physicalScanSelect;
        }
        case SRA_PROJECT: {
            physical_scan *scan = physical_scan_generate(db, sra->select.sra, tx);
            physical_scan *physicalScanProject = physical_scan_project_create(scan);
            physicalScanProject->physicalScanProject->order_by = sra->project.order_by;
            physicalScanProject->physicalScanProject->group_by = sra->project.group_by;
            physicalScanProject->physicalScanProject->distinct = sra->project.distinct;
            physicalScanProject->physicalScanProject->expr_list = sra->project.expr_list;
            return physicalScanProject;
        }
        case SRA_UNION:
            return NULL;
        case SRA_EXCEPT:
            return NULL;
        case SRA_INTERSECT:
            return NULL;
        case SRA_JOIN: {//cross join , product
            physical_scan *scan1 = physical_scan_generate(db, sra->binary.sra1, tx);
            physical_scan *scan2 = physical_scan_generate(db, sra->binary.sra2, tx);
            physical_scan *physicalScanJoin = physical_scan_join_nest_loop_create(scan1, scan2);
            physicalScanJoin->physicalScanJoinNestLoop->cond = NULL;
            return physicalScanJoin;
        }
        case SRA_NATURAL_JOIN:
            return NULL;
        case SRA_LEFT_OUTER_JOIN:
        case SRA_RIGHT_OUTER_JOIN:
        case SRA_FULL_OUTER_JOIN:
            return NULL;
        default:
            return NULL;
    }
};

/**
 *计算后序表达式的值
 * @param expr
 * @param scan
 * @param var
 * @return 后序表达式中的下一个表达式
 */
Expression *physical_scan_evaluate_expression(Expression *expr, physical_scan *scan, variant *var) {
    if (!expr) return NULL;
    switch (expr->opType) {
        case TOKEN_NOT: {
            variant *var1 = (variant *) calloc(sizeof(variant), 1);
            Expression *nextexpr1 = physical_scan_evaluate_expression(expr, scan, var1);
            var->type = DATA_TYPE_BOOLEAN;
            var->booleanValue = !var1->booleanValue;
            return nextexpr1;
        }
        case TOKEN_AND: {
            variant *var1 = (variant *) calloc(sizeof(variant), 1);
            variant *var2 = (variant *) calloc(sizeof(variant), 1);
            Expression *nextexpr1 = physical_scan_evaluate_expression(expr, scan, var1);
            Expression *nextexpr2 = physical_scan_evaluate_expression(nextexpr1->nextexpr, scan, var1);
            var->type = DATA_TYPE_BOOLEAN;
            var->booleanValue = var1->booleanValue && var2->booleanValue;
            return nextexpr2;
        }
        case TOKEN_OR: {
            variant *var1 = (variant *) calloc(sizeof(variant), 1);
            variant *var2 = (variant *) calloc(sizeof(variant), 1);
            Expression *nextexpr1 = physical_scan_evaluate_expression(expr, scan, var1);
            Expression *nextexpr2 = physical_scan_evaluate_expression(nextexpr1->nextexpr, scan, var1);
            var->type = DATA_TYPE_BOOLEAN;
            var->booleanValue = var1->booleanValue || var2->booleanValue;
            return nextexpr2;
        }
        case TOKEN_PLUS: {
            variant *var1 = (variant *) calloc(sizeof(variant), 1);
            variant *var2 = (variant *) calloc(sizeof(variant), 1);
            Expression *nextexpr1 = physical_scan_evaluate_expression(expr, scan, var1);
            Expression *nextexpr2 = physical_scan_evaluate_expression(nextexpr1->nextexpr, scan, var1);
            var->type = DATA_TYPE_INT;
            var->intValue = var1->intValue + var2->intValue;
            return nextexpr2;
        }
        case TOKEN_MINUS: {
            variant *var1 = (variant *) calloc(sizeof(variant), 1);
            variant *var2 = (variant *) calloc(sizeof(variant), 1);
            Expression *nextexpr1 = physical_scan_evaluate_expression(expr, scan, var1);
            Expression *nextexpr2 = physical_scan_evaluate_expression(nextexpr1->nextexpr, scan, var1);
            var->type = DATA_TYPE_INT;
            var->intValue = var1->intValue - var2->intValue;
            return nextexpr2;
        }
        case TOKEN_MULTIPLY: {
            variant *var1 = (variant *) calloc(sizeof(variant), 1);
            variant *var2 = (variant *) calloc(sizeof(variant), 1);
            Expression *nextexpr1 = physical_scan_evaluate_expression(expr, scan, var1);
            Expression *nextexpr2 = physical_scan_evaluate_expression(nextexpr1->nextexpr, scan, var1);
            var->type = DATA_TYPE_INT;
            var->intValue = var1->intValue * var2->intValue;
            return nextexpr2;
        }
        case TOKEN_DIVIDE: {
            variant *var1 = (variant *) calloc(sizeof(variant), 1);
            variant *var2 = (variant *) calloc(sizeof(variant), 1);
            Expression *nextexpr1 = physical_scan_evaluate_expression(expr, scan, var1);
            Expression *nextexpr2 = physical_scan_evaluate_expression(nextexpr1->nextexpr, scan, var1);
            var->type = DATA_TYPE_INT;
            var->intValue = var1->intValue / var2->intValue;
            return nextexpr2;
        }
        case TOKEN_GT: {
            variant *var1 = (variant *) calloc(sizeof(variant), 1);
            variant *var2 = (variant *) calloc(sizeof(variant), 1);
            Expression *nextexpr1 = physical_scan_evaluate_expression(expr, scan, var1);
            Expression *nextexpr2 = physical_scan_evaluate_expression(nextexpr1->nextexpr, scan, var1);
            var->type = DATA_TYPE_BOOLEAN;
            var->booleanValue = var1->intValue > var2->intValue;
            return nextexpr2;
        }
        case TOKEN_LT: {
            variant *var1 = (variant *) calloc(sizeof(variant), 1);
            variant *var2 = (variant *) calloc(sizeof(variant), 1);
            Expression *nextexpr1 = physical_scan_evaluate_expression(expr, scan, var1);
            Expression *nextexpr2 = physical_scan_evaluate_expression(nextexpr1->nextexpr, scan, var1);
            var->type = DATA_TYPE_BOOLEAN;
            var->booleanValue = var1->intValue < var2->intValue;
            return nextexpr2;
        }
        case TOKEN_LE: {
            variant *var1 = (variant *) calloc(sizeof(variant), 1);
            variant *var2 = (variant *) calloc(sizeof(variant), 1);
            Expression *nextexpr1 = physical_scan_evaluate_expression(expr, scan, var1);
            Expression *nextexpr2 = physical_scan_evaluate_expression(nextexpr1->nextexpr, scan, var1);
            var->type = DATA_TYPE_BOOLEAN;
            var->booleanValue = var1->intValue <= var2->intValue;
            return nextexpr2;
        }
        case TOKEN_GE: {
            variant *var1 = (variant *) calloc(sizeof(variant), 1);
            variant *var2 = (variant *) calloc(sizeof(variant), 1);
            Expression *nextexpr1 = physical_scan_evaluate_expression(expr, scan, var1);
            Expression *nextexpr2 = physical_scan_evaluate_expression(nextexpr1->nextexpr, scan, var1);
            var->type = DATA_TYPE_BOOLEAN;
            var->booleanValue = var1->intValue >= var2->intValue;
            return nextexpr2;
        }
        case TOKEN_EQ: {
            variant *var1 = (variant *) calloc(sizeof(variant), 1);
            variant *var2 = (variant *) calloc(sizeof(variant), 1);
            Expression *nextexpr1 = physical_scan_evaluate_expression(expr, scan, var1);
            Expression *nextexpr2 = physical_scan_evaluate_expression(nextexpr1->nextexpr, scan, var1);
            var->type = DATA_TYPE_BOOLEAN;
            var->booleanValue = var1->intValue == var2->intValue;
            return nextexpr2;
        }
        case TOKEN_NOT_EQUAL: {
            variant *var1 = (variant *) calloc(sizeof(variant), 1);
            variant *var2 = (variant *) calloc(sizeof(variant), 1);
            Expression *nextexpr1 = physical_scan_evaluate_expression(expr, scan, var1);
            Expression *nextexpr2 = physical_scan_evaluate_expression(nextexpr1->nextexpr, scan, var1);
            var->type = DATA_TYPE_BOOLEAN;
            var->booleanValue = var1->intValue != var2->intValue;
            return nextexpr2;
        }
        case TOKEN_COMMA:
            return NULL;
        case TOKEN_ASSIGNMENT:
            return NULL;
        case TOKEN_LIKE:
            return NULL;
        case TOKEN_IN:
            return NULL;
        case TOKEN_FUN:
            return NULL;
        case TOKEN_WORD: {
            char *fieldName = expr->term->id;
            field_info *fi = scan->getField(scan, fieldName);
            if (fi->type == DATA_TYPE_INT) {
                var->type = DATA_TYPE_INT;
                var->intValue = scan->getInt(scan, fieldName);
                return NULL;
            } else if (fi->type == DATA_TYPE_CHAR) {
                var->type = DATA_TYPE_CHAR;
                var->strValue = (char *) calloc(fi->length, 1);
                scan->getString(scan, fieldName, var->strValue);

                return NULL;
            }
            return NULL;
        }
        case TOKEN_DECIMAL:{
            var->type = DATA_TYPE_INT;
            var->intValue = expr->term->val->val.ival;
            return NULL;
        }
        case TOKEN_STRING:
        case TOKEN_CHAR: {
            var->type = DATA_TYPE_CHAR;
            var->strValue = strdup(expr->term->val->val.strval);
            return NULL;
        }
        default:
            return NULL;
    }
};
