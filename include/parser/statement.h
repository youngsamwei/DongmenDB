//
// Created by Sam on 2018/1/17.
//

#ifndef DONGMENDB_STATEMENT_H
#define DONGMENDB_STATEMENT_H


#include "dongmendb/Recordfile.h"
#include "dongmendb/DongmenDB.h"
#include "dongmendb/SecurityManager.h"
#include "dongmensql/column.h"
#include <dongmensql/sra.h>
#include <dongmensql/sqlstatement.h>
#include "parser.h"


#include <vector>

using namespace std;

class sql_stmt_create  : public SQLStatement {
public:
    TableInfo *tableInfo;

    Constraint_t *constraints;

} ;

class sql_stmt_insert : public SQLStatement{
public:
    char *tableName;
    vector<char*> fields;
    vector<variant*> values;
} ;

class sql_stmt_update  : public SQLStatement {
public:
    char *tableName;
    vector<char*> fields; //set fields 被更新的字段列表
    vector<Expression*> fieldsExpr;  //set fields expression 新值(表达式)列表
    SRA_t *where;
} ;

class sql_stmt_delete  : public SQLStatement{
public:
    char *tableName;
    SRA_t *where;
} ;

typedef enum {
    GRANT_REVOKE_TYPE_ROLE_USER,  //角色与用户
    GRANT_REVOKE_TYPE_RIGHT_USER, //权限与用户
    GRANT_REVOKE_TYPE_RIGHT_ROLE  //权限与角色
}grant_revoke_type;

class sql_stmt_grant_revoke  : public SQLStatement{
public:
    sql_stmt_type sqlStmtType;   //SQL_STMT_GRANT, SQL_STMT_REVOKE
    grant_revoke_type grantType;
    vector<Role*> roles;
    vector<User*> users;
    vector<Right*> *rights;
};

User *parse_create_user(Parser *parser);

Role *parse_create_role(Parser *parser);

sql_stmt_grant_revoke *parse_grant(Parser *parser);

sql_stmt_grant_revoke *parse_revoke(Parser *parser);


/* 打印 sql_stmt_update */
int sql_stmt_update_print(sql_stmt_update *sqlStmtUpdate);



#endif //DONGMENDB_STATEMENT_H
