//
// Created by Sam on 2018/2/13.
//

#include <parser/statement.h>

/**
 * 在现有实现基础上，实现update from子句
 *
 * 支持的update语法：
 *
 * UPDATE <table_name> SET <field1> = <expr1>[, <field2 = <expr2>, ..]
 * WHERE <logical_expr>
 *
 * 解析获得 sql_stmt_update 结构
 */

/*TODO: parse_sql_stmt_update， update语句解析*/
sql_stmt_update *parse_sql_stmt_update(ParserT *parser) {
//    fprintf(stderr, "TODO: update is not implemented yet. in parse_sql_stmt_update \n");

};