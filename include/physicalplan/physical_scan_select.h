//
// Created by Sam on 2018/2/7.
//

#ifndef DONGMENDB_PHYSICAL_SCAN_SELECT_H
#define DONGMENDB_PHYSICAL_SCAN_SELECT_H

#include <parser/expression.h>
#include "physicalscan.h"
#ifdef __cplusplus
extern "C" {
#endif

/**
 * 实现选择操作
 */
typedef struct physical_scan_ physical_scan;
typedef struct physical_scan_select_ {
    Expression *cond;
    physical_scan *scan;
} physical_scan_select;


physical_scan *physical_scan_select_create(physical_scan *scan);

void physical_scan_select_init_scan(physical_scan *scan);

int physical_scan_select_before_first(physical_scan *scan);

int physical_scan_select_next(physical_scan *scan);

int physical_scan_select_close(physical_scan *scan);
variant *physical_scan_select_get_val(physical_scan *scan, char *tableName, char *fieldName);

variant *physical_scan_select_get_val_by_index(physical_scan *scan, int index);

int physical_scan_select_get_int_by_index(physical_scan *scan, int index);

int physical_scan_select_get_string_by_index(physical_scan *scan, int index, char *value);

int physical_scan_select_get_int(physical_scan *scan, char *tableName, char *fieldName);

int physical_scan_select_get_string(physical_scan *scan, char *tableName, char *fieldName, char *value);

int physical_scan_select_has_field(physical_scan *scan, char *tableName, char *fieldName);

field_info *physical_scan_select_get_field(physical_scan *scan, char *tableName, char *fieldName);

arraylist *physical_scan_select_get_fields_name(physical_scan *scan, char *tableName);

int physical_scan_select_set_int(physical_scan *scan, char *tableName, char *fieldName, int value);

int physical_scan_select_set_string(physical_scan *scan, char *tableName, char *fieldName, char *value);

int physical_scan_select_delete(physical_scan *scan);

int physical_scan_select_insert(physical_scan *scan);

int physical_scan_select_get_rid(physical_scan *scan, record_id *recordId);

int physical_scan_select_moveto_rid(physical_scan *scan, record_id *recordId);
#ifdef __cplusplus
}
#endif

#endif //DONGMENDB_PHYSICAL_SCAN_SELECT_H
