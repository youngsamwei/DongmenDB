//
// Created by Sam on 2018/2/7.
//

#include "physical_scan_select.h"

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
    scan->getInt = physical_scan_select_get_int;
    scan->getString = physical_scan_select_get_string;
    scan->hasField = physical_scan_select_has_field;
    scan->setInt = physical_scan_select_set_int;
    scan->setString = physical_scan_select_set_string;
    scan->delete  = physical_scan_select_delete;
    scan->insert = physical_scan_select_insert;
    scan->getRid = physical_scan_select_get_rid;
    scan->movetoRid = physical_scan_select_moveto_rid;
}

int physical_scan_select_before_first(physical_scan *tableScan){};

int physical_scan_select_next(physical_scan *tableScan){};

int physical_scan_select_close(physical_scan *tableScan){};

int physical_scan_select_get_int(physical_scan *tableScan, char *fieldName){};

int physical_scan_select_get_string(physical_scan *tableScan, char *fieldName, char *value){};

int physical_scan_select_has_field(physical_scan *tableScan, char *fieldName){};

int physical_scan_select_set_int(physical_scan *tableScan, char *fieldName, int value){};

int physical_scan_select_set_string(physical_scan *tableScan, char *fieldName, char *value){};

int physical_scan_select_delete(physical_scan *tableScan){};

int physical_scan_select_insert(physical_scan *tableScan){};

int physical_scan_select_get_rid(physical_scan *tableScan, record_id *recordId){};

int physical_scan_select_moveto_rid(physical_scan *tableScan, record_id *recordId){};