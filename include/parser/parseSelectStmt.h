//
// Created by Sam on 2018/1/23.
//

#ifndef DONGMENDB_PARSESELECTSTMT_H
#define DONGMENDB_PARSESELECTSTMT_H


#include <dongmensql/sra.h>
#include "parser.h"
#include "dongmendb/dongmendb.h"
#ifdef __cplusplus
extern "C" {
#endif

SRA_t *parse_sql_stmt_select(ParserT *parser);

arraylist *parseFieldsExpr(ParserT *parser);
SRA_t *parseTablesExpr(ParserT *parser);

sql_stmt_create *parse_sql_stmt_create(ParserT *parser);
field_info *parse_sql_stmt_columnexpr(ParserT *parser);

sql_stmt_insert *parse_sql_stmt_insert(ParserT *parser);
#ifdef __cplusplus
}
#endif

#endif //DONGMENDB_PARSESELECTSTMT_H
