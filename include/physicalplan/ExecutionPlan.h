//
// Created by sam on 2018/11/16.
//

#ifndef DONGMENDB_EXECUTIONPLAN_H
#define DONGMENDB_EXECUTIONPLAN_H

#include <dongmendb/DongmenDB.h>
#include <dongmensql/sra.h>
#include <dongmensql/statement.h>
#include <physicalplan/Scan.h>

/*执行计划实现*/
class ExecutionPlan {
    public :
    Scan* generateSelect(DongmenDB *db, SRA_t *sra, Transaction *tx);

    Scan* generateScan(DongmenDB *db, SRA_t *sra, Transaction *tx);

    int executeInsert(DongmenDB *db, char *tableName,  vector<char*> fieldNames,  vector<variant*> values, Transaction *tx);
    int executeUpdate(DongmenDB *db, sql_stmt_update *sqlStmtUpdate, Transaction *tx);
    int executeDelete(DongmenDB *db, sql_stmt_delete *sqlStmtDelete, Transaction *tx);
};


#endif //DONGMENDB_EXECUTIONPLAN_H
