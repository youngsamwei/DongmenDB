//
// Created by Sam on 2018/1/17.
//

#ifndef DONGMENDB_STATEMENT_H
#define DONGMENDB_STATEMENT_H

#include "expression.h"

/*定义语句级的结构*/
typedef struct SelectStmt_ {
    FieldsExpr *fieldsExpr;
    TablesExpr *tablesExpr;
    WhereExpr *whereExpr;
    GroupExpr *groupExpr;
    OrderExpr *orderExpr;

} SelectStmt;

typedef struct CreateStmt_ {
    TableExpr *tableExpr;
    ColumnsExpr *columnsExpr;
    ConstraintsExpr *constraintsExpr;

} CreateStmt_;

typedef struct AlterStmt_ {

} AlterStmt;

typedef struct DeleteStmt_ {
    TablesExpr *tablesExpr;
    WhereExpr *whereExpr;
} DeleteStmt;

typedef struct UpdateStmt_ {
    TablesExpr *tablesExpr;
    SetExpr *setExpr;
    WhereExpr *whereExpr;
} UpdateStmt;

#endif //DONGMENDB_STATEMENT_H
