//
// Created by Sam on 2018/1/17.
//

#ifndef DONGMENDB_PARSER_H
#define DONGMENDB_PARSER_H

#include <statement.h>
#include "tokenizer.h"
#include "expression.h"

/*
 * 定义parser的基础数据结构和共用的函数
 *
 * */

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


sql_stmt_create *parse_sql_stmt_create(ParserT *parser);
AlterStmt *parseAlter(ParserT *parser);
InsertStmt *parseInsert(ParserT *parser);
DeleteStmt *parseDelete(ParserT *parser);
UpdateStmt *parseUpdate(ParserT *parser);

TermExpr *parseTermExpr(ParserT *parser);
BinaryExpr *parseBinaryExpr(ParserT *parser);
UnaryExpr *parseUnaryExpr(ParserT *parser);
arraylist *parseGroupExpr(ParserT *parser);
arraylist *parseOrderExpr(ParserT *parser);
ColumnsExpr *parseColumnsExpr(ParserT *parser);
SetExpr *parseSetExpr(ParserT *parser);

int matchToken(ParserT *parser, TokenType type, char *text);

#endif //DONGMENDB_PARSER_H
