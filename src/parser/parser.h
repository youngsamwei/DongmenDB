//
// Created by Sam on 2018/1/17.
//

#ifndef DONGMENDB_PARSER_H
#define DONGMENDB_PARSER_H

#include "statement.h"
#include "tokenizer.h"
#include "expression.h"

#define PARSER_MESSAGE_LENTTH 128

/*语法分析状态类型*/
typedef enum {
    PARSER_WRONG,
    PARSER_CORRECT,
    PARSER_RESERVED_WORD
} ParserStateType;

typedef struct ParserT_{

    TokenizerT *tokenizer;
    TokenT *currToken;
    ParserStateType parserStateType;
    char parserMessage[PARSER_MESSAGE_LENTTH];
} ParserT;


ParserT * newParser(TokenizerT *tokenizer);
TokenT *parseNextToken(ParserT *parser);
TokenT *parseEatToken(ParserT *parser);
TokenT *parseEatAndNextToken(ParserT *parser);
void *parseError(ParserT *parser, char *message);

SelectStmt *parseSelect(ParserT *parser);
CreateStmt *parseCreate(ParserT *parser);
AlterStmt *parseAlter(ParserT *parser);
InsertStmt *parseInsert(ParserT *parser);
DeleteStmt *parseDelete(ParserT *parser);
UpdateStmt *parseUpdate(ParserT *parser);

FieldsExpr *parseFieldsExpr(ParserT *parser);
TablesExpr *parseTablesExpr(ParserT *parser);

TermExpr *parseTermExpr(ParserT *parser);
BinaryExpr *parseBinaryExpr(ParserT *parser);
UnaryExpr *parseUnaryExpr(ParserT *parser);
GroupExpr *parseGroupExpr(ParserT *parser);
OrderExpr *parseOrderExpr(ParserT *parser);
ColumnsExpr *parseColumnsExpr(ParserT *parser);
SetExpr *parseSetExpr(ParserT *parser);


#endif //DONGMENDB_PARSER_H
