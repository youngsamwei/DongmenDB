//
// Created by Sam on 2018/1/17.
//

#include <malloc.h>
#include "parser/statement.h"
#include <dongmensql/dongmensql.h>


sql_stmt_create *parseCreate(Parser *parser) { return NULL; };
/*
AlterStmt *parseAlter(Parser *parser) { return NULL; };

DeleteStmt *parseDelete(Parser *parser) { return NULL; };

UpdateStmt *parseUpdate(Parser *parser) { return NULL; };
*/
/*
TermExpr *parseTermExpr(Parser *parser) { return NULL; };

BinaryExpr *parseBinaryExpr(Parser *parser) { return NULL; };

UnaryExpr *parseUnaryExpr(Parser *parser) { return NULL; };

arraylist *parseGroupExpr(Parser *parser) { return NULL; };
*/
vector<Expression*> parseOrderExpr(Parser *parser) {
    vector<Expression*> v;
    return v; };
/*
ColumnsExpr *parseColumnsExpr(Parser *parser) { return NULL; };

SetExpr *parseSetExpr(Parser *parser) { return NULL; };
*/
int dongmensql_parser(const char *sql, dongmensql_statement_t **stmt){};
int dongmensql_stmt_print(dongmensql_statement_t *stmt){};

/* 打印 sql_stmt_update */
int sql_stmt_update_print(sql_stmt_update *sqlStmtUpdate){
    SRA_print(sqlStmtUpdate->where);
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