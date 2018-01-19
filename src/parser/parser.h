//
// Created by Sam on 2018/1/17.
//

#ifndef DONGMENDB_PARSER_H
#define DONGMENDB_PARSER_H

#include "statement.h"
#include "tokenizer.h"
#include "expression.h"

SelectStmt *parseSelect(TokenizerT *tokenizer);
CreateStmt *parseCreate(TokenizerT *tokenizer);
AlterStmt *parseAlter(TokenizerT *tokenizer);
InsertStmt *parseInsert(TokenizerT *tokenizer);
DeleteStmt *parseDelete(TokenizerT *tokenizer);
UpdateStmt *parseUpdate(TokenizerT *tokenizer);

FieldsExpr *parseFieldsExpr(TokenizerT *tokenizer);
TablesExpr *parseTablesExpr(TokenizerT *tokenizer);

Func *parseFunc(TokenizerT *tokenizer);
TermExpr *parseTermExpr(TokenizerT *tokenizer);
BinaryExpr *parseBinaryExpr(TokenizerT *tokenizer);
UnaryExpr *parseUnaryExpr(TokenizerT *tokenizer);
GroupExpr *parseGroupExpr(TokenizerT *tokenizer);
OrderExpr *parseOrderExpr(TokenizerT *tokenizer);
ColumnsExpr *parseColumnsExpr(TokenizerT *tokenizer);
SetExpr *parseSetExpr(TokenizerT *tokenizer);


#endif //DONGMENDB_PARSER_H
