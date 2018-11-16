//
// Created by sam on 2018/11/16.
//


#include <physicalplan/ExecutionPlan.h>
#include <physicalplan/TableScan.h>
#include <physicalplan/Select.h>
#include <physicalplan/Project.h>
#include <physicalplan/Join.h>

Scan& ExecutionPlan::generateSelect(dongmendb *db, SRA_t *sra, transaction *tx){
    Scan& plan = generateScan(db, sra, tx);
//    if (typeid(*plan) == typeid(Project)) {
    /*处理select 中形如 student.* */
//        physical_scan_project_generate_expr_list(plan);
//    }
    return plan;
};

Scan& ExecutionPlan::generateScan(dongmendb *db, SRA_t *sra, transaction *tx){
    switch (sra->t) {
        case SRA_TABLE: {
            TableScan tableScan(db, sra->table.ref->table_name, tx);
            return tableScan;
        }
        case SRA_SELECT: {
            Scan& scan = this->generateScan(db, sra->select.sra, tx);
            Select select(scan);
            select.cond = sra->select.cond;
            return select;
        }
        case SRA_PROJECT: {
            Scan& scan = this->generateScan(db, sra->select.sra, tx);
            Project project(scan);

            project.order_by = sra->project.order_by;
            project.group_by = sra->project.group_by;
            project.distinct = sra->project.distinct;
            /*处理 形如student.*的字段表示 */
            project.original_expr_list = sra->project.expr_list;
            project.expr_list = arraylist_create();

            return project;
        }
//        case SRA_UNION:
//            return (Scan&)nullptr;
//        case SRA_EXCEPT:
//            return (Scan&)nullptr;
//        case SRA_INTERSECT:
//            return (Scan&)nullptr;
        case SRA_JOIN: {//cross join , product
            Scan& scan1 = this->generateScan(db, sra->binary.sra1, tx);
            Scan& scan2 = this->generateScan(db, sra->binary.sra2, tx);
            Join join(scan1, scan2);
            join.cond = NULL;
            return join;
        }
//        case SRA_NATURAL_JOIN:
//            return (Scan&)nullptr;
//        case SRA_LEFT_OUTER_JOIN:
//        case SRA_RIGHT_OUTER_JOIN:
//        case SRA_FULL_OUTER_JOIN:
//            return (Scan&)nullptr;
//        default:
//            return (Scan&)nullptr;
    }

};

int ExecutionPlan::executeInsert(dongmendb *db, char *tableName, arraylist *fieldNames, arraylist *values, transaction *tx){
    TableScan tableScan(db, tableName, tx);
    tableScan.insertRecord();
    for (size_t i = 0; i < fieldNames->size; i++){

        char *fieldName = (char *)arraylist_get(fieldNames, i);

        void_ptr *ptr = (void_ptr *) calloc(sizeof(void_ptr *), 1);
        hashmap_get(tableScan.m_tableInfo->fields, fieldName, ptr);
        field_info *fieldInfo = (field_info *)*ptr;

        /* TODO: 完整性检查 */
        int type = fieldInfo->type;
        if (type == DATA_TYPE_INT) {
            integer *val = (integer *)arraylist_get(values, i);
            tableScan.setInt(tableName, fieldName, val->val);
        }else if (type == DATA_TYPE_CHAR){
            char *val = (char *)arraylist_get(values, i);
            /*字符串超出定义时的长度，则截断字符串.*/
            if(fieldInfo->length<strlen(val)){
                val[fieldInfo->length] = '\0';
            }
            tableScan.setString(tableName, fieldName, val);
        }else{
            return DONGMENDB_EINVALIDSQL;
        }
    }
    return DONGMENDB_OK;
};

int ExecutionPlan::executeUpdate(dongmendb *db, sql_stmt_update *sqlStmtUpdate, transaction *tx){

    return -1;
};
int ExecutionPlan::executeDelete(dongmendb *db, sql_stmt_delete *sqlStmtDelete, transaction *tx){
    return -1;
};