//
// Created by Sam on 2018/1/17.
//

#include <malloc.h>
#include "parser/statement.h"
#include <dongmensql/dongmensql.h>

sql_stmt_create *sql_stmt_create_create(char *tableName,
                            arraylist *fieldsName, hmap_t columns,
                                        Constraint_t *constraints) {
    sql_stmt_create *sqlStmtCreate = (sql_stmt_create *)malloc(sizeof(sql_stmt_create));
    sqlStmtCreate->tableInfo =  table_info_create(tableName, fieldsName, columns);
    sqlStmtCreate->constraints = constraints;
    return sqlStmtCreate;
};


sql_stmt_create *parseCreate(ParserT *parser) { return NULL; };
/*
AlterStmt *parseAlter(ParserT *parser) { return NULL; };

DeleteStmt *parseDelete(ParserT *parser) { return NULL; };

UpdateStmt *parseUpdate(ParserT *parser) { return NULL; };
*/
/*
TermExpr *parseTermExpr(ParserT *parser) { return NULL; };

BinaryExpr *parseBinaryExpr(ParserT *parser) { return NULL; };

UnaryExpr *parseUnaryExpr(ParserT *parser) { return NULL; };

arraylist *parseGroupExpr(ParserT *parser) { return NULL; };
*/
arraylist *parseOrderExpr(ParserT *parser) { return NULL; };
/*
ColumnsExpr *parseColumnsExpr(ParserT *parser) { return NULL; };

SetExpr *parseSetExpr(ParserT *parser) { return NULL; };
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