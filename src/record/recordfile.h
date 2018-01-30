//
// Created by Sam on 2018/1/26.
//

#ifndef DONGMENDB_RECORDFILE_H
#define DONGMENDB_RECORDFILE_H

#include <filemanager.h>
#include <malloc.h>
#include <mem.h>
#include <transaction.h>

#define RECORD_FILE_EXT ".tbl"

typedef struct table_info_ table_info;

typedef struct record_page_ record_page;
typedef struct transaction_ transaction;
/**
 *
 */
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

typedef enum {
    DATA_TYPE_INT = 0,
    DATA_TYPE_CHAR
} DATA_TYPE;

typedef struct field_info_ {
    DATA_TYPE type;
    int length;
} field_info;

/**
 * 用于保存在hashmap中的key和value
 */
typedef struct integer_{
    int val;
} integer;

/**
 * 描述数据表的结构信息
 */
typedef struct table_info_ {
    array_list fieldsName;
    hmap_t fields;
    hmap_t offsets;
    int recordLen;
    char *tableName;
} table_info;

/**
 * 管理一个bolck中的记录。
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

int record_file_moveto(record_file *recordFile, int blockNum);

int record_file_atlast_block(record_file *recordFile);

int record_file_append_block(record_file *recordFile);

int record_file_record_formatter(record_file *recordFile, memory_page *memoryPage);

int field_info_create(field_info *fieldInfo, DATA_TYPE type, int lenght);

int table_info_create(table_info *tableInfo, char *tableName, array_list fieldsName, hmap_t fields);

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
#endif //DONGMENDB_RECORDFILE_H
