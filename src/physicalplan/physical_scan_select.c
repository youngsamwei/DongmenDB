//
// Created by Sam on 2018/2/7.
//

#include "physicalplan/physical_scan_select.h"

/* 物理计划：选择的实现 */
physical_scan *physical_scan_select_create(physical_scan *scan) {
    physical_scan_select *physicalScanSelect = (physical_scan_select *)calloc(sizeof(physical_scan_select),1);
    physicalScanSelect->scan = scan;

    physical_scan *physicalScan = (physical_scan *)calloc(sizeof(physical_scan),1);
    physicalScan->scanType = SCAN_TYPE_SELECT;
    physicalScan->physicalScanSelect = physicalScanSelect;
    physical_scan_select_init_scan(physicalScan);
    return physicalScan;
};

void physical_scan_select_init_scan(physical_scan *scan){
    scan->beforeFirst = physical_scan_select_before_first;
    scan->next = physical_scan_select_next;
    scan->close  = physical_scan_select_close;
    scan->getValByIndex = physical_scan_select_get_val_by_index;
    scan->getIntByIndex = physical_scan_select_get_int_by_index;
    scan->getStringByIndex = physical_scan_select_get_string_by_index;
    scan->getVal = physical_scan_select_get_val;
    scan->getInt = physical_scan_select_get_int;
    scan->getString = physical_scan_select_get_string;
    scan->getField = physical_scan_select_get_field;
    scan->hasField = physical_scan_select_has_field;
    scan->getFieldsName = physical_scan_select_get_fields_name;
    scan->setInt = physical_scan_select_set_int;
    scan->setString = physical_scan_select_set_string;
    scan->deleterec  = physical_scan_select_delete;
    scan->insert = physical_scan_select_insert;
    scan->getRid = physical_scan_select_get_rid;
    scan->movetoRid = physical_scan_select_moveto_rid;

}

int physical_scan_select_before_first(physical_scan *scan){
    physical_scan *scan1  = scan->physicalScanSelect->scan;
    return scan1->beforeFirst(scan1);
};

int physical_scan_select_next(physical_scan *scan){
    physical_scan *scan1  = scan->physicalScanSelect->scan;
    Expression *expr = scan->physicalScanSelect->cond;
    while (scan1->next(scan1)){
        variant *var = (variant *)calloc(sizeof(variant),1);
        physical_scan_evaluate_expression(expr, scan1, var);
        if (var->type == DATA_TYPE_BOOLEAN && var->booleanValue){
            return 1;
        }
    }

    return 0;
};

int physical_scan_select_close(physical_scan *scan){
    physical_scan *scan1  = scan->physicalScanSelect->scan;
    return scan1->close(scan1);
};

variant *physical_scan_select_get_val(physical_scan *scan, char *tableName, char *fieldName){
    physical_scan *scan1 = scan->physicalScanProject->scan;
    return scan1->getVal(scan1, tableName, fieldName);
};

variant *physical_scan_select_get_val_by_index(physical_scan *scan, int index){
    physical_scan *scan1 = scan->physicalScanProject->scan;
    return scan1->getValByIndex(scan1, index);
};

int physical_scan_select_get_int_by_index(physical_scan *scan, int index){
    physical_scan *scan1 = scan->physicalScanProject->scan;
    return scan1->getIntByIndex(scan1, index);
};

int physical_scan_select_get_string_by_index(physical_scan *scan, int index, char *value){
    physical_scan *scan1 = scan->physicalScanProject->scan;
    scan1->getStringByIndex(scan1, index, value);
    return 1;
};

int physical_scan_select_get_int(physical_scan *scan, char *tableName, char *fieldName){
    physical_scan *scan1  = scan->physicalScanSelect->scan;
    return scan1->getInt(scan1, tableName, fieldName);
};

int physical_scan_select_get_string(physical_scan *scan, char *tableName, char *fieldName, char *value){
    physical_scan *scan1  = scan->physicalScanSelect->scan;
    return scan1->getString(scan1, tableName, fieldName, value);
};

int physical_scan_select_has_field(physical_scan *scan, char *tableName, char *fieldName){
    physical_scan *scan1  = scan->physicalScanSelect->scan;
    return scan1->hasField(scan1, tableName, fieldName);
};

field_info *physical_scan_select_get_field(physical_scan *scan, char *tableName, char *fieldName){
    physical_scan *scan1  = scan->physicalScanSelect->scan;
    return scan1->getField(scan1, tableName, fieldName);
};
arraylist *physical_scan_select_get_fields_name(physical_scan *scan, char *tableName){
    physical_scan *scan1  = scan->physicalScanSelect->scan;
    return scan1->getFieldsName(scan1, tableName);
};

int physical_scan_select_set_int(physical_scan *scan, char *tableName, char *fieldName, int value){
    physical_scan *scan1  = scan->physicalScanSelect->scan;
    return scan1->setInt(scan1, tableName, fieldName, value);
};

int physical_scan_select_set_string(physical_scan *scan, char *tableName, char *fieldName, char *value){
    physical_scan *scan1  = scan->physicalScanSelect->scan;
    return scan1->setString(scan1, tableName, fieldName, value);
};

int physical_scan_select_delete(physical_scan *scan){
    physical_scan *scan1  = scan->physicalScanSelect->scan;
    return scan1->deleterec(scan1);
};

int physical_scan_select_insert(physical_scan *scan){};

int physical_scan_select_get_rid(physical_scan *scan, record_id *recordId){};

int physical_scan_select_moveto_rid(physical_scan *scan, record_id *recordId){};