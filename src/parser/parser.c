//
// Created by Sam on 2018/1/17.
//

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "parser.h"
#include "parseExpression.h"
#include "parseExpressionRD.h"

/**
 * @brief 创建语法解析器
 * @param tokenizer 词法分析器
 * @return 解析器
 */
ParserT *newParser(TokenizerT *tokenizer) {
    ParserT *parser = (ParserT *) malloc(sizeof(ParserT));
    parser->tokenizer = tokenizer;
    parser->currToken = NULL;
    parser->parserStateType = PARSER_CORRECT;
    return parser;
};

/**
 * @brief 获取下一个token，如果当前的token不为NULL，则不获取下一个token。
 * 如果解析出错则返回NULL,停止解析
 * @param parser 解析器
 * @return Token
 */
TokenT *parseNextToken(ParserT *parser) {
    if (parser->parserStateType==PARSER_WRONG){
        return NULL;
    }
    if (parser->currToken == NULL) {
        parser->currToken = TKGetNextToken(parser->tokenizer);
    }
    return parser->currToken;
};

/**
 * @brief 处理完一个token后，需要调用此方法把token销毁。
 * @param parser  解析器
 * @return  NULL
 */
TokenT *parseEatToken(ParserT *parser) {
    if (parser->parserStateType==PARSER_WRONG){
        return NULL;
    }
    parser->currToken = NULL;
    return NULL;
};

/**
 * @brief eat掉当前的token，并获取下一个token
 * @param parser
 * @return
 */
TokenT *parseEatAndNextToken(ParserT *parser) {
    if (parser->parserStateType==PARSER_WRONG){
        return NULL;
    }
    parser->currToken = TKGetNextToken(parser->tokenizer);
    return parser->currToken;
};

/**
 * @brief 产生解析错误
 * @param parser 解析器
 * @param message 错误消息
 * @return NULL
 */
void *parseError(ParserT *parser, char *message) {
    strcpy(parser->parserMessage, message);
    parser->parserStateType = PARSER_WRONG;
    return NULL;
};

/**
 * @brief 解析select语句
 * @param parser 解析器
 * @return select语句
 */
SelectStmt *parseSelect(ParserT *parser) {
    TokenT *token = parseNextToken(parser);
    if (stricmp(token->text, "select") ==0){
        token = parseEatAndNextToken(parser);
    }else{
        strcpy(parser->parserMessage , "语法错误.");
        return NULL;
    }
    FieldsExpr *fieldsExpr = parseFieldsExpr(parser);
    token = parseNextToken(parser);
    if (stricmp(token->text, "from") != 0) {
        strcpy(parser->parserMessage , "语法错误.");
        return NULL;
    }else{
        token = parseEatAndNextToken(parser);
    }
    TablesExpr *tablesExpr = parseTablesExpr(parser);
    token = parseNextToken(parser);
    if (token == NULL || token->type ==TOKEN_SEMICOLON) {
        return createSelectStmt(fieldsExpr, tablesExpr, NULL, NULL, NULL);
    }
    if (stricmp(token->text, "where") != 0) {
        strcpy(parser->parserMessage , "语法错误.");
        return NULL;
    }
    Expression *whereExpr = parseExpressionRD(parser);
    token = parseNextToken(parser);;
    if (token == NULL) {
        return createSelectStmt(fieldsExpr, tablesExpr, whereExpr, NULL, NULL);
    }
    TokenT *tokenBy = parseNextToken(parser);;
    if (stricmp(token->text, "group") != 0) {
        strcpy(parser->parserMessage , "语法错误.");
        return NULL;
    } else if (stricmp(tokenBy->text, "by") != 0) {
        strcpy(parser->parserMessage , "语法错误.");
        return NULL;
    }
    GroupExpr *groupExpr = parseGroupExpr(parser);
    token = parseNextToken(parser);;
    if (token == NULL) {
        return createSelectStmt(fieldsExpr, tablesExpr, whereExpr, groupExpr, NULL);
    }
    tokenBy = parseNextToken(parser);
    if (stricmp(token->text, "order") != 0) {
        strcpy(parser->parserMessage , "语法错误.");
        return NULL;
    } else if (stricmp(tokenBy->text, "by") != 0) {
        strcpy(parser->parserMessage , "语法错误.");
        return NULL;
    }
    OrderExpr *orderExpr = parseOrderExpr(parser);
    token = parseNextToken(parser);;
    if (token == NULL) {
        return createSelectStmt(fieldsExpr, tablesExpr, whereExpr, groupExpr, orderExpr);
    }else{
        return NULL;
    }

};

CreateStmt *parseCreate(ParserT *parser) {return NULL;};

AlterStmt *parseAlter(ParserT *parser) {return NULL;};

DeleteStmt *parseDelete(ParserT *parser) {return NULL;};

UpdateStmt *parseUpdate(ParserT *parser) {return NULL;};

FieldsExpr *parseFieldsExpr(ParserT *parser) {
    Expression *expr0 = parseExpressionRD(parser);
    FieldsExpr *fieldsExpr = (FieldsExpr *)malloc(sizeof(FieldsExpr));
    fieldsExpr->alias=NULL;
    fieldsExpr->nextField = NULL;
    fieldsExpr->expr = expr0;
    return fieldsExpr;
};

TablesExpr *parseTablesExpr(ParserT *parser) {
    TokenT *token = parseNextToken(parser);
    if (token->type==TOKEN_WORD){
        parseEatAndNextToken(parser);
        TablesExpr *tablesExpr = (TablesExpr *)malloc(sizeof(TablesExpr));
        tablesExpr->db=NULL;
        tablesExpr->joinExpr=NULL;
        tablesExpr->name = token->text;
        tablesExpr->nextTable = NULL;
        tablesExpr->schema = NULL;
        return tablesExpr;
    }

    return NULL;
};

TermExpr *parseTermExpr(ParserT *parser) {return NULL;};

BinaryExpr *parseBinaryExpr(ParserT *parser) {return NULL;};

UnaryExpr *parseUnaryExpr(ParserT *parser) {return NULL;};

GroupExpr *parseGroupExpr(ParserT *parser) {return NULL;};

OrderExpr *parseOrderExpr(ParserT *parser) {return NULL;};

ColumnsExpr *parseColumnsExpr(ParserT *parser) {return NULL;};

SetExpr *parseSetExpr(ParserT *parser) {return NULL;};

