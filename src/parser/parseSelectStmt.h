//
// Created by Sam on 2018/1/23.
//

#ifndef DONGMENDB_PARSESELECTSTMT_H
#define DONGMENDB_PARSESELECTSTMT_H


#include "parser.h"
#include "dongmendb.h"

SRA_t *parse_sql_stmt_select(ParserT *parser);

arraylist *parseFieldsExpr(ParserT *parser);
SRA_t *parseTablesExpr(ParserT *parser);

sql_stmt_create *parse_sql_stmt_create(ParserT *parser);
field_info *parse_sql_stmt_columnexpr(ParserT *parser);

sql_stmt_insert *parse_sql_stmt_insert(ParserT *parser);

#endif //DONGMENDB_PARSESELECTSTMT_H
