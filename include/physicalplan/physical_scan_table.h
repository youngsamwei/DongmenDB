//
// Created by Sam on 2018/2/7.
//

#ifndef DONGMENDB_PHYSICAL_SCAN_TABLE_H
#define DONGMENDB_PHYSICAL_SCAN_TABLE_H

#include "dongmendb/recordfile.h"
#include "physicalplan/physicalscan.h"
#ifdef __cplusplus
extern "C" {
#endif

/*
 * 实现全表扫描操作
 * */
typedef struct variant_ variant;
typedef struct physical_scan_ physical_scan;

typedef struct physical_scan_table_ {
    dongmendb *db;
    record_file *recordFile;
    table_info *tableInfo;
    transaction *tx;
} physical_scan_table;

physical_scan *physical_scan_table_create(dongmendb *db, char *tableName, transaction *tx);

void physical_scan_table_init_scan(physical_scan *scan);

int physical_scan_table_before_first(physical_scan *scan);

int physical_scan_table_next(physical_scan *scan);

int physical_scan_table_close(physical_scan *scan);

variant *physical_scan_table_get_val(physical_scan *scan, char *tableName, char *fieldName);

variant *physical_scan_table_get_val_by_index(physical_scan *scan, int index);

int physical_scan_table_get_int_by_index(physical_scan *scan, int index);

int physical_scan_table_get_string_by_index(physical_scan *scan, int index, char *value);

int physical_scan_table_get_int(physical_scan *scan, char *tableName, char *fieldName);

int physical_scan_table_get_string(physical_scan *scan, char *tableName, char *fieldName, char *value);

int physical_scan_table_has_field(physical_scan *scan, char *tableName, char *fieldName);

field_info *physical_scan_table_get_field(physical_scan *scan, char *tableName, char *fieldName);

arraylist *physical_scan_table_get_fields_name(physical_scan *scan, char *tableName);

int physical_scan_table_set_int(physical_scan *scan, char *tableName, char *fieldName, int value);

int physical_scan_table_set_string(physical_scan *scan, char *tableName, char *fieldName, char *value);

int physical_scan_table_delete(physical_scan *scan);

int physical_scan_table_insert(physical_scan *scan);

int physical_scan_table_get_rid(physical_scan *scan, record_id *recordId);

int physical_scan_table_moveto_rid(physical_scan *scan, record_id *recordId);
#ifdef __cplusplus
}
#endif

#endif //DONGMENDB_PHYSICAL_SCAN_TABLE_H
