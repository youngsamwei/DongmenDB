//
// Created by Sam on 2018/2/7.
//

#include "physical_scan_project.h"


physical_scan *physical_scan_project_create(physical_scan *scan) {
    physical_scan_project *physicalScanProject = (physical_scan_project *)calloc(sizeof(physical_scan_project),1);
    physicalScanProject->scan = scan;

    physical_scan *physicalScan = (physical_scan *)calloc(sizeof(physical_scan),1);
    physicalScan->scanType = SCAN_TYPE_PROJECT;
    physicalScan->physicalScanProject = physicalScanProject;
    physical_scan_project_init_scan(physicalScan);
    return physicalScan;
};

void physical_scan_project_init_scan(physical_scan *scan){
    scan->beforeFirst = physical_scan_project_before_first;
    scan->next = physical_scan_project_next;
    scan->close  = physical_scan_project_close;
    scan->getInt = physical_scan_project_get_int;
    scan->getString = physical_scan_project_get_string;
    scan->hasField = physical_scan_project_has_field;
    scan->setInt = physical_scan_project_set_int;
    scan->setString = physical_scan_project_set_string;
    scan->delete  = physical_scan_project_delete;
    scan->insert = physical_scan_project_insert;
    scan->getRid = physical_scan_project_get_rid;
    scan->movetoRid = physical_scan_project_moveto_rid;
}

int physical_scan_project_before_first(physical_scan *scan){};

int physical_scan_project_next(physical_scan *scan){};

int physical_scan_project_close(physical_scan *scan){};

int physical_scan_project_get_int(physical_scan *scan, char *fieldName){};

int physical_scan_project_get_string(physical_scan *scan, char *fieldName, char *value){};

int physical_scan_project_has_field(physical_scan *scan, char *fieldName){};

int physical_scan_project_set_int(physical_scan *scan, char *fieldName, int value){};

int physical_scan_project_set_string(physical_scan *scan, char *fieldName, char *value){};

int physical_scan_project_delete(physical_scan *scan){};

int physical_scan_project_insert(physical_scan *scan){};

int physical_scan_project_get_rid(physical_scan *scan, record_id *recordId){};

int physical_scan_project_moveto_rid(physical_scan *scan, record_id *recordId){};

