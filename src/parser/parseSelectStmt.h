//
// Created by Sam on 2018/1/23.
//

#ifndef DONGMENDB_PARSESELECTSTMT_H
#define DONGMENDB_PARSESELECTSTMT_H


#include "parser.h"
#include "dongmengdb.h"

SelectStmt *parseSelectStmt(ParserT *parser);

FieldsExpr *parseFieldsExpr(ParserT *parser);
TablesExpr *parseTablesExpr(ParserT *parser);

sql_stmt_create *parse_sql_stmt_create(ParserT *parser);
field_info *parse_sql_stmt_columnexpr(ParserT *parser);

#endif //DONGMENDB_PARSESELECTSTMT_H
