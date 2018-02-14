//
// Created by Sam on 2018/2/7.
//

#ifndef DONGMENDB_PHYSICAL_SCAN_PROJECT_H
#define DONGMENDB_PHYSICAL_SCAN_PROJECT_H

#include <utils/arraylist.h>
#include "dongmendb/transaction.h"
#include "physicalscan.h"
#ifdef __cplusplus
extern "C" {
#endif

/**
 * 实现投影操作
 */
typedef struct physical_scan_project_ {
    arraylist *original_expr_list;//select语句中指定的原始表达式列表
    arraylist *expr_list; //将 * 等解析后得到的表达式列表，在prepare时处理
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

arraylist *physical_scan_project_get_fields_name(physical_scan *scan, char *tableName);

field_info *physical_scan_project_get_field(physical_scan *scan, char *tableName, char *fieldName);

int physical_scan_project_get_rid(physical_scan *scan, record_id *recordId);

int physical_scan_project_moveto_rid(physical_scan *scan, record_id *recordId);

int physical_scan_project_generate_expr_list(physical_scan *scan);
#ifdef __cplusplus
}
#endif

#endif //DONGMENDB_PHYSICAL_SCAN_PROJECT_H
