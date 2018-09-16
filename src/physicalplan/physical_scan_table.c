//
// Created by Sam on 2018/2/7.
//

#include "physicalplan/physical_scan_table.h"

/*物理计划：表扫描*/
physical_scan *physical_scan_table_create(dongmendb *db, char *tableName, transaction *tx) {
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
    scan->getValByIndex = physical_scan_table_get_val_by_index;
    scan->getIntByIndex = physical_scan_table_get_int_by_index;
    scan->getStringByIndex = physical_scan_table_get_string_by_index;
    scan->getVal = physical_scan_table_get_val;
    scan->getInt = physical_scan_table_get_int;
    scan->getString = physical_scan_table_get_string;
    scan->hasField = physical_scan_table_has_field;
    scan->getField = physical_scan_table_get_field;
    scan->getFieldsName = physical_scan_table_get_fields_name;
    scan->setInt = physical_scan_table_set_int;
    scan->setString = physical_scan_table_set_string;
    scan->deleterec  = physical_scan_table_delete;
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

variant *physical_scan_table_get_val(physical_scan *scan, char *tableName, char *fieldName){};

variant *physical_scan_table_get_val_by_index(physical_scan *scan, int index){};

int physical_scan_table_get_int_by_index(physical_scan *scan, int index){};

int physical_scan_table_get_string_by_index(physical_scan *scan, int index, char *value){};

int physical_scan_table_get_int(physical_scan *scan, char *tableName, char *fieldName) {
    if (tableName && stricmp(scan->physicalScanTable->tableInfo->tableName, tableName) != 0){
        return 0;
    }
    return record_file_get_int(scan->physicalScanTable->recordFile, fieldName);
};

int physical_scan_table_get_string(physical_scan *scan, char *tableName, char *fieldName, char *value) {
    if (tableName && stricmp(scan->physicalScanTable->tableInfo->tableName, tableName) != 0){
        value = NULL;
        return 0;
    }
    return record_file_get_string(scan->physicalScanTable->recordFile, fieldName, value);
};

int physical_scan_table_has_field(physical_scan *scan, char *tableName, char *fieldName) {
    if (tableName && stricmp(scan->physicalScanTable->tableInfo->tableName, tableName) != 0){
        return 0;
    }
    if (physical_scan_table_get_field(scan, tableName, fieldName)){
        return 1;
    }else{
        return 0;
    }
};

field_info *physical_scan_table_get_field(physical_scan *scan, char *tableName, char *fieldName){
    if (tableName && stricmp(scan->physicalScanTable->tableInfo->tableName, tableName) != 0){
        return NULL;
    }
    hmap_t fields = scan->physicalScanTable->tableInfo->fields;
    void_ptr *fiptr = (void_ptr *)calloc(sizeof(void_ptr), 1);
    hashmap_get(fields, fieldName, fiptr);
    if (*fiptr){
        field_info *fi = *fiptr;
        return fi;
    }else{
        return NULL;
    }
};

arraylist *physical_scan_table_get_fields_name(physical_scan *scan, char *tableName){

    if (tableName == NULL || stricmp(tableName, scan->physicalScanTable->tableInfo->tableName) == 0){
        return scan->physicalScanTable->tableInfo->fieldsName;
    }
    /*如果不是指定的tableName，则返回空的arraylist*/
    return arraylist_create();
};

int physical_scan_table_set_int(physical_scan *scan, char *tableName, char *fieldName, int value) {
    return record_file_set_int(scan->physicalScanTable->recordFile, fieldName, value);
};

int physical_scan_table_set_string(physical_scan *scan, char *tableName, char *fieldName, char *value) {
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
