//
// Created by Sam on 2018/1/26.
//

#include <utils/utils.h>
#include <iostream>
#include "dongmendb/MetadataManager.h"

 TableManager::TableManager(int isNew, Transaction *tx) {
    map<string, FieldInfo *> *tableDescfields = new map<string, FieldInfo *>();

    char* fn_tablename = "tablename";
    FieldInfo *tablename = new FieldInfo(DATA_TYPE_CHAR, MAX_ID_NAME_LENGTH, fn_tablename);
    tableDescfields->insert(pair<string, FieldInfo *>(fn_tablename, tablename));
    char* fn_reclength = "reclength";
    FieldInfo *reclength = new FieldInfo(DATA_TYPE_INT, INT_SIZE, fn_reclength);
    tableDescfields->insert(pair<string, FieldInfo *>(fn_reclength, reclength));

    vector<char*> tableMetaFieldsName;
    tableMetaFieldsName.push_back(fn_tablename);
    tableMetaFieldsName.push_back(fn_reclength);

    this->tcatInfo = new TableInfo("tablecat", tableMetaFieldsName, tableDescfields);

    map<string, FieldInfo *> *fieldDescfields  = new map<string, FieldInfo *>();

    FieldInfo *field_tablename = new FieldInfo(DATA_TYPE_CHAR, MAX_ID_NAME_LENGTH, "tablename");

    fieldDescfields->insert(pair<string, FieldInfo *>("tablename",field_tablename));

    FieldInfo *fieldname = new FieldInfo(DATA_TYPE_CHAR, MAX_ID_NAME_LENGTH, "fieldname");
    fieldDescfields->insert(pair<string, FieldInfo *>("fieldname",  fieldname));

    FieldInfo *type = new FieldInfo(DATA_TYPE_INT, INT_SIZE, "type");
    fieldDescfields->insert(pair<string, FieldInfo *>("type", type));
    FieldInfo *length = new FieldInfo(DATA_TYPE_INT, INT_SIZE, "length");
    fieldDescfields->insert(pair<string, FieldInfo *>("length", length));
    FieldInfo *offset = new FieldInfo(DATA_TYPE_INT, INT_SIZE, "offset");
    fieldDescfields->insert(pair<string, FieldInfo *>("offset", offset));

    vector<char*> fieldMetaFieldsName ;
    fieldMetaFieldsName.push_back("tablename");
    fieldMetaFieldsName.push_back("fieldname");
    fieldMetaFieldsName.push_back("type");
    fieldMetaFieldsName.push_back("length");
    fieldMetaFieldsName.push_back("offset");

    this->fcatInfo =
            new TableInfo("fieldcat", fieldMetaFieldsName, fieldDescfields);

    if (isNew) {
        table_manager_create_table( "tablecat", tableMetaFieldsName, tableDescfields, tx);
        table_manager_create_table( "fieldcat", fieldMetaFieldsName, fieldDescfields, tx);
    }
};

/**
 * 创建tableInfo，创建表
 * @param tableManager
 * @param tableName
 * @param fieldsName
 * @param fields
 * @param tx
 * @return
 */
int TableManager::table_manager_create_table(char *tableName, vector<char*> fieldsName,  map<string, FieldInfo *>  *fields,
                               Transaction *tx) {


    /*打开元数据表*/
    RecordFile *tcatFile = new RecordFile(this->tcatInfo, tx);

    /*遍历整个record_file,第一列是tablename,第二列是recordlen，
     * 遍历第一列，判断表名是否已经存在*/
    while(tcatFile->record_file_next()){
        char *name=new_id_name();
        tcatFile->record_file_get_string("tablename",name);
        if(strcmp(name,tableName)==0)
        {
            fprintf(stderr,"table already exists!\n");
            tcatFile->record_file_close();
            return DONGMENDB_EINVALIDSQL;
        }
        //int value=record_file_get_int(tcatFile,"reclength");
    }

    /*使record_file中的指针再次指向文件开头*/
    tcatFile->record_file_before_first();


    TableInfo *tableInfo = new TableInfo(tableName, fieldsName, fields);
    /*增加元数据到table描述表中*/
    tcatFile->record_file_insert();
    tcatFile-> record_file_set_string("tablename", tableName);
    tcatFile->record_file_set_int( "reclength", tableInfo->recordLen);
    tcatFile-> record_file_close();
    free(tcatFile);

    /*打开元数据表 */
    RecordFile *fcatFile = new RecordFile(this->fcatInfo, tx);

    /*增加元数据到field描述表中*/
    int count = tableInfo->fieldsName.size() - 1;
    for (int i = 0; i <= count; i++) {
        char *fieldName = tableInfo->fieldsName.at( i);

        FieldInfo *fieldInfo = tableInfo->fields->find(fieldName)->second;
        int offset = tableInfo->offsets->find(fieldInfo->hashCode)->second;

        fcatFile-> record_file_insert();
        fcatFile->record_file_set_string( "tablename", tableName);
        fcatFile-> record_file_set_string("fieldname", fieldName);
        fcatFile-> record_file_set_int( "type", fieldInfo->type);
        fcatFile->record_file_set_int( "length", fieldInfo->length);
        fcatFile->record_file_set_int( "offset", offset);
    }
    fcatFile->record_file_close();
    free(fcatFile);
    return DONGMENDB_OK;
};

TableInfo *TableManager::table_manager_get_tableinfo(const char *tableName, Transaction *tx) {
    RecordFile *tcatFile = new RecordFile(this->tcatInfo, tx);

    int recordLen = -1;
    while (tcatFile->record_file_next()) {
        char *name = new_id_name();
        tcatFile->record_file_get_string("tablename", name);
        if (stricmp(tableName, name) == 0) {
            recordLen = tcatFile->record_file_get_int("reclength");
        }
        // free(name);
    }
    tcatFile-> record_file_close();
    free(tcatFile);

    RecordFile *fcatFile = new RecordFile( this->fcatInfo, tx);
    vector<char*> fieldsName ;
    map<string, FieldInfo *> *fields = new map<string, FieldInfo *> ();

    while (fcatFile->record_file_next()) {
        char *name = new_id_name();
        fcatFile->record_file_get_string( "tablename", name);
        if (stricmp(tableName, name) == 0) {
            char *fieldName = new_id_name();
            fcatFile->record_file_get_string( "fieldname", fieldName);
            enum data_type type = (data_type)fcatFile->record_file_get_int( "type");
            int length = fcatFile->record_file_get_int( "length");

            FieldInfo *fi = new FieldInfo(type, length, fieldName);
            fields->insert(pair<string, FieldInfo *>(fieldName, fi));

            fieldsName.push_back(fieldName);
        }
        // free(name);
    }
    fcatFile->record_file_close();
    free(fcatFile);
    TableInfo *tableInfo = new TableInfo(tableName, fieldsName, fields);
    tableInfo->recordLen = recordLen;
    return tableInfo;
};
