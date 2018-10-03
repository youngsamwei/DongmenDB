//
// Created by Sam on 2018/1/26.
//

#include <utils/utils.h>
#include "dongmendb/metadatamanager.h"

int metadata_manager_create(metadata_manager *metadataManager, const char *file, transaction *tx, int isNew) {
    metadataManager->tableManager = table_manager_create(isNew, tx);
};

table_manager *table_manager_create(int isNew, transaction *tx) {
    table_manager *tableManager = (table_manager *) malloc(sizeof(table_manager));
    hmap_t tableDescfields = hashmap_create();

    field_info *fieldInfo = field_info_create(DATA_TYPE_CHAR, MAX_ID_NAME_LENGTH);
    hashmap_put(tableDescfields, "tablename", fieldInfo);

    fieldInfo = field_info_create(DATA_TYPE_INT, INT_SIZE);
    hashmap_put(tableDescfields, "reclength", fieldInfo);

    arraylist *tableMetaFieldsName = arraylist_create();
    arraylist_add(tableMetaFieldsName, "tablename");
    arraylist_add(tableMetaFieldsName, "reclength");

    tableManager->tcatInfo = table_info_create("tablecat", tableMetaFieldsName, tableDescfields);

    hmap_t fieldDescfields = hashmap_create();
    fieldInfo = field_info_create(DATA_TYPE_CHAR, MAX_ID_NAME_LENGTH);
    hashmap_put(fieldDescfields, "tablename", fieldInfo);
    fieldInfo = field_info_create(DATA_TYPE_CHAR, MAX_ID_NAME_LENGTH);
    hashmap_put(fieldDescfields, "fieldname", fieldInfo);
    fieldInfo = field_info_create(DATA_TYPE_INT, INT_SIZE);
    hashmap_put(fieldDescfields, "type", fieldInfo);
    fieldInfo = field_info_create(DATA_TYPE_INT, INT_SIZE);
    hashmap_put(fieldDescfields, "length", fieldInfo);
    fieldInfo = field_info_create(DATA_TYPE_INT, INT_SIZE);
    hashmap_put(fieldDescfields, "offset", fieldInfo);

    arraylist *fieldMetaFieldsName = arraylist_create();
    arraylist_add(fieldMetaFieldsName, "tablename");
    arraylist_add(fieldMetaFieldsName, "fieldname");
    arraylist_add(fieldMetaFieldsName, "type");
    arraylist_add(fieldMetaFieldsName, "length");
    arraylist_add(fieldMetaFieldsName, "offset");

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
int table_manager_create_table(table_manager *tableManager, char *tableName, arraylist *fieldsName, hmap_t fields,
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
    int count = tableInfo->fieldsName->size - 1;
    for (int i = 0; i <= count; i++) {
        char *fieldName = arraylist_get(tableInfo->fieldsName, i);

        void_ptr *ptr = (void_ptr *) malloc(sizeof(void_ptr *));
        hashmap_get(tableInfo->fields, fieldName, ptr);
        field_info *fieldInfo = *ptr;

        int offset = table_info_offset(tableInfo, fieldName);

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

table_info *table_manager_get_tableinfo(table_manager *tableManager, char *tableName, transaction *tx) {
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
    arraylist *fieldsName = arraylist_create();
    hmap_t *fields = hashmap_create();
    hmap_t *offsets = hashmap_create();
    while (record_file_next(fcatFile)) {
        char *name = new_id_name();
        record_file_get_string(fcatFile, "tablename", name);
        if (stricmp(tableName, name) == 0) {
            char *fieldName = new_id_name();
            record_file_get_string(fcatFile, "fieldname", fieldName);
            int type = record_file_get_int(fcatFile, "type");
            int length = record_file_get_int(fcatFile, "length");
            int offset = record_file_get_int(fcatFile, "offset");
            integer *soffset = (integer *) malloc(sizeof(integer));
            soffset->val = offset;
            field_info *fi = field_info_create(type, length);
            hashmap_put(fields, fieldName, fi);
            hashmap_put(offsets, fieldName, soffset);
            arraylist_add(fieldsName, fieldName);
        }
        // free(name);
    }
    record_file_close(fcatFile);
    table_info *tableInfo = table_info_create(tableName, fieldsName, fields);
    tableInfo->recordLen = recordLen;
    return tableInfo;
};
