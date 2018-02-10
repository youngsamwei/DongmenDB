//
// Created by Sam on 2018/1/17.
//

#include <malloc.h>
#include <mem.h>
#include "statement.h"

sql_stmt_create *sql_stmt_create_create(char *tableName,
                            arraylist *fieldsName, hmap_t columns,
                                        Constraint_t *constraints) {
    sql_stmt_create *sqlStmtCreate = (sql_stmt_create *)malloc(sizeof(sql_stmt_create));
    sqlStmtCreate->tableInfo =  table_info_create(tableName, fieldsName, columns);
    sqlStmtCreate->constraints = constraints;
    return sqlStmtCreate;
};
/*
AlterStmt *createAlterStmt(char *tableName, enum AlterType type,
                          ColumnsExpr *columnsExpr) {};

DeleteStmt *createDeleteStmt(char *tableName,
                            Expression *whereExpr) {};

UpdateStmt *createUpdateStmt(char *tableName,
                            SetExpr *setExpr,
                            Expression *whereExpr) {};


*/