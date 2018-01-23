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


char *printSelectStmt(SelectStmt *selectStmt) {
    char *selectstr = "SELECT ";

    FieldsExpr *fieldsExpr;
    fieldsExpr = selectStmt->fieldsExpr;
    while (fieldsExpr!=NULL){
        strcat(selectstr, printExpression(fieldsExpr->expr));
        if (fieldsExpr->nextField!=NULL){
            strcat(selectstr, " , ");
        }
        fieldsExpr = fieldsExpr->nextField;
    }

    TablesExpr *tablesExpr;
    tablesExpr = selectStmt->tablesExpr;
    while (tablesExpr != NULL) {
        strcat(selectstr, tablesExpr->name);
        if (tablesExpr->nextTable != NULL) {
            strcat(selectstr, " , ");
        }
        tablesExpr = tablesExpr->nextTable;
    }

}
