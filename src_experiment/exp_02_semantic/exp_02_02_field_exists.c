//
// Created by Sam on 2018/2/13.
//

#include <dongmendb/dongmendb.h>

/*
 * 检查字段是否存在
 * 此函数在以下情况会被调用：
 * 1. 执行select，update，insert，delete时调用，检查相关字段是否存在
 * 2  执行create index时检查相关字段是否存在
 * */


int semantic_check_field_exists(table_manager *tableManager, char *tableName, char *fieldName, transaction *tx){


    /*
     * 1. 调用 table_manager_get_tableinfo 获得 table_info
     * 2. 若 table_info 为NULL则不存在，否则存在.
     * 3  通过table_info查看fieldsName是否存在指定的fieldName
     * */
    return DONGMENDB_OK;
}