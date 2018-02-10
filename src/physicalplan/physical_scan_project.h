//
// Created by Sam on 2018/2/7.
//

#ifndef DONGMENDB_PHYSICAL_SCAN_PROJECT_H
#define DONGMENDB_PHYSICAL_SCAN_PROJECT_H

#include <arraylist.h>
#include "../dongmengdb/transaction.h"
#include "physicalscan.h"

 /**
  * 实现投影操作
  */
typedef struct physical_scan_project_ {
    arraylist *expr_list; //expression列表
    arraylist *order_by; //order_by_expr的列表
    int distinct;
    arraylist *group_by; // expression列表

    physical_scan *scan;

} physical_scan_project;


physical_scan *physical_scan_project_create(physical_scan *scan);

void physical_scan_project_init_scan(physical_scan *scan);

int physical_scan_project_before_first(physical_scan *scan);

int physical_scan_project_next(physical_scan *scan);

int physical_scan_project_close(physical_scan *scan);

variant *physical_scan_project_get_val(physical_scan *scan, char *tableName, char *fieldName);

variant *physical_scan_project_get_val_by_index(physical_scan *scan, int index);

int physical_scan_project_get_int_by_index(physical_scan *scan, int index);

int physical_scan_project_get_string_by_index(physical_scan *scan, int index, char *value);

int physical_scan_project_get_int(physical_scan *scan, char *tableName, char *fieldName);

int physical_scan_project_get_string(physical_scan *scan, char *tableName, char *fieldName, char *value);

int physical_scan_project_has_field(physical_scan *scan, char *tableName, char *fieldName);

field_info *physical_scan_project_get_field(physical_scan *scan, char *tableName, char *fieldName);

int physical_scan_project_set_int(physical_scan *scan, char *tableName, char *fieldName, int value);

int physical_scan_project_set_string(physical_scan *scan, char *tableName, char *fieldName, char *value);

int physical_scan_project_delete(physical_scan *scan);

int physical_scan_project_insert(physical_scan *scan);

int physical_scan_project_get_rid(physical_scan *scan, record_id *recordId);

int physical_scan_project_moveto_rid(physical_scan *scan, record_id *recordId);


#endif //DONGMENDB_PHYSICAL_SCAN_PROJECT_H
