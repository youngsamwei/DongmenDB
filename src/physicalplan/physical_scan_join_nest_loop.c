//
// Created by Sam on 2018/2/7.
//

#include "physical_scan_join_nest_loop.h"

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
    scan->getInt = physical_scan_join_nest_loop_get_int;
    scan->getString = physical_scan_join_nest_loop_get_string;
    scan->hasField = physical_scan_join_nest_loop_has_field;
    scan->setInt = physical_scan_join_nest_loop_set_int;
    scan->setString = physical_scan_join_nest_loop_set_string;
    scan->delete  = physical_scan_join_nest_loop_delete;
    scan->insert = physical_scan_join_nest_loop_insert;
    scan->getRid = physical_scan_join_nest_loop_get_rid;
    scan->movetoRid = physical_scan_join_nest_loop_moveto_rid;
}

int physical_scan_join_nest_loop_before_first(physical_scan *tableScan){};

int physical_scan_join_nest_loop_next(physical_scan *tableScan){};

int physical_scan_join_nest_loop_close(physical_scan *tableScan){};

int physical_scan_join_nest_loop_get_int(physical_scan *tableScan, char *fieldName){};

int physical_scan_join_nest_loop_get_string(physical_scan *tableScan, char *fieldName, char *value){};

int physical_scan_join_nest_loop_has_field(physical_scan *tableScan, char *fieldName){};

int physical_scan_join_nest_loop_set_int(physical_scan *tableScan, char *fieldName, int value){};

int physical_scan_join_nest_loop_set_string(physical_scan *tableScan, char *fieldName, char *value){};

int physical_scan_join_nest_loop_delete(physical_scan *tableScan){};

int physical_scan_join_nest_loop_insert(physical_scan *tableScan){};

int physical_scan_join_nest_loop_get_rid(physical_scan *tableScan, record_id *recordId){};

int physical_scan_join_nest_loop_moveto_rid(physical_scan *tableScan, record_id *recordId){};