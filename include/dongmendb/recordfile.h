//
// Created by Sam on 2018/1/26.
//

#ifndef DONGMENDB_RECORDFILE_H
#define DONGMENDB_RECORDFILE_H

/**
 * 以记录为单位的数据管理操作.
 * recordpage可以取消？
 */


#include <malloc.h>
#include <mem.h>
#include "dongmendb/transaction.h"
#include <dongmensql/common.h>
#include "dongmendb/filemanager.h"
#include "utils/arraylist.h"

#ifdef __cplusplus
extern "C" {
#endif

#define RECORD_FILE_EXT ".tbl"

typedef struct table_info_ table_info;

typedef struct record_page_ record_page;
typedef struct transaction_ transaction;

typedef struct record_file_ {
    table_info *tableInfo;
    transaction *tx;
    char *fileName;
    record_page *recordPage;
    int currentBlkNum;
} record_file;

typedef enum {
    RECORD_PAGE_EMPTY = 0,
    RECORD_PAGE_INUSE
} record_page_status;

typedef struct field_info_ {
    char *fieldName;
    enum data_type type;
    int length;
} field_info;

typedef struct record_value_{
    arraylist *fieldsInfo; //field_info list
    arraylist *value;      //variant list
}record_value;
/**
 * 用于保存在hashmap中的key和value
 */
typedef struct integer_ {
    int val;
} integer;

/**
 * 描述数据表的结构信息
 */
typedef struct table_info_ {
    arraylist *fieldsName;
    hmap_t fields;
    hmap_t offsets;
    int recordLen;
    char *tableName;
} table_info;

/**
 * 管理一个block中的记录。
 */
typedef struct record_page_ {
    disk_block *diskBlock;
    table_info *tableInfo;
    transaction *tx;
    int slotSize;
    int currentSlot;
} record_page;

typedef struct record_id_ {
    int blockNum;
    int id;
} record_id;

int record_file_create(record_file *recordFile, table_info *tableInfo,
                       transaction *tx);

int record_file_close(record_file *recordFile);

int record_file_before_first(record_file *recordFile);

int record_file_next(record_file *recordFile);

int record_file_atlast(record_file *recordFile);

int record_file_get_int(record_file *recordFile, char *fieldName);

int record_file_get_string(record_file *recordFile, char *fieldName, char *value);

int record_file_set_int(record_file *recordFile, char *fieldName, int value);

int record_file_set_string(record_file *recordFile, char *fieldName, char *value);

int record_file_delete(record_file *recordFile);

int record_file_insert(record_file *recordFile);

int record_file_moveto_recordid(record_file *recordFile, record_id *recordId);

int record_file_current_recordid(record_file *recordFile, record_id *recordId);

int record_file_moveto(record_file *recordFile, int currentBlkNum);

int record_file_atlast_block(record_file *recordFile);

int record_file_append_block(record_file *recordFile);

int record_file_record_formatter(record_file *recordFile, memory_page *memoryPage);

field_info *field_info_create(enum data_type type, int length);

table_info *table_info_create(char *tableName, arraylist *fieldsName, hmap_t fields);

int table_info_free(table_info *tableInfo);

int table_info_offset(table_info *tableInfo, char *fieldName);

record_page *record_page_create(transaction *tx, table_info *tableInfo, disk_block *diskBlock);

int record_page_close(record_page *recordPage);

int record_page_insert(record_page *recordPage);

int record_page_moveto_id(record_page *recordPage, int id);

int record_page_next(record_page *recordPage);

int record_page_getint(record_page *recordPage, char *fieldName);

int record_page_getstring(record_page *recordPage, char *fieldName, char *value);

int record_page_setint(record_page *recordPage, char *fieldName, int value);

int record_page_setstring(record_page *recordPage, char *fieldName, char *value);

int record_page_delete(record_page *recordPage);

int record_page_searchfor(record_page *recordPage, record_page_status status);

int record_page_current_pos(record_page *recordPage);

int record_page_fieldpos(record_page *recordPage, char *fieldName);

int record_page_is_valid_slot(record_page *recordPage);


#ifdef __cplusplus
}
#endif

#endif //DONGMENDB_RECORDFILE_H
