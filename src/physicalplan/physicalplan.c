//
// Created by Sam on 2018/2/2.
//

#include "physicalplan/physicalplan.h"
#include "physicalplan/physicalscan.h"
#include "physicalplan/physical_scan_table.h"

/*将关系代数表达式转换为物理扫描计划*/
physical_scan *plan_execute_select(dongmendb *db, SRA_t *sra, transaction *tx){
    physical_scan *plan = physical_scan_generate(db, sra, tx);
    if (plan->scanType == SCAN_TYPE_PROJECT) {
        /*处理select 中形如 student.* */
        physical_scan_project_generate_expr_list(plan);
    }
    return plan;
};



/**
 * insert数据，一次一条
 * @param db
 * @param tableName
 * @param fieldNames
 * @param values
 * @param tx
 * @return
 */
int plan_execute_insert(dongmendb *db, char *tableName, arraylist *fieldNames, arraylist *values, transaction *tx){
    physical_scan *scan = physical_scan_table_create(db, tableName, tx);
    scan->insert(scan);
    for (size_t i = 0; i < fieldNames->size; i++){

        char *fieldName = arraylist_get(fieldNames, i);

        void_ptr *ptr = (void_ptr *) calloc(sizeof(void_ptr *), 1);
        hashmap_get(scan->physicalScanTable->tableInfo->fields, fieldName, ptr);
        field_info *fieldInfo = *ptr;

        /* TODO: 完整性检查 */
        int type = fieldInfo->type;
        if (type == DATA_TYPE_INT) {
            integer *val = arraylist_get(values, i);
            scan->setInt(scan, tableName, fieldName, val->val);
        }else if (type == DATA_TYPE_CHAR){
            char *val = arraylist_get(values, i);
            /*字符串超出定义时的长度，则截断字符串.*/
            if(fieldInfo->length<strlen(val)){
                val[fieldInfo->length] = '\0';
            }
            scan->setString(scan, tableName, fieldName, val);
        }else{
            return DONGMENDB_EINVALIDSQL;
        }
    }
    return DONGMENDB_OK;
};

int plan_execute_create_table(char *tableName, table_info *tableInfo, transaction *tx){

};



