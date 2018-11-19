//
// Created by sam on 2018/11/16.
//


#include <physicalplan/ExecutionPlan.h>
#include <physicalplan/TableScan.h>
#include <physicalplan/Select.h>
#include <physicalplan/Project.h>
#include <physicalplan/Join.h>

Scan* ExecutionPlan::generateSelect(dongmendb *db, SRA_t *sra, transaction *tx){
    Scan* plan = generateScan(db, sra, tx);

    return plan;
};

Scan* ExecutionPlan::generateScan(dongmendb *db, SRA_t *sra, transaction *tx){
    switch (sra->t) {
        case SRA_TABLE: {
            TableScan *tableScan = new TableScan(db, sra->table.ref->table_name, tx);
            return tableScan;
        }
        case SRA_SELECT: {
            Scan* scan = (Scan*)this->generateScan(db, sra->select.sra, tx);
            Select *select = new Select(scan);
            select->cond = sra->select.cond;
            return select;
        }
        case SRA_PROJECT: {
            Scan* scan = this->generateScan(db, sra->select.sra, tx);
            Project *project = new Project(scan);

            project->order_by.assign(sra->project.order_by.begin(), sra->project.order_by.end())  ;

            project->group_by.assign(sra->project.group_by.begin(), sra->project.group_by.end());
            project->distinct = sra->project.distinct;
            /*设置表达式列表 */
            project->setOriginalExprList(sra->project.expr_list);

            return project;
        }
//        case SRA_UNION:
//            return (Scan&)nullptr;
//        case SRA_EXCEPT:
//            return (Scan&)nullptr;
//        case SRA_INTERSECT:
//            return (Scan&)nullptr;
        case SRA_JOIN: {//cross join , product
            Scan* scan1 = this->generateScan(db, sra->binary.sra1, tx);
            Scan* scan2 = this->generateScan(db, sra->binary.sra2, tx);
            Join *join = new Join(scan1, scan2);
            join->cond = NULL;
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

int ExecutionPlan::executeInsert(dongmendb *db, char *tableName,  vector<char*> fieldNames,  vector<variant*> values, transaction *tx){
    TableScan tableScan(db, tableName, tx);
    tableScan.insertRecord();
    for (size_t i = 0; i < fieldNames.size(); i++){

        char *fieldName = fieldNames.at(i);

        void_ptr *ptr = (void_ptr *) calloc(sizeof(void_ptr *), 1);
        hashmap_get(tableScan.m_tableInfo->fields, fieldName, ptr);
        field_info *fieldInfo = (field_info *)*ptr;

        /* TODO: 完整性检查 */
        int type = fieldInfo->type;
        if (type == DATA_TYPE_INT) {
            integer *val = (integer *)values.at(i);
            tableScan.setInt(tableName, fieldName, val->val);
        }else if (type == DATA_TYPE_CHAR){
            char *val = (char *)values.at(i);
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