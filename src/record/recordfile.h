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
 * 描述数据表的结构信息
 */
typedef struct table_info_ {
    field_info *fieldInfo[0];
    int *offsets[0];
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


int record_file_new(record_file *recordFile, table_info *tableInfo,
                    transaction *tx) ;

int record_file_close(record_file *recordFile) ;

int record_file_append_block(record_file *recordFile) ;

int record_page_close(record_page *recordPage) ;

#endif //DONGMENDB_RECORDFILE_H
