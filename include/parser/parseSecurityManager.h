//
// Created by Sam on 2018/2/15.
//

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


user *parse_create_user(ParserT *parser);

role *parse_create_role(ParserT *parser);

sql_stmt_grant_revoke *parse_grant(ParserT *parser);

sql_stmt_grant_revoke *parse_revoke(ParserT *parser);
