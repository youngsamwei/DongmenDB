//
// Created by Sam on 2018/1/26.
//

#include "metadatamanager.h"

int table_manager_create(table_manager *tableManager, int isNew, transaction *tx) {
    field_info *fieldInfo;

    hmap_t tableDescfields = hashmap_create();
    field_info_create(fieldInfo, DATA_TYPE_CHAR, MAX_ID_NAME_LENGTH);
    hashmap_put(tableDescfields, "tablename", fieldInfo);
    field_info_create(fieldInfo, DATA_TYPE_INT, INT_SIZE);
    hashmap_put(tableDescfields, "reclength", fieldInfo);

    table_info_create(tableManager->tcatInfo, "tablecat", tableDescfields);

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

    table_info_create(tableManager->fcatInfo, "fieldcat", fieldDescfields);

    if(isNew){
        table_manager_create_table(tableManager, "tablecat", tableDescfields, tx);
        table_manager_create_table(tableManager, "fieldcat", fieldDescfields, tx);
    }
};

int table_manager_create_table(table_manager *tableManager, char *tableName, hmap_t fields, transaction *tx) {
    table_info *tableInfo ;
    table_info_create(tableInfo, tableName, fields);
    record_file *tcatFile;
    record_file_create(tcatFile, tableManager->tcatInfo, tx);
    record_file_insert(tcatFile);
    record_file_set_string(tcatFile, "tablename", tableName);
    record_file_set_int(tcatFile, "reclength", tableInfo->recordLen);
    record_file_close(tcatFile);


};

int table_manager_get_tableinfo(table_manager *tableManager, char *tableName, transaction tx, hmap_t fields) {

};
