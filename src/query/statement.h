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
    /*返回真假的表达式*/
    Expression *whereExpr;
    GroupExpr *groupExpr;
    OrderExpr *orderExpr;

} SelectStmt;

typedef struct CreateStmt_ {
    TablesExpr *tableExpr;
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
