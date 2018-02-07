//
// Created by Sam on 2018/2/7.
//

#ifndef DONGMENDB_PHYSICAL_SCAN_TABLE_H
#define DONGMENDB_PHYSICAL_SCAN_TABLE_H

#include <recordfile.h>
#include "physicalscan.h"

typedef struct physical_scan_ physical_scan;

typedef struct physical_scan_table_{
    dongmengdb *db;
    record_file *recordFile;
    table_info *tableInfo;
    transaction *tx;
}physical_scan_table;

physical_scan *physical_scan_table_create(dongmengdb *db, char *tableName, transaction *tx);

void physical_scan_table_init_scan(physical_scan *scan);

int physical_scan_table_before_first(physical_scan *scan);

int physical_scan_table_next(physical_scan *scan);

int physical_scan_table_close(physical_scan *scan);

int physical_scan_table_get_val(physical_scan *scan, char *fieldName);

int physical_scan_table_get_int(physical_scan *scan, char *fieldName);

int physical_scan_table_get_string(physical_scan *scan, char *fieldName, char *value);

int physical_scan_table_has_field(physical_scan *scan, char *fieldName);

int physical_scan_table_set_int(physical_scan *scan, char *fieldName, int value);

int physical_scan_table_set_string(physical_scan *scan, char *fieldName, char *value);

int physical_scan_table_delete(physical_scan *scan);

int physical_scan_table_insert(physical_scan *scan);

int physical_scan_table_get_rid(physical_scan *scan, record_id *recordId);

int physical_scan_table_moveto_rid(physical_scan *scan, record_id *recordId);

#endif //DONGMENDB_PHYSICAL_SCAN_TABLE_H
