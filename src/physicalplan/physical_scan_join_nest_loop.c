//
// Created by Sam on 2018/2/7.
//

#include "physicalplan/physical_scan_join_nest_loop.h"

/* 物理计划：连接实现：使用嵌套循环算法*/
physical_scan *physical_scan_join_nest_loop_create(physical_scan *scan1, physical_scan *scan2) {
    physical_scan_join_nest_loop *physicalScanJoinNestLoop = (physical_scan_join_nest_loop *)calloc(sizeof(physical_scan_join_nest_loop),1);
    physicalScanJoinNestLoop->scan1 = scan1;
    physicalScanJoinNestLoop->scan2 = scan2;

    physical_scan *physicalScan = (physical_scan *)calloc(sizeof(physical_scan),1);
    physicalScan->scanType = SCAN_TYPE_JOIN_NEST_LOOP;
    physicalScan->physicalScanJoinNestLoop = physicalScanJoinNestLoop;
    physical_scan_join_nest_loop_init_scan(physicalScan);
    return physicalScan;
};

void physical_scan_join_nest_loop_init_scan(physical_scan *scan){
    scan->beforeFirst = physical_scan_join_nest_loop_before_first;
    scan->next = physical_scan_join_nest_loop_next;
    scan->close  = physical_scan_join_nest_loop_close;
    scan->getValByIndex = NULL;
    scan->getIntByIndex = NULL;
    scan->getStringByIndex = NULL;
    scan->getVal = physical_scan_join_nest_loop_get_val;
    scan->getInt = physical_scan_join_nest_loop_get_int;
    scan->getString = physical_scan_join_nest_loop_get_string;
    scan->getField = physical_scan_join_nest_loop_get_field;
    scan->hasField = physical_scan_join_nest_loop_has_field;
    scan->getFieldsName = physical_scan_join_nest_loop_get_fields_name;
    scan->setInt = NULL;
    scan->setString = NULL;
    scan->deleterec = NULL;
    scan->insert = NULL;
    scan->getRid = physical_scan_join_nest_loop_get_rid;
    scan->movetoRid = physical_scan_join_nest_loop_moveto_rid;
}

int physical_scan_join_nest_loop_before_first(physical_scan *scan){
    physical_scan *scan1  = scan->physicalScanJoinNestLoop->scan1;
    physical_scan *scan2  = scan->physicalScanJoinNestLoop->scan2;
    scan1->beforeFirst(scan1);
    scan1->next(scan1);
    scan2->beforeFirst(scan2);
    return 1;
};

int physical_scan_join_nest_loop_next(physical_scan *scan){
    physical_scan *scan1  = scan->physicalScanJoinNestLoop->scan1;
    physical_scan *scan2  = scan->physicalScanJoinNestLoop->scan2;
    if (scan2->next(scan2)){
        return 1;
    }else{
        scan2->beforeFirst(scan2);
        return scan2->next(scan2) && scan1->next(scan1);
    }
};

int physical_scan_join_nest_loop_close(physical_scan *scan){
    physical_scan *scan1  = scan->physicalScanJoinNestLoop->scan1;
    physical_scan *scan2  = scan->physicalScanJoinNestLoop->scan2;
    scan1->close(scan1);
    scan2->close(scan2);
};

variant *physical_scan_join_nest_loop_get_val(physical_scan *scan, char *tableName, char *fieldName){
    physical_scan *scan1  = scan->physicalScanJoinNestLoop->scan1;
    physical_scan *scan2  = scan->physicalScanJoinNestLoop->scan2;
    if (scan1->hasField(scan1, tableName, fieldName)){
        return scan1->getVal(scan1, tableName, fieldName);
    }else{
        return scan2->getVal(scan2, tableName, fieldName);
    };
};

int physical_scan_join_nest_loop_get_int(physical_scan *scan, char *tableName, char *fieldName){
    physical_scan *scan1  = scan->physicalScanJoinNestLoop->scan1;
    physical_scan *scan2  = scan->physicalScanJoinNestLoop->scan2;
    if (scan1->hasField(scan1, tableName, fieldName)){
        return scan1->getInt(scan1, tableName, fieldName);
    }else{
        return scan2->getInt(scan2, tableName, fieldName);
    };
};

int physical_scan_join_nest_loop_get_string(physical_scan *scan, char *tableName, char *fieldName, char *value){
    physical_scan *scan1  = scan->physicalScanJoinNestLoop->scan1;
    physical_scan *scan2  = scan->physicalScanJoinNestLoop->scan2;
    if (scan1->hasField(scan1, tableName, fieldName)){
        return scan1->getString(scan1, tableName, fieldName, value);
    }else{
        return scan2->getString(scan2, tableName, fieldName, value);
    };
};

int physical_scan_join_nest_loop_has_field(physical_scan *scan, char *tableName, char *fieldName){
    physical_scan *scan1  = scan->physicalScanJoinNestLoop->scan1;
    physical_scan *scan2  = scan->physicalScanJoinNestLoop->scan2;
    return (scan1->hasField(scan1, tableName, fieldName)) || (scan2->hasField(scan2, tableName, fieldName));
};

field_info *physical_scan_join_nest_loop_get_field(physical_scan *scan, char *tableName, char *fieldName){
    physical_scan *scan1  = scan->physicalScanJoinNestLoop->scan1;
    physical_scan *scan2  = scan->physicalScanJoinNestLoop->scan2;
    field_info *fi = scan1->getField(scan1, tableName, fieldName);
    if (fi){
        return fi;
    }else{
        return scan2->getField(scan2, tableName, fieldName);
    }
};

arraylist *physical_scan_join_nest_loop_get_fields_name(physical_scan *scan, char *tableName){
    physical_scan *scan1  = scan->physicalScanJoinNestLoop->scan1;
    physical_scan *scan2  = scan->physicalScanJoinNestLoop->scan2;
    arraylist *scan1flds = scan1->getFieldsName(scan1, tableName);
    arraylist *scan2flds = scan2->getFieldsName(scan2, tableName);
    arraylist *all = arraylist_create();
    arraylist_add_all(all, scan1flds);
    arraylist_add_all(all, scan2flds);
    return all;
};

int physical_scan_join_nest_loop_get_rid(physical_scan *scan, record_id *recordId){};

int physical_scan_join_nest_loop_moveto_rid(physical_scan *scan, record_id *recordId){};