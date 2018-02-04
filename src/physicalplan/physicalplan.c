//
// Created by Sam on 2018/2/2.
//

#include "physicalplan.h"
#include "tableplan.h"

int plan_execute_delete(dongmengdb *db, char *tableName, Expression *condition, transaction *tx){

};

int plan_execute_update(dongmengdb *db, char *tableName, arraylist *fieldNames, hmap_t values, Expression *condition, transaction *tx){

};

/**
 * insert数据，一次一条
 * @param db
 * @param tableName
 * @param fieldNames
 * @param values
 * @param tx
 * @return
 */
int plan_execute_insert(dongmengdb *db, char *tableName, arraylist *fieldNames, arraylist *values, transaction *tx){
    table_plan *tablePlan = table_plan_create(db, tableName, tx);
    table_scan *tableScan = table_plan_open(tablePlan);
    table_scan_insert(tableScan);
    for (size_t i = 0; i < fieldNames->size; i++){

        char *fieldName = arraylist_get(fieldNames, i);

        void_ptr *ptr = (void_ptr *) malloc(sizeof(void_ptr *));
        hashmap_get(tablePlan->tableInfo->fields, fieldName, ptr);
        field_info *fieldInfo = *ptr;

        int type = fieldInfo->type;
        if (type == DATA_TYPE_INT) {
            integer *val = arraylist_get(values, i);
            table_scan_set_int(tableScan, fieldName, val->val);
        }else if (type == DATA_TYPE_CHAR){
            char *val = arraylist_get(values, i);
            /*字符串超出定义时的长度，则截断字符串.*/
            if(fieldInfo->length<strlen(val)){
                val[fieldInfo->length] = '\0';
            }
            table_scan_set_string(tableScan, fieldName, val);
        }else{
            return DONGMENGDB_EINVALIDSQL;
        }
    }
    return DONGMENGDB_OK;
};

int plan_execute_create_table(char *tableName, table_info *tableInfo, transaction *tx){

};
