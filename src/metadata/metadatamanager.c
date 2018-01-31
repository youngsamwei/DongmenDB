//
// Created by Sam on 2018/1/26.
//

#include "metadatamanager.h"

int metadata_manager_create(metadata_manager *metadataManager, const char *file, transaction *tx, int isNew){
    metadataManager->tableManager =    table_manager_create( isNew, tx);
};

table_manager *table_manager_create(int isNew, transaction *tx) {
    table_manager *tableManager = (table_manager *)malloc(sizeof(table_manager));
    hmap_t tableDescfields = hashmap_create();

    field_info *fieldInfo = (field_info *)malloc(sizeof(field_info));
    field_info_create(fieldInfo, DATA_TYPE_CHAR, MAX_ID_NAME_LENGTH);
    hashmap_put(tableDescfields, "tablename", fieldInfo);

    fieldInfo = (field_info *)malloc(sizeof(field_info));
    field_info_create(fieldInfo, DATA_TYPE_INT, INT_SIZE);
    hashmap_put(tableDescfields, "reclength", fieldInfo);

    arraylist *tableMetaFieldsName = arraylist_create();
    arraylist_add(tableMetaFieldsName, "tablename");
    arraylist_add(tableMetaFieldsName, "reclength");

    tableManager->tcatInfo = (table_info *)malloc(sizeof(table_info));
    table_info_create(tableManager->tcatInfo, "tablecat", tableMetaFieldsName, tableDescfields);

    hmap_t fieldDescfields = hashmap_create();
    field_info_create(fieldInfo, DATA_TYPE_CHAR, MAX_ID_NAME_LENGTH);
    hashmap_put(fieldDescfields, "tablename", fieldInfo);
    field_info_create(fieldInfo, DATA_TYPE_CHAR, MAX_ID_NAME_LENGTH);
    hashmap_put(fieldDescfields, "fieldname", fieldInfo);
    field_info_create(fieldInfo, DATA_TYPE_INT, INT_SIZE);
    hashmap_put(fieldDescfields, "type", fieldInfo);
    field_info_create(fieldInfo, DATA_TYPE_INT, INT_SIZE);
    hashmap_put(fieldDescfields, "length", fieldInfo);
    field_info_create(fieldInfo, DATA_TYPE_INT, INT_SIZE);
    hashmap_put(fieldDescfields, "offset", fieldInfo);

    arraylist *fieldMetaFieldsName = arraylist_create();
    arraylist_add(fieldMetaFieldsName, "tablename");
    arraylist_add(fieldMetaFieldsName, "fieldname");
    arraylist_add(fieldMetaFieldsName, "type");
    arraylist_add(fieldMetaFieldsName, "length");
    arraylist_add(fieldMetaFieldsName, "offset");

    tableManager->fcatInfo = (table_info *)malloc(sizeof(table_info));
    table_info_create(tableManager->fcatInfo, "fieldcat", fieldMetaFieldsName, fieldDescfields);

    if(isNew){
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
int table_manager_create_table(table_manager *tableManager, char *tableName, arraylist *fieldsName, hmap_t fields, transaction *tx) {
    table_info *tableInfo = (table_info *)malloc(sizeof(table_info));
    table_info_create(tableInfo, tableName, fieldsName, fields);

    /*打开元数据表*/
    record_file *tcatFile = (record_file *) malloc(sizeof(record_file));
    record_file_create(tcatFile, tableManager->tcatInfo, tx);

    /*增加元数据到table描述表中*/
    record_file_insert(tcatFile);
    record_file_set_string(tcatFile, "tablename", tableName);
    record_file_set_int(tcatFile, "reclength", tableInfo->recordLen);
    record_file_close(tcatFile);

    /*打开元数据表 */
    record_file *fcatFile = (record_file *) malloc(sizeof(record_file));
    record_file_create(fcatFile, tableManager->fcatInfo, tx);

    /*增加元数据到field描述表中*/
    for (int i = 0; i<=tableInfo->fieldsName->size - 1; i++) {
        char *fieldName = arraylist_get(tableInfo->fieldsName, i);

        void_ptr *ptr = (void_ptr *)malloc(sizeof(void_ptr));
        hashmap_get(tableInfo->fields, fieldName, ptr);
        field_info *fieldInfo = *ptr;

        void_ptr *ofvalue  = (void_ptr)malloc(sizeof(void_ptr));
        hashmap_get(tableInfo->offsets, fieldName, ofvalue);
        integer *offset  =  *ofvalue;

        record_file_insert(fcatFile);
        record_file_set_string(fcatFile, "tablename", tableName);
        record_file_set_string(fcatFile, "fieldname", fieldName);
        record_file_set_int(fcatFile, "type", fieldInfo->type);
        record_file_set_int(fcatFile, "length", fieldInfo->length);
        record_file_set_int(fcatFile, "offset", offset->val);
    }
    record_file_close(fcatFile);
};

int table_manager_get_tableinfo(table_manager *tableManager, char *tableName, transaction tx, hmap_t fields) {

};
