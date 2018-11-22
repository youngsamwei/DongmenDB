//
// Created by Sam on 2018/1/26.
//

#include <utils/utils.h>
#include <iostream>
#include "dongmendb/metadatamanager.h"

int metadata_manager_create(metadata_manager *metadataManager, const char *file, transaction *tx, int isNew) {
    metadataManager->tableManager = table_manager_create(isNew, tx);
};

table_manager *table_manager_create(int isNew, transaction *tx) {
    table_manager *tableManager = (table_manager *) malloc(sizeof(table_manager));
    map<string, field_info*> *tableDescfields = new map<string, field_info*>();

    field_info *tablename = field_info_create(DATA_TYPE_CHAR, MAX_ID_NAME_LENGTH, "tablename");
    tableDescfields->insert(pair<string, field_info*>("tablename", tablename));

    field_info *reclength = field_info_create(DATA_TYPE_INT, INT_SIZE, "reclength");
    tableDescfields->insert(pair<string, field_info*>("reclength", reclength));

    vector<char*> tableMetaFieldsName;
    tableMetaFieldsName.push_back("tablename");
    tableMetaFieldsName.push_back("reclength");

    tableManager->tcatInfo = table_info_create("tablecat", tableMetaFieldsName, tableDescfields);

    map<string, field_info*> *fieldDescfields  = new map<string, field_info*>();

    field_info *field_tablename = field_info_create(DATA_TYPE_CHAR, MAX_ID_NAME_LENGTH, "tablename");

    fieldDescfields->insert(pair<string, field_info*>("tablename",field_tablename));


    field_info *fieldname = field_info_create(DATA_TYPE_CHAR, MAX_ID_NAME_LENGTH, "fieldname");
    fieldDescfields->insert(pair<string, field_info*>("fieldname",  fieldname));

    field_info *type = field_info_create(DATA_TYPE_INT, INT_SIZE, "type");
    fieldDescfields->insert(pair<string, field_info*>("type", type));
    field_info *length = field_info_create(DATA_TYPE_INT, INT_SIZE, "length");
    fieldDescfields->insert(pair<string, field_info*>("length", length));
    field_info *offset = field_info_create(DATA_TYPE_INT, INT_SIZE, "offset");
    fieldDescfields->insert(pair<string, field_info*>("offset", offset));

    vector<char*> fieldMetaFieldsName ;
    fieldMetaFieldsName.push_back("tablename");
    fieldMetaFieldsName.push_back("fieldname");
    fieldMetaFieldsName.push_back("type");
    fieldMetaFieldsName.push_back("length");
    fieldMetaFieldsName.push_back("offset");

    tableManager->fcatInfo =
            table_info_create("fieldcat", fieldMetaFieldsName, fieldDescfields);

    if (isNew) {
        table_manager_create_table(tableManager, "tablecat", tableMetaFieldsName, tableDescfields, tx);
        table_manager_create_table(tableManager, "fieldcat", fieldMetaFieldsName, fieldDescfields, tx);
    }
    return tableManager;
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
int table_manager_create_table(table_manager *tableManager, char *tableName, vector<char*> fieldsName,  map<string, field_info*>  *fields,
                               transaction *tx) {


    /*打开元数据表*/
    record_file *tcatFile = (record_file *) malloc(sizeof(record_file));
    record_file_create(tcatFile, tableManager->tcatInfo, tx);

    /*遍历整个record_file,第一列是tablename,第二列是recordlen，
     * 遍历第一列，判断表名是否已经存在*/
    while(record_file_next(tcatFile)){
        char *name=new_id_name();
        record_file_get_string(tcatFile,"tablename",name);
        if(strcmp(name,tableName)==0)
        {
            fprintf(stderr,"table already exists!\n");
            record_file_close(tcatFile);
            return DONGMENDB_EINVALIDSQL;
        }
        //int value=record_file_get_int(tcatFile,"reclength");
    }

    /*使record_file中的指针再次指向文件开头*/
    record_file_before_first(tcatFile);


    table_info *tableInfo = table_info_create(tableName, fieldsName, fields);
    /*增加元数据到table描述表中*/
    record_file_insert(tcatFile);
    record_file_set_string(tcatFile, "tablename", tableName);
    record_file_set_int(tcatFile, "reclength", tableInfo->recordLen);
    record_file_close(tcatFile);

    /*打开元数据表 */
    record_file *fcatFile = (record_file *) malloc(sizeof(record_file));
    record_file_create(fcatFile, tableManager->fcatInfo, tx);

    /*增加元数据到field描述表中*/
    int count = tableInfo->fieldsName.size() - 1;
    for (int i = 0; i <= count; i++) {
        char *fieldName = tableInfo->fieldsName.at( i);

        field_info *fieldInfo = tableInfo->fields->find(fieldName)->second;
        unsigned int fid = bkdr_hash(fieldName);
        int offset = tableInfo->offsets->find(fid)->second;

        record_file_insert(fcatFile);
        record_file_set_string(fcatFile, "tablename", tableName);
        record_file_set_string(fcatFile, "fieldname", fieldName);
        record_file_set_int(fcatFile, "type", fieldInfo->type);
        record_file_set_int(fcatFile, "length", fieldInfo->length);
        record_file_set_int(fcatFile, "offset", offset);
    }
    record_file_close(fcatFile);
    return DONGMENDB_OK;
};

table_info *table_manager_get_tableinfo(table_manager *tableManager, const char *tableName, transaction *tx) {
    record_file *tcatFile = (record_file *) malloc(sizeof(record_file));
    record_file_create(tcatFile, tableManager->tcatInfo, tx);

    int recordLen = -1;
    while (record_file_next(tcatFile)) {
        char *name = new_id_name();
        record_file_get_string(tcatFile, "tablename", name);
        if (stricmp(tableName, name) == 0) {
            recordLen = record_file_get_int(tcatFile, "reclength");
        }
        // free(name);
    }
    record_file_close(tcatFile);

    record_file *fcatFile = (record_file *) malloc(sizeof(record_file));
    record_file_create(fcatFile, tableManager->fcatInfo, tx);
    vector<char*> fieldsName ;
    map<string, field_info*> *fields = new map<string, field_info*> ();

    while (record_file_next(fcatFile)) {
        char *name = new_id_name();
        record_file_get_string(fcatFile, "tablename", name);
        if (stricmp(tableName, name) == 0) {
            char *fieldName = new_id_name();
            record_file_get_string(fcatFile, "fieldname", fieldName);
            enum data_type type = (data_type)record_file_get_int(fcatFile, "type");
            int length = record_file_get_int(fcatFile, "length");

            field_info *fi = field_info_create(type, length, "length");
            fields->insert(pair<string, field_info*>(fieldName, fi));

            fieldsName.push_back(fieldName);
        }
        // free(name);
    }
    record_file_close(fcatFile);
    table_info *tableInfo = table_info_create(tableName, fieldsName, fields);
    tableInfo->recordLen = recordLen;
    return tableInfo;
};
