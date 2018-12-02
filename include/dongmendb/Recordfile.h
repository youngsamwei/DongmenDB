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
#include "dongmendb/Transaction.h"
#include <dongmensql/common.h>
#include "dongmendb/FileManager.h"

#include <vector>
#include <map>

using namespace std;

#define RECORD_FILE_EXT ".tbl"

class Transaction;
class RecordPage;
typedef struct record_id_ record_id;

class RecordFile {
public:
    TableInfo *tableInfo;
    Transaction *tx;
    char *fileName;
    RecordPage *recordPage;
    int currentBlkNum;


    RecordFile(TableInfo *tableInfo,
                           Transaction *tx);

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

    int record_file_record_formatter(MemoryPage *memoryPage);

} ;

typedef enum {
    RECORD_PAGE_EMPTY = 0,
    RECORD_PAGE_INUSE
} record_page_status;

class FieldInfo {
public:
    char *fieldName;
    unsigned int hashCode;
    enum data_type type;
    int length;

    FieldInfo(enum data_type type, int length, char* fieldName);

} ;

typedef struct record_value_{
    vector<FieldInfo *> *fieldsInfo; //field_info list
    vector<variant*> *value;      //variant list
}record_value;


/**
 * 描述数据表的结构信息
 */
class TableInfo {
public:
    vector<char*> fieldsName;
    map<string, FieldInfo *> *fields;
    map< unsigned int, int> *offsets;
    int recordLen;
    char *tableName;


    TableInfo(const char *tableName,  vector<char*> fieldsName,  map<string, FieldInfo *>  *fields);

    int table_info_free();

    int table_info_offset(const char *fieldName);

} ;

typedef struct record_id_ {
    int blockNum;
    int id;
} record_id;



/**
 * 管理一个block中的记录。
 */
class  RecordPage {
public:
    DiskBlock *diskBlock;
    TableInfo *tableInfo;
    Transaction *tx;
    int slotSize;
    int currentSlot;

    RecordPage(Transaction *tx, TableInfo *tableInfo, DiskBlock *diskBlock);

    int record_page_close();

    int record_page_insert();

    int record_page_moveto_id(int id);

    int record_page_next();

    int record_page_getint(const  char *fieldName);

    int record_page_getstring(const char *fieldName, char *value);

    int record_page_setint(const char *fieldName, int value);

    int record_page_setstring(const  char *fieldName, const char *value);

    int record_page_delete();

    int record_page_searchfor(record_page_status status);

    int record_page_current_pos();

    int record_page_fieldpos(const  char *fieldName);

    int record_page_is_valid_slot();

} ;



#endif //DONGMENDB_RECORDFILE_H
