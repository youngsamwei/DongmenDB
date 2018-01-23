//
// Created by Sam on 2018/1/23.
//

#ifndef DONGMENDB_PARSESELECTSTMT_H
#define DONGMENDB_PARSESELECTSTMT_H


#include "parser.h"

SelectStmt *parseSelectStmt(ParserT *parser);

FieldsExpr *parseFieldsExpr(ParserT *parser);
TablesExpr *parseTablesExpr(ParserT *parser);


#endif //DONGMENDB_PARSESELECTSTMT_H
