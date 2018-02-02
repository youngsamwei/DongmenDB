//
// Created by Sam on 2018/1/17.
//

#ifndef DONGMENDB_STATEMENT_H
#define DONGMENDB_STATEMENT_H

#include <arraylist.h>
#include "expression.h"
#include "sqlexpression.h"

/*定义语句级的结构*/
typedef struct SelectStmt_ {
    FieldsExpr *fieldsExpr;
    TablesExpr *tablesExpr;
    /*返回真假的表达式*/
    Expression *whereExpr;
    GroupExpr *groupExpr;
    OrderExpr *orderExpr;

} SelectStmt;


typedef struct sql_stmt_create_ {
    char *tableName;
    arraylist *columns;
   // ColumnsExpr *columnsExpr;
    Constraints *constraints;

} sql_stmt_create;

enum AlterType {
    ALTER_ADD,
    ALTER_REMOVE
};

typedef struct AlterStmt_ {
    char *tableName;
    enum AlterType type;
    ColumnsExpr *columnsExpr;
} AlterStmt;

typedef struct InsertStmt_ {
    char *tableName;
    FieldsExpr *fieldsExpr;
    ValueList *valueList;
} InsertStmt;

typedef struct DeleteStmt_ {
    char *tableName;
    Expression *whereExpr;
} DeleteStmt;

typedef struct UpdateStmt_ {
    char *tableName;
    SetExpr *setExpr;
    Expression *whereExpr;
} UpdateStmt;


SelectStmt *createSelectStmt(
        FieldsExpr *fieldsExpr,
        TablesExpr *tablesExpr,
        Expression *whereExpr,
        GroupExpr *groupExpr,
        OrderExpr *orderExpr
);

sql_stmt_create *sql_stmt_create_create(char *tableName,
                                        arraylist *columns,
                                        Constraints *constraints);

AlterStmt *createAlterStmt(char *tableName, enum AlterType type,
                           ColumnsExpr *columnsExpr);

DeleteStmt *createDeleteStmt(char *tableName,
                             Expression *whereExpr);

UpdateStmt *createUpdateStmt(char *tableName,
                             SetExpr *setExpr,
                             Expression *whereExpr);

char *printSelectStmt(char *selectStr, SelectStmt *selectStmt);

#endif //DONGMENDB_STATEMENT_H
