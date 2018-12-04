//
// Created by sam on 2018/12/4.
//

#ifndef DONGMENDB_SQLSTATEMENT_H
#define DONGMENDB_SQLSTATEMENT_H

#include <dongmensql/column.h>
#include <dongmensql/sra.h>
#include <dongmendb/Recordfile.h>
#include <dongmendb/SecurityManager.h>

#include <vector>
using namespace std;

#define SQL_NOTVALID (-1)
#define SQL_NULL (0)
#define SQL_INTEGER_1BYTE (1)
#define SQL_INTEGER_2BYTE (2)
#define SQL_INTEGER_4BYTE (4)
#define SQL_TEXT (13)

#define STMT_CREATE (0)
#define STMT_SELECT (1)
#define STMT_INSERT (2)
#define STMT_DELETE (3)

typedef struct Constraint_s Constraint_t;
typedef struct SRA_s SRA_t;
class Expression;

class SQLStatement {
public:
    /*sql文本*/
    char *text;
    sql_statement_type type;

    SQLStatement(char *text, sql_statement_type type){
        this->text = strdup(text);
        this->type = type;
    }
};


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
    sql_statement_type sqlStmtType;   //SQL_STMT_GRANT, SQL_STMT_REVOKE
    grant_revoke_type grantType;
    vector<Role*> roles;
    vector<User*> users;
    vector<Right*> *rights;
};

/* 打印 sql_stmt_update */
int sql_stmt_update_print(sql_stmt_update *sqlStmtUpdate);


#endif //DONGMENDB_SQLSTATEMENT_H
