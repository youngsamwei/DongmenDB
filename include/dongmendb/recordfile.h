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

#include <vector>
#include <map>

using namespace std;

#define RECORD_FILE_EXT ".tbl"

typedef struct table_info_ table_info;
typedef struct record_id_ record_id;
typedef struct record_page_ record_page;
typedef struct transaction_ transaction;

class RecordFile {
public:
    table_info *tableInfo;
    transaction *tx;
    char *fileName;
    record_page *recordPage;
    int currentBlkNum;


    RecordFile(table_info *tableInfo,
                           transaction *tx);

    int record_file_close();

    int record_file_before_first();

    int record_file_next();

    int record_file_atlast();

    int record_file_get_int(const char *fieldName);

    int record_file_get_string(const char *fieldName, char *value);

    int record_file_set_int(const char *fieldName, int value);

    int record_file_set_string(const char *fieldName, const char *value);

    int record_file_delete();

    int record_file_insert();

    int record_file_moveto_recordid(record_id *recordId);

    int record_file_current_recordid(record_id *recordId);

    int record_file_moveto(int currentBlkNum);

    int record_file_atlast_block();

    int record_file_append_block();

    int record_file_record_formatter(memory_page *memoryPage);

} ;

typedef enum {
    RECORD_PAGE_EMPTY = 0,
    RECORD_PAGE_INUSE
} record_page_status;

typedef struct field_info_ {
    char *fieldName;
    unsigned int hashCode;
    enum data_type type;
    int length;
} field_info;

typedef struct record_value_{
    vector<field_info*> *fieldsInfo; //field_info list
    vector<variant*> *value;      //variant list
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
    vector<char*> fieldsName;
    map<string, field_info*> *fields;
    map< unsigned int, int> *offsets;
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

field_info *field_info_create(enum data_type type, int length, char* fieldName);

table_info *table_info_create(const char *tableName,  vector<char*> fieldsName,  map<string, field_info*>  *fields);

int table_info_free(table_info *tableInfo);

int table_info_offset(table_info *tableInfo, const char *fieldName);

record_page *record_page_create(transaction *tx, table_info *tableInfo, disk_block *diskBlock);

int record_page_close(record_page *recordPage);

int record_page_insert(record_page *recordPage);

int record_page_moveto_id(record_page *recordPage, int id);

int record_page_next(record_page *recordPage);

int record_page_getint(record_page *recordPage,const  char *fieldName);

int record_page_getstring(record_page *recordPage, const char *fieldName, char *value);

int record_page_setint(record_page *recordPage, const char *fieldName, int value);

int record_page_setstring(record_page *recordPage,const  char *fieldName, const char *value);

int record_page_delete(record_page *recordPage);

int record_page_searchfor(record_page *recordPage, record_page_status status);

int record_page_current_pos(record_page *recordPage);

int record_page_fieldpos(record_page *recordPage,const  char *fieldName);

int record_page_is_valid_slot(record_page *recordPage);




#endif //DONGMENDB_RECORDFILE_H
