//
// Created by Sam on 2018/1/17.
//

#ifndef DONGMENDB_STATEMENT_H
#define DONGMENDB_STATEMENT_H

#include <utils/arraylist.h>
#include "dongmendb/recordfile.h"
#include "dongmendb/dongmendb.h"
#include "dongmendb/securitymanager.h"
#include "dongmensql/column.h"
#include <dongmensql/sra.h>
#include "parser.h"

#ifdef __cplusplus
extern "C" {
#endif
typedef struct ParserT_  ParserT;
typedef struct sql_stmt_create_ {
    table_info *tableInfo;

    Constraint_t *constraints;

} sql_stmt_create;

typedef struct sql_stmt_insert_ {
    char *tableName;
    arraylist *fields;
    arraylist *values;
} sql_stmt_insert;

typedef struct sql_stmt_update_ {
    char *tableName;
    arraylist *fields; //set fields 被更新的字段列表
    arraylist *fieldsExpr;  //set fields expression 新值(表达式)列表
    SRA_t *where;
} sql_stmt_update;

typedef struct sql_stmt_delete_{
    char *tableName;
    SRA_t *where;
} sql_stmt_delete;

typedef enum {
    GRANT_REVOKE_TYPE_ROLE_USER,  //角色与用户
    GRANT_REVOKE_TYPE_RIGHT_USER, //权限与用户
    GRANT_REVOKE_TYPE_RIGHT_ROLE  //权限与角色
}grant_revoke_type;

typedef struct sql_stmt_grant_{
    sql_stmt_type sqlStmtType;   //SQL_STMT_GRANT, SQL_STMT_REVOKE
    grant_revoke_type grantType;
    arraylist *roles;
    arraylist *users;
    arraylist *rights;
}sql_stmt_grant_revoke;


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

sql_stmt_create *parse_sql_stmt_create(ParserT *parser);
/*
AlterStmt *parseAlter(ParserT *parser);
InsertStmt *parseInsert(ParserT *parser);
DeleteStmt *parseDelete(ParserT *parser);
UpdateStmt *parseUpdate(ParserT *parser);
*/
/*
TermExpr *parseTermExpr(ParserT *parser);
BinaryExpr *parseBinaryExpr(ParserT *parser);
UnaryExpr *parseUnaryExpr(ParserT *parser);
arraylist *parseGroupExpr(ParserT *parser);
 */
arraylist *parseOrderExpr(ParserT *parser);
/*
ColumnsExpr *parseColumnsExpr(ParserT *parser);
SetExpr *parseSetExpr(ParserT *parser);
*/

/* 在src_experiment\exp_01_stmt_parser\exp_01_03_select.c 中实现*/
SRA_t *parse_sql_stmt_select(ParserT *parser);

arraylist *parseFieldsExpr(ParserT *parser);
SRA_t *parseTablesExpr(ParserT *parser);

sql_stmt_create *parse_sql_stmt_create(ParserT *parser);
field_info *parse_sql_stmt_columnexpr(ParserT *parser);

/*  在src_experiment\exp_01_stmt_parser\exp_01_02_insert.c 中实现*/;
sql_stmt_insert *parse_sql_stmt_insert(ParserT *parser);

/* 在src_experiment\exp_01_stmt_parser\exp_01_04_update.c 中实现*/
sql_stmt_update *parse_sql_stmt_update(ParserT *parser);

/* 在src_experiment\exp_01_stmt_parser\exp_01_05_delete.c 中实现*/
sql_stmt_delete *parse_sql_stmt_delete(ParserT *parser);

user *parse_create_user(ParserT *parser);

role *parse_create_role(ParserT *parser);

sql_stmt_grant_revoke *parse_grant(ParserT *parser);

sql_stmt_grant_revoke *parse_revoke(ParserT *parser);


/* 打印 sql_stmt_update */
int sql_stmt_update_print(sql_stmt_update *sqlStmtUpdate);

#ifdef __cplusplus
}
#endif

#endif //DONGMENDB_STATEMENT_H