/*物理操作选择：物理优化*/
physical_scan *physical_scan_generate(dongmendb *db, SRA_t *sra, transaction *tx) {
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
            /*处理 形如student.*的字段表示 */
            physicalScanProject->physicalScanProject->original_expr_list = sra->project.expr_list;
            physicalScanProject->physicalScanProject->expr_list = arraylist_create();

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
            Expression *nextexpr1 = physical_scan_evaluate_expression(expr->nextexpr, scan, var1);
            var->type = DATA_TYPE_BOOLEAN;
            var->booleanValue = !var1->booleanValue;
            return nextexpr1;
        }
        case TOKEN_AND: {
            variant *var1 = (variant *) calloc(sizeof(variant), 1);
            variant *var2 = (variant *) calloc(sizeof(variant), 1);
            Expression *nextexpr1 = physical_scan_evaluate_expression(expr->nextexpr, scan, var1);
            Expression *nextexpr2 = physical_scan_evaluate_expression(nextexpr1, scan, var2);
            var->type = DATA_TYPE_BOOLEAN;
            var->booleanValue = var1->booleanValue && var2->booleanValue;
            return nextexpr2;
        }
        case TOKEN_OR: {
            variant *var1 = (variant *) calloc(sizeof(variant), 1);
            variant *var2 = (variant *) calloc(sizeof(variant), 1);
            Expression *nextexpr1 = physical_scan_evaluate_expression(expr->nextexpr, scan, var1);
            Expression *nextexpr2 = physical_scan_evaluate_expression(nextexpr1, scan, var2);
            var->type = DATA_TYPE_BOOLEAN;
            var->booleanValue = var1->booleanValue || var2->booleanValue;
            return nextexpr2;
        }
        case TOKEN_PLUS: {
            variant *var1 = (variant *) calloc(sizeof(variant), 1);
            variant *var2 = (variant *) calloc(sizeof(variant), 1);
            Expression *nextexpr1 = physical_scan_evaluate_expression(expr->nextexpr, scan, var1);
            Expression *nextexpr2 = physical_scan_evaluate_expression(nextexpr1, scan, var2);
            var->type = DATA_TYPE_INT;
            var->intValue = var1->intValue + var2->intValue;
            return nextexpr2;
        }
        case TOKEN_MINUS: {
            variant *var1 = (variant *) calloc(sizeof(variant), 1);
            variant *var2 = (variant *) calloc(sizeof(variant), 1);
            Expression *nextexpr1 = physical_scan_evaluate_expression(expr->nextexpr, scan, var1);
            Expression *nextexpr2 = physical_scan_evaluate_expression(nextexpr1, scan, var2);
            var->type = DATA_TYPE_INT;
            var->intValue = var1->intValue - var2->intValue;
            return nextexpr2;
        }
        case TOKEN_MULTIPLY: {
            variant *var1 = (variant *) calloc(sizeof(variant), 1);
            variant *var2 = (variant *) calloc(sizeof(variant), 1);
            Expression *nextexpr1 = physical_scan_evaluate_expression(expr->nextexpr, scan, var1);
            Expression *nextexpr2 = physical_scan_evaluate_expression(nextexpr1, scan, var2);
            var->type = DATA_TYPE_INT;
            var->intValue = var1->intValue * var2->intValue;
            return nextexpr2;
        }
        case TOKEN_DIVIDE: {
            variant *var1 = (variant *) calloc(sizeof(variant), 1);
            variant *var2 = (variant *) calloc(sizeof(variant), 1);
            Expression *nextexpr1 = physical_scan_evaluate_expression(expr->nextexpr, scan, var1);
            Expression *nextexpr2 = physical_scan_evaluate_expression(nextexpr1, scan, var2);
            var->type = DATA_TYPE_INT;
            var->intValue = var1->intValue / var2->intValue;
            return nextexpr2;
        }
        case TOKEN_GT: {
            variant *var1 = (variant *) calloc(sizeof(variant), 1);
            variant *var2 = (variant *) calloc(sizeof(variant), 1);
            Expression *nextexpr1 = physical_scan_evaluate_expression(expr->nextexpr, scan, var1);
            Expression *nextexpr2 = physical_scan_evaluate_expression(nextexpr1, scan, var2);
            var->type = DATA_TYPE_BOOLEAN;
            var->booleanValue = 0;
            if (var1->type == DATA_TYPE_INT) {
                var->booleanValue = var1->intValue > var2->intValue;
            } else if (var1->type == DATA_TYPE_CHAR) {
                var->booleanValue = stricmp(var1->strValue, var2->strValue);
            }
            return nextexpr2;
        }
        case TOKEN_LT: {
            variant *var1 = (variant *) calloc(sizeof(variant), 1);
            variant *var2 = (variant *) calloc(sizeof(variant), 1);
            Expression *nextexpr1 = physical_scan_evaluate_expression(expr->nextexpr, scan, var1);
            Expression *nextexpr2 = physical_scan_evaluate_expression(nextexpr1, scan, var2);
            var->type = DATA_TYPE_BOOLEAN;
            var->booleanValue = 0;
            if (var1->type == DATA_TYPE_INT) {
                var->booleanValue = var1->intValue < var2->intValue;
            } else if (var1->type == DATA_TYPE_CHAR) {
                var->booleanValue = stricmp(var1->strValue, var2->strValue);
            }
            return nextexpr2;
        }
        case TOKEN_LE: {
            variant *var1 = (variant *) calloc(sizeof(variant), 1);
            variant *var2 = (variant *) calloc(sizeof(variant), 1);
            Expression *nextexpr1 = physical_scan_evaluate_expression(expr->nextexpr, scan, var1);
            Expression *nextexpr2 = physical_scan_evaluate_expression(nextexpr1, scan, var2);
            var->type = DATA_TYPE_BOOLEAN;
            var->booleanValue = 0;
            if (var1->type == DATA_TYPE_INT) {
                var->booleanValue = var1->intValue <= var2->intValue;
            } else if (var1->type == DATA_TYPE_CHAR) {
                var->booleanValue = stricmp(var1->strValue, var2->strValue);
            }

            return nextexpr2;
        }
        case TOKEN_GE: {
            variant *var1 = (variant *) calloc(sizeof(variant), 1);
            variant *var2 = (variant *) calloc(sizeof(variant), 1);
            Expression *nextexpr1 = physical_scan_evaluate_expression(expr->nextexpr, scan, var1);
            Expression *nextexpr2 = physical_scan_evaluate_expression(nextexpr1, scan, var2);
            var->type = DATA_TYPE_BOOLEAN;
            var->booleanValue = 0;
            if (var1->type == DATA_TYPE_INT) {
                var->booleanValue = var1->intValue >= var2->intValue;
            } else if (var1->type == DATA_TYPE_CHAR) {
                var->booleanValue = stricmp(var1->strValue, var2->strValue);
            }

            return nextexpr2;
        }
        case TOKEN_EQ: {
            variant *var1 = (variant *) calloc(sizeof(variant), 1);
            variant *var2 = (variant *) calloc(sizeof(variant), 1);
            Expression *nextexpr1 = physical_scan_evaluate_expression(expr->nextexpr, scan, var1);
            Expression *nextexpr2 = physical_scan_evaluate_expression(nextexpr1, scan, var2);
            var->type = DATA_TYPE_BOOLEAN;
            var->booleanValue = 0;
            if (var1->type == DATA_TYPE_INT) {
                var->booleanValue = var1->intValue == var2->intValue;
            } else if (var1->type == DATA_TYPE_CHAR) {
                var->booleanValue = !stricmp(var1->strValue, var2->strValue);
            }
            return nextexpr2;
        }
        case TOKEN_NOT_EQUAL: {
            variant *var1 = (variant *) calloc(sizeof(variant), 1);
            variant *var2 = (variant *) calloc(sizeof(variant), 1);
            Expression *nextexpr1 = physical_scan_evaluate_expression(expr->nextexpr, scan, var1);
            Expression *nextexpr2 = physical_scan_evaluate_expression(nextexpr1, scan, var2);
            var->type = DATA_TYPE_BOOLEAN;
            var->booleanValue = 0;
            if (var1->type == DATA_TYPE_INT) {
                var->booleanValue = var1->intValue != var2->intValue;
            } else if (var1->type == DATA_TYPE_CHAR) {
                var->booleanValue = stricmp(var1->strValue, var2->strValue);
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
                char *fieldName = expr->term->ref->columnName;/*包含表名的字段名全称*/
                char *tableName = expr->term->ref->tableName;
                field_info *fi = scan->getField(scan, tableName, fieldName);
                switch (fi->type) {
                    case DATA_TYPE_INT:
                    case DATA_TYPE_DOUBLE:
                        var->type = DATA_TYPE_INT;
                        var->intValue = scan->getInt(scan, tableName, fieldName);
                        return expr->nextexpr;
                    case DATA_TYPE_CHAR:
                    case DATA_TYPE_TEXT:
                        var->type = DATA_TYPE_CHAR;
                        var->strValue = (char *) calloc(fi->length, 1);
                        scan->getString(scan, tableName, fieldName, var->strValue);
                        return expr->nextexpr;
                    case DATA_TYPE_BOOLEAN:
                        var->type = DATA_TYPE_BOOLEAN;
                        var->booleanValue = scan->getInt(scan, tableName, fieldName);
                        return expr->nextexpr;
                }
            } else if (type == TERM_FUNC){
                /*函数*/
            }
            return expr->nextexpr;
        }
        case TOKEN_DECIMAL: {
            var->type = DATA_TYPE_INT;
            var->intValue = expr->term->val->val.ival;
            return expr->nextexpr;
        }
        case TOKEN_STRING:
        case TOKEN_CHAR: {
            var->type = DATA_TYPE_CHAR;
            var->strValue = strdup(expr->term->val->val.strval);
            return expr->nextexpr;
        }
        default:
            return expr->nextexpr;
    }
};
