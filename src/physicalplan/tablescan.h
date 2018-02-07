//
// Created by Sam on 2018/2/2.
//

#ifndef DONGMENDB_TABLESCAN_H
#define DONGMENDB_TABLESCAN_H

#include <recordfile.h>

typedef struct table_scan_ {
    dongmengdb *db;
    record_file *recordFile;
    table_info *tableInfo;
    transaction *tx;
} table_scan;

table_scan *table_scan_create(dongmengdb *db, char *tableName, transaction *tx);

int table_scan_before_first(table_scan *tableScan);

int table_scan_next(table_scan *tableScan);

int table_scan_close(table_scan *tableScan);

int table_scan_get_val(table_scan *tableScan, char *fieldName);

int table_scan_get_int(table_scan *tableScan, char *fieldName);

int table_scan_get_string(table_scan *tableScan, char *fieldName, char *value);

int table_scan_has_field(table_scan *tableScan, char *fieldName);

int table_scan_set_int(table_scan *tableScan, char *fieldName, int value);

int table_scan_set_string(table_scan *tableScan, char *fieldName, char *value);

int table_scan_delete(table_scan *tableScan);

int table_scan_insert(table_scan *tableScan);

int table_scan_get_rid(table_scan *tableScan, record_id *recordId);

int table_scan_moveto_rid(table_scan *tableScan, record_id *recordId);


#endif //DONGMENDB_TABLESCAN_H
