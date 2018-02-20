//
// Created by Sam on 2018/1/17.
//

#ifndef DONGMENDB_PARSER_H
#define DONGMENDB_PARSER_H

#include "tokenizer.h"

/*
 * 定义parser的基础数据结构和共用的函数
 *
 * */
#ifdef __cplusplus
extern "C" {
#endif

#define PARSER_MESSAGE_LENTTH 128

typedef enum{
    SQL_STMT_SELECT,
    SQL_STMT_INSERT,
    SQL_STMT_UPDATE,
    SQL_STMT_DELETE,
    SQL_STMT_CREATE_TABLE,
    SQL_STMT_CREATE_USER,
    SQL_STMT_CREATE_ROLE,
    SQL_STMT_DROP_TABLE,
    SQL_STMT_DROP_USER,
    SQL_STMT_DROP_ROLE,
    SQL_STMT_GRANT,
    SQL_STMT_REVOKE
}sql_stmt_type;

/*语法分析状态类型*/
typedef enum {
    PARSER_WRONG,
    PARSER_CORRECT,
    PARSER_RESERVED_WORD
} ParserStateType;

typedef struct ParserT_ {

    TokenizerT *tokenizer;
    TokenT *currToken;
    ParserStateType parserStateType;
    char parserMessage[PARSER_MESSAGE_LENTTH];
} ParserT;


ParserT *newParser(TokenizerT *tokenizer);
TokenT *parseNextToken(ParserT *parser);
TokenT *parseEatToken(ParserT *parser);
TokenT *parseEatAndNextToken(ParserT *parser);
void *parseError(ParserT *parser, char *message);

int matchToken(ParserT *parser, TokenType type, char *text);

#ifdef __cplusplus
}
#endif

#endif //DONGMENDB_PARSER_H
