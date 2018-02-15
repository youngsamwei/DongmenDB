//
// Created by Sam on 2018/2/15.
//

#include <parser/parser.h>
#include <dongmendb/securitymanager.h>
#include "parser/parseSecurityManager.h"
/**
 * 实现以下语句的解析：
 * 1）create user | role ,
 * 2）grant right on table  to user
 * 3）grant right on table to role
 * 4) grant role to user
 * 5）revoke right on table from user
 * 6）revoke right on table from role
 * 7) revoke role from user
 *
 */


user *parse_create_user(ParserT *parser){

};

role *parse_create_role(ParserT *parser){

};

sql_stmt_grant_revoke *parse_grant(ParserT *parser){

};

sql_stmt_grant_revoke *parse_revoke(ParserT *parser){

};
