//
// Created by Sam on 2018/2/7.
//

#ifndef DONGMENDB_PHYSICALSCAN_H
#define DONGMENDB_PHYSICALSCAN_H

#include "dongmendb/recordfile.h"
#include <dongmensql/sra.h>
#include "physical_scan_table.h"
#include "physical_scan_join_nest_loop.h"
#include "physical_scan_select.h"
#include "physical_scan_project.h"
#ifdef __cplusplus
extern "C" {
#endif

/**
 * 定义物理计划的数据结构和接口函数.
 */
typedef struct variant_ {
    enum data_type type;
    union {
        int intValue;
        char *strValue;
        int booleanValue;
    };
} variant;

typedef enum {
    SCAN_TYPE_JOIN_NEST_LOOP,  //使用嵌套循环实现连接
    SCAN_TYPE_TABLE,
    SCAN_TYPE_SELECT,
    SCAN_TYPE_PROJECT
} scan_type;

typedef struct physical_scan_table_ physical_scan_table;
typedef struct physical_scan_ physical_scan;
typedef struct physical_scan_project_ physical_scan_project;
typedef struct physical_scan_select_ physical_scan_select;
typedef struct physical_scan_join_nest_loop_ physical_scan_join_nest_loop;

typedef int (*physical_scan_before_first)(physical_scan *scan);

typedef int (*physical_scan_next)(physical_scan *scan);

typedef int (*physical_scan_close)(physical_scan *scan);

typedef variant *(*physical_scan_get_val_by_index)(physical_scan *scan, int index);

typedef int (*physical_scan_get_int_by_index)(physical_scan *scan, int index);

typedef int (*physical_scan_get_string_by_index)(physical_scan *scan, int index, char *value);

typedef variant *( *physical_scan_get_val)(physical_scan *scan, char *tableName, char *fieldName);

typedef int (*physical_scan_get_int)(physical_scan *scan, char *tableName, char *fieldName);

typedef int (*physical_scan_get_string)(physical_scan *scan, char *tableName, char *fieldName, char *value);

typedef int (*physical_scan_has_field)(physical_scan *scan, char *tableName, char *fieldName);

typedef field_info *(*physical_scan_get_field)(physical_scan *scan, char *tableName, char *fieldName);

/*获得指定表中字段名称，若tableName为NULL，则获得全部*/
typedef arraylist *(*physical_scan_get_fields_name)(physical_scan *scan, char *tableName);

typedef int (*physical_scan_set_int)(physical_scan *scan, char *tableName, char *fieldName, int value);

typedef int (*physical_scan_set_string)(physical_scan *scan, char *tableName, char *fieldName, char *value);

typedef int (*physical_scan_delete)(physical_scan *scan);

typedef int (*physical_scan_insert)(physical_scan *scan);

typedef int (*physical_scan_get_rid)(physical_scan *scan, record_id *recordId);

typedef int (*physical_scan_moveto_rid)(physical_scan *scan, record_id *recordId);

typedef struct physical_scan_ {
    scan_type scanType;
    union {
        physical_scan_table *physicalScanTable;
        physical_scan_join_nest_loop *physicalScanJoinNestLoop;
        physical_scan_select *physicalScanSelect;
        physical_scan_project *physicalScanProject;
    };
    physical_scan_before_first beforeFirst;
    physical_scan_next next;
    physical_scan_close close;
    physical_scan_get_val_by_index getValByIndex;
    physical_scan_get_int_by_index getIntByIndex;
    physical_scan_get_string_by_index getStringByIndex;
    physical_scan_get_val getVal;
    physical_scan_get_int getInt;
    physical_scan_get_string getString;
    physical_scan_has_field hasField;
    physical_scan_get_field getField;
    physical_scan_get_fields_name getFieldsName;
    physical_scan_set_int setInt;
    physical_scan_set_string setString;
    physical_scan_delete deleterec;
    physical_scan_insert insert;
    physical_scan_get_rid getRid;
    physical_scan_moveto_rid movetoRid;
} physical_scan;

physical_scan *physical_scan_generate(dongmendb *db, SRA_t *sra, transaction *tx);

Expression *physical_scan_evaluate_expression(Expression *expr, physical_scan *scan, variant *var);
#ifdef __cplusplus
}
#endif

#endif //DONGMENDB_PHYSICALSCAN_H
