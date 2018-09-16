//
// Created by Sam on 2018/2/7.
//

#include "physicalplan/physical_scan_project.h"

/*物理计划：投影的实现*/
physical_scan *physical_scan_project_create(physical_scan *scan) {
    physical_scan_project *physicalScanProject = (physical_scan_project *) calloc(sizeof(physical_scan_project), 1);
    physicalScanProject->scan = scan;

    physical_scan *physicalScan = (physical_scan *) calloc(sizeof(physical_scan), 1);
    physicalScan->scanType = SCAN_TYPE_PROJECT;
    physicalScan->physicalScanProject = physicalScanProject;
    physical_scan_project_init_scan(physicalScan);
    return physicalScan;
};

void physical_scan_project_init_scan(physical_scan *scan) {
    scan->beforeFirst = physical_scan_project_before_first;
    scan->next = physical_scan_project_next;
    scan->close = physical_scan_project_close;
    scan->getValByIndex = physical_scan_project_get_val_by_index;
    scan->getIntByIndex = physical_scan_project_get_int_by_index;
    scan->getStringByIndex = physical_scan_project_get_string_by_index;
    scan->getVal = physical_scan_project_get_val;
    scan->getInt = physical_scan_project_get_int;
    scan->getString = physical_scan_project_get_string;
    scan->getField = physical_scan_project_get_field;
    scan->hasField = physical_scan_project_has_field;
    scan->getFieldsName = physical_scan_project_get_fields_name;
    scan->setInt = NULL;
    scan->setString = NULL;
    scan->deleterec = NULL;
    scan->insert = NULL;
    scan->getRid = physical_scan_project_get_rid;
    scan->movetoRid = physical_scan_project_moveto_rid;
}

int physical_scan_project_before_first(physical_scan *scan) {
    physical_scan *scan1 = scan->physicalScanProject->scan;
    return scan1->beforeFirst(scan1);
};

int physical_scan_project_next(physical_scan *scan) {
    physical_scan *scan1 = scan->physicalScanProject->scan;
    return scan1->next(scan1);
};

int physical_scan_project_close(physical_scan *scan) {
    physical_scan *scan1 = scan->physicalScanProject->scan;
    return scan1->close(scan1);
};

variant *physical_scan_project_get_val(physical_scan *scan, char *tableName, char *fieldName){
    physical_scan *scan1 = scan->physicalScanProject->scan;
    return scan1->getVal(scan1, tableName, fieldName);
};

variant *physical_scan_project_get_val_by_index(physical_scan *scan, int index){
    arraylist *exprs = scan->physicalScanProject->expr_list;
    Expression *expr = arraylist_get(exprs, index);
    variant *var = (variant *)calloc(sizeof(variant),1);
    physical_scan_evaluate_expression(expr,scan, var);
    return var;
};

int physical_scan_project_get_int_by_index(physical_scan *scan, int index) {
    arraylist *exprs = scan->physicalScanProject->expr_list;
    Expression *expr = arraylist_get(exprs, index);
    variant *var = (variant *)calloc(sizeof(variant),1);
    physical_scan_evaluate_expression(expr,scan, var);
    if (var->type == DATA_TYPE_INT){
        return var->intValue;
    }else{
        return 0;
    }
};

int physical_scan_project_get_int(physical_scan *scan, char *tableName, char *fieldName) {
    physical_scan *scan1 = scan->physicalScanProject->scan;
    return scan1->getInt(scan1, tableName, fieldName);
};

int physical_scan_project_get_string_by_index(physical_scan *scan, int index, char *value) {
    arraylist *exprs = scan->physicalScanProject->expr_list;
    Expression *expr = arraylist_get(exprs, index);
    variant *var = (variant *)calloc(sizeof(variant),1);
    physical_scan_evaluate_expression(expr,scan, var);
    if (var->type == DATA_TYPE_CHAR){
        value= var->strValue;
        return 1;
    }else{
        return 0;
    }
};

int physical_scan_project_get_string(physical_scan *scan, char *tableName, char *fieldName, char *value) {
    physical_scan *scan1 = scan->physicalScanProject->scan;
    return scan1->getString(scan1, tableName, fieldName, value);
};

int physical_scan_project_has_field(physical_scan *scan, char *tableName, char *fieldName) {
    physical_scan *scan1 = scan->physicalScanProject->scan;
    /*TODO:判断是否包含在 scan->physicalScanProject->expr_list 中*/
    return scan1->hasField(scan1, tableName, fieldName);
};
arraylist *physical_scan_project_get_fields_name(physical_scan *scan, char *tableName){
    physical_scan *scan1  = scan->physicalScanProject->scan;
    return scan1->getFieldsName(scan1, tableName);
};

field_info *physical_scan_project_get_field(physical_scan *scan, char *tableName, char *fieldName){
    physical_scan *scan1 = scan->physicalScanProject->scan;
    return scan1->getField(scan1, tableName, fieldName);
};

int physical_scan_project_get_rid(physical_scan *scan, record_id *recordId) {};

int physical_scan_project_moveto_rid(physical_scan *scan, record_id *recordId) {};

/**
 * 处理select中形如student.*的字段列表表示
 * @param scan
 * @return
 */
int physical_scan_project_generate_expr_list(physical_scan *scan){
    arraylist *srcExprList = scan->physicalScanProject->original_expr_list;
    arraylist *dstExprList = scan->physicalScanProject->expr_list;
    for (int i = 0; i <= srcExprList->size - 1; i++){
        Expression *expr = arraylist_get(srcExprList, i);
        if (expr->term !=NULL && expr->term->t == TERM_COLREF){
            ColumnReference_t *columnReference = expr->term->ref;
            if (stricmp(columnReference->columnName, "*")==0){
                arraylist *names = scan->getFieldsName(scan, columnReference->tableName);
                for (int j = 0; j <= names->size - 1; j ++){
                    char *name = arraylist_get(names, j);
                    ColumnReference_t *ref = column_get_reference(name);
                    Expression *expr0 = newExpression(TOKEN_WORD, NULL);
                    TermExpr *term = newTermExpr();
                    term->t = TERM_COLREF;
                    term->ref = ref;
                    expr0->term = term;
                    arraylist_add(dstExprList, expr0);
                }

            }else{
                arraylist_add(dstExprList, expr);
            }
        }else{
            arraylist_add(dstExprList, expr);
        }
    }
};