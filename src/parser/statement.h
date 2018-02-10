//
// Created by Sam on 2018/1/17.
//

#ifndef DONGMENDB_STATEMENT_H
#define DONGMENDB_STATEMENT_H

#include <arraylist.h>
#include "../dongmendb/recordfile.h"
#include "sqlexpression.h"
#include "column.h"


typedef struct sql_stmt_create_ {
    table_info *tableInfo;

    Constraint_t *constraints;

} sql_stmt_create;

typedef struct sql_stmt_insert_{
    char *tableName;
    arraylist *fields;
    arraylist *values;
} sql_stmt_insert;

/*
enum AlterType {
    ALTER_ADD,
    ALTER_REMOVE
};

typedef struct AlterStmt_ {
    char *tableName;
    enum AlterType type;
    ColumnsExpr *columnsExpr;
} AlterStmt;


typedef struct DeleteStmt_ {
    char *tableName;
    Expression *whereExpr;
} DeleteStmt;

typedef struct UpdateStmt_ {
    char *tableName;
    SetExpr *setExpr;
    Expression *whereExpr;
} UpdateStmt;
*/

sql_stmt_create *sql_stmt_create_create(char *tableName,
                                        arraylist *fieldsName, hmap_t columns,
                                        Constraint_t *constraints);
/*
AlterStmt *createAlterStmt(char *tableName, enum AlterType type,
                           ColumnsExpr *columnsExpr);

DeleteStmt *createDeleteStmt(char *tableName,
                             Expression *whereExpr);

UpdateStmt *createUpdateStmt(char *tableName,
                             SetExpr *setExpr,
                             Expression *whereExpr);

*/
#endif //DONGMENDB_STATEMENT_H
