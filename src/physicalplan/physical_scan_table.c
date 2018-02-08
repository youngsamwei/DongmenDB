//
// Created by Sam on 2018/2/7.
//

#include "physical_scan_table.h"

physical_scan *physical_scan_table_create(dongmengdb *db, char *tableName, transaction *tx) {
    physical_scan_table *physicalScanTable = (physical_scan_table *)calloc(sizeof(physical_scan_table),1);
    physicalScanTable->db = db;
    physicalScanTable->tableInfo = table_manager_get_tableinfo(db->metadataManager->tableManager, tableName, tx);
    physicalScanTable->tx = tx;
    physicalScanTable->recordFile = (record_file *) malloc(sizeof(record_file));
    record_file_create(physicalScanTable->recordFile, physicalScanTable->tableInfo, tx);

    physical_scan *physicalScan = (physical_scan *)calloc(sizeof(physical_scan),1);
    physicalScan->scanType = SCAN_TYPE_TABLE;
    physicalScan->physicalScanTable = physicalScanTable;
    physical_scan_table_init_scan(physicalScan);
    return physicalScan;
};

void physical_scan_table_init_scan(physical_scan *scan){
    scan->beforeFirst = physical_scan_table_before_first;
    scan->next = physical_scan_table_next;
    scan->close  = physical_scan_table_close;
    scan->getInt = physical_scan_table_get_int;
    scan->getString = physical_scan_table_get_string;
    scan->hasField = physical_scan_table_has_field;
    scan->getField = physical_scan_table_get_field;
    scan->setInt = physical_scan_table_set_int;
    scan->setString = physical_scan_table_set_string;
    scan->delete  = physical_scan_table_delete;
    scan->insert = physical_scan_table_insert;
    scan->getRid = physical_scan_table_get_rid;
    scan->movetoRid = physical_scan_table_moveto_rid;
}

int physical_scan_table_before_first(physical_scan *scan) {
    return record_file_before_first(scan->physicalScanTable->recordFile);
};

int physical_scan_table_next(physical_scan *scan) {
    return record_file_next(scan->physicalScanTable->recordFile);
};

int physical_scan_table_close(physical_scan *scan) {
    return record_file_close(scan->physicalScanTable->recordFile);
};

int physical_scan_table_get_int(physical_scan *scan, char *fieldName) {
    return record_file_get_int(scan->physicalScanTable->recordFile, fieldName);
};

int physical_scan_table_get_string(physical_scan *scan, char *fieldName, char *value) {
    return record_file_get_string(scan->physicalScanTable->recordFile, fieldName, value);
};

int physical_scan_table_has_field(physical_scan *scan, char *fieldName) {
    if (physical_scan_table_get_field(scan, fieldName)){
        return 1;
    }else{
        return 0;
    }
};

field_info *physical_scan_table_get_field(physical_scan *scan, char *fieldName){
    hmap_t fields = scan->physicalScanTable->tableInfo->fields;
    void_ptr *fiptr = (void_ptr *)calloc(sizeof(void_ptr), 1);
    hashmap_get(fields, fieldName, fiptr);
    if (fiptr){
        field_info *fi = *fiptr;
        return fi;
    }else{
        return NULL;
    }
};

int physical_scan_table_set_int(physical_scan *scan, char *fieldName, int value) {
    return record_file_set_int(scan->physicalScanTable->recordFile, fieldName, value);
};

int physical_scan_table_set_string(physical_scan *scan, char *fieldName, char *value) {
    return record_file_set_string(scan->physicalScanTable->recordFile, fieldName, value);
};

int physical_scan_table_delete(physical_scan *scan) {
    return record_file_delete(scan->physicalScanTable->recordFile);
};

int physical_scan_table_insert(physical_scan *scan) {
    return record_file_insert(scan->physicalScanTable->recordFile);
};

int physical_scan_table_get_rid(physical_scan *scan, record_id *recordId) {
    return record_file_current_recordid(scan->physicalScanTable->recordFile, recordId);
};

int physical_scan_table_moveto_rid(physical_scan *scan, record_id *recordId) {
    return record_file_moveto_recordid(scan->physicalScanTable->recordFile, recordId);
};
