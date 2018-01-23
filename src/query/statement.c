//
// Created by Sam on 2018/1/17.
//

#include <malloc.h>
#include <mem.h>
#include "statement.h"

SelectStmt *createSelectStmt(
        FieldsExpr *fieldsExpr,
        TablesExpr *tablesExpr,
        Expression *whereExpr,
        GroupExpr *groupExpr,
        OrderExpr *orderExpr
) {
    SelectStmt *selectStmt = (SelectStmt *)malloc(sizeof(SelectStmt));
    selectStmt->fieldsExpr=fieldsExpr;
    selectStmt->tablesExpr =tablesExpr;
    selectStmt->whereExpr = whereExpr;
    selectStmt->groupExpr=groupExpr;
    selectStmt->orderExpr=orderExpr;
    return selectStmt;
};

CreateStmt *createCreateStmt(char *tableName,
                            ColumnsExpr *columnsExpr,
                            Constraints *constraints) {};

AlterStmt *createAlterStmt(char *tableName, enum AlterType type,
                          ColumnsExpr *columnsExpr) {};

DeleteStmt *createDeleteStmt(char *tableName,
                            Expression *whereExpr) {};

UpdateStmt *createUpdateStmt(char *tableName,
                            SetExpr *setExpr,
                            Expression *whereExpr) {};



char *printSelectStmt(SelectStmt *selectStmt, char *selectStr) {
    strcat(selectStr, "SELECT " );
    FieldsExpr *fieldsExpr;
    fieldsExpr = selectStmt->fieldsExpr;
    while (fieldsExpr!=NULL){
        printExpression(fieldsExpr->expr, selectStr);
        if (fieldsExpr->nextField!=NULL){
            strcat(selectStr, " , ");
        }
        fieldsExpr = fieldsExpr->nextField;
    }

    strcat(selectStr, "\nFROM " );
    TablesExpr *tablesExpr;
    tablesExpr = selectStmt->tablesExpr;
    while (tablesExpr != NULL) {
        strcat(selectStr, tablesExpr->name);
        if (tablesExpr->nextTable != NULL) {
            strcat(selectStr, " , ");
        }
        tablesExpr = tablesExpr->nextTable;
    }

    strcat(selectStr, "\nWHERE " );
    printExpression(selectStmt->whereExpr, selectStr);
};