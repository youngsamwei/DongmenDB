//
// Created by Sam on 2018/2/15.
//

#include <parser/parser.h>
#include <dongmendb/securitymanager.h>
#include <parser/statement.h>
/**
 * 安全性管理实验需要实现：
 * 1. 安全性定义：语句解析
 * 2. 安全性存储管理：系统数据表结构
 * 3. 权限验证：当select，update，delete，insert时检查该用户是否有相应的权限
 * 4. 处理：合法则放行，违法则拒绝。
 *
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
