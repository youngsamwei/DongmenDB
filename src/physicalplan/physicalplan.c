//
// Created by Sam on 2018/2/2.
//

#include "physicalplan.h"
#include "physicalscan.h"
#include "physical_scan_table.h"

physical_scan *plan_execute_select(dongmendb *db, SRA_t *sra, transaction *tx){
    physical_scan *plan = physical_scan_generate(db, sra, tx);
    if (plan->scanType == SCAN_TYPE_PROJECT) {
        /*处理select 中形如 student.* */
        physical_scan_project_generate_expr_list(plan);
    }
    return plan;
};

int plan_execute_delete(dongmendb *db, char *tableName, Expression *condition, transaction *tx){

};

int plan_execute_update(dongmendb *db, char *tableName, arraylist *fieldNames, hmap_t values, Expression *condition, transaction *tx){

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
int plan_execute_insert(dongmendb *db, char *tableName, arraylist *fieldNames, arraylist *values, transaction *tx){
    physical_scan *scan = physical_scan_table_create(db, tableName, tx);
    scan->insert(scan);
    for (size_t i = 0; i < fieldNames->size; i++){

        char *fieldName = arraylist_get(fieldNames, i);

        void_ptr *ptr = (void_ptr *) calloc(sizeof(void_ptr *), 1);
        hashmap_get(scan->physicalScanTable->tableInfo->fields, fieldName, ptr);
        field_info *fieldInfo = *ptr;

        int type = fieldInfo->type;
        if (type == DATA_TYPE_INT) {
            integer *val = arraylist_get(values, i);
            scan->setInt(scan, tableName, fieldName, val->val);
        }else if (type == DATA_TYPE_CHAR){
            char *val = arraylist_get(values, i);
            /*字符串超出定义时的长度，则截断字符串.*/
            if(fieldInfo->length<strlen(val)){
                val[fieldInfo->length] = '\0';
            }
            scan->setString(scan, tableName, fieldName, val);
        }else{
            return DONGMENDB_EINVALIDSQL;
        }
    }
    return DONGMENDB_OK;
};

int plan_execute_create_table(char *tableName, table_info *tableInfo, transaction *tx){

};
