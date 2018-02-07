//
// Created by Sam on 2018/2/2.
//

#include "tablescan.h"

table_scan *table_scan_create(dongmengdb *db, char *tableName,  transaction *tx) {
    table_scan *tableScan = (table_scan *) calloc(sizeof(table_scan), 1);
    tableScan->db = db;
    tableScan->tableInfo = table_manager_get_tableinfo(db->metadataManager->tableManager, tableName, tx);
    tableScan->recordFile = (record_file *) malloc(sizeof(record_file));
    record_file_create(tableScan->recordFile, tableScan->tableInfo, tx);
    return tableScan;
};

int table_scan_before_first(table_scan *tableScan) {
    return record_file_before_first(tableScan->recordFile);
};

int table_scan_next(table_scan *tableScan) {
    return record_file_next(tableScan->recordFile);
};

int table_scan_close(table_scan *tableScan) {
    return record_file_close(tableScan->recordFile);
};

int table_scan_get_val(table_scan *tableScan, char *fieldName) {

};

int table_scan_get_int(table_scan *tableScan, char *fieldName) {
    return record_file_get_int(tableScan->recordFile, fieldName);
};

int table_scan_get_string(table_scan *tableScan, char *fieldName, char *value) {
    return record_file_get_string(tableScan->recordFile, fieldName, value);
};

int table_scan_has_field(table_scan *tableScan, char *fieldName) {

};

int table_scan_set_int(table_scan *tableScan, char *fieldName, int value) {
    return record_file_set_int(tableScan->recordFile, fieldName, value);
};

int table_scan_set_string(table_scan *tableScan, char *fieldName, char *value) {
    return record_file_set_string(tableScan->recordFile, fieldName, value);
};

int table_scan_delete(table_scan *tableScan) {
    return record_file_delete(tableScan->recordFile);
};

int table_scan_insert(table_scan *tableScan) {
    return record_file_insert(tableScan->recordFile);
};

int table_scan_get_rid(table_scan *tableScan, record_id *recordId) {
    return record_file_current_recordid(tableScan->recordFile, recordId);
};

int table_scan_moveto_rid(table_scan *tableScan, record_id *recordId) {
    return record_file_moveto_recordid(tableScan->recordFile, recordId);
};

