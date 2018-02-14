//
// Created by Sam on 2018/2/7.
//

#ifndef DONGMENDB_PHYSICAL_SCAN_JOIN_NEST_LOOP_H
#define DONGMENDB_PHYSICAL_SCAN_JOIN_NEST_LOOP_H

#include "parser/expression.h"
#include "physicalscan.h"
#ifdef __cplusplus
extern "C" {
#endif

/**
 * 采用嵌套循环算法实现笛卡尔积操作
 */
typedef struct variant_ variant;
typedef struct physical_scan_ physical_scan;
typedef struct physical_scan_join_nest_loop_ {
    physical_scan *scan1, *scan2;
    Expression *cond;

} physical_scan_join_nest_loop;


physical_scan *physical_scan_join_nest_loop_create(physical_scan *scan1, physical_scan *scan2);

void physical_scan_join_nest_loop_init_scan(physical_scan *scan);

int physical_scan_join_nest_loop_before_first(physical_scan *tableScan);

int physical_scan_join_nest_loop_next(physical_scan *tableScan);

int physical_scan_join_nest_loop_close(physical_scan *tableScan);

variant *physical_scan_join_nest_loop_get_val(physical_scan *scan, char *tableName, char *fieldName);

int physical_scan_join_nest_loop_get_int(physical_scan *tableScan, char *tableName, char *fieldName);

int physical_scan_join_nest_loop_get_string(physical_scan *tableScan, char *tableName, char *fieldName, char *value);

int physical_scan_join_nest_loop_has_field(physical_scan *tableScan, char *tableName, char *fieldName);

field_info *physical_scan_join_nest_loop_get_field(physical_scan *scan, char *tableName, char *fieldName);

arraylist *physical_scan_join_nest_loop_get_fields_name(physical_scan *scan, char *tableName);

int physical_scan_join_nest_loop_get_rid(physical_scan *tableScan, record_id *recordId);

int physical_scan_join_nest_loop_moveto_rid(physical_scan *tableScan, record_id *recordId);
#ifdef __cplusplus
}
#endif

#endif //DONGMENDB_PHYSICAL_SCAN_JOIN_NEST_LOOP_H
