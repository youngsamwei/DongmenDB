//
// Created by Sam on 2018/1/17.
//

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "parser.h"
#include "parseExpression.h"
#include "parseExpressionRD.h"

ParserT * newParser(TokenizerT *tokenizer){
    ParserT *parser = (ParserT *)malloc(sizeof(ParserT));
    parser->tokenizer = tokenizer;
    parser->currToken = NULL;
    parser->parserStateType = PARSER_CORRECT;
    return parser;
};

TokenT *parseNextToken(ParserT *parser){
    parser->currToken = TKGetNextToken(parser->tokenizer);
    return parser->currToken;
};

SelectStmt *parseSelect(ParserT *parser) {
    FieldsExpr *fieldsExpr = parseFieldsExpr(parser);
    TokenT *token = parseNextToken(parser);
    if (stricmp(token->text, "from") != 0) {
        parser->parserMessage = "语法错误.";
        return NULL;
    }
    TablesExpr *tablesExpr = parseTablesExpr(parser);
    token = parseNextToken(parser);;
    if (token == NULL) {
        return createSelectStmt(fieldsExpr, tablesExpr, NULL, NULL, NULL);
    }
    if (stricmp(token->text, "where") != 0) {
        parser->parserMessage = "语法错误.";
        return NULL;
    }
    Expression *whereExpr = parseExpressionRD(parser);
    token = parseNextToken(parser);;
    if (token == NULL) {
        return createSelectStmt(fieldsExpr, tablesExpr, whereExpr, NULL, NULL);
    }
    TokenT *tokenBy = parseNextToken(parser);;
    if (stricmp(token->text, "group") != 0) {
        parser->parserMessage = "语法错误.";
        return NULL;
    } else if (stricmp(tokenBy->text, "by") != 0) {
        parser->parserMessage = "语法错误.";
        return NULL;
    }
    GroupExpr *groupExpr = parseGroupExpr(parser);
    token = parseNextToken(parser);;
    if (token == NULL) {
        return createSelectStmt(fieldsExpr, tablesExpr, whereExpr, groupExpr, NULL);
    }
    tokenBy = parseNextToken(parser);
    if (stricmp(token->text, "order") != 0) {
        parser->parserMessage = "语法错误.";
        return NULL;
    } else if (stricmp(tokenBy->text, "by") != 0) {
        parser->parserMessage = "语法错误.";
        return NULL;
    }
    OrderExpr *orderExpr = parseOrderExpr(parser);
    token = parseNextToken(parser);;
    if (token == NULL) {
        return createSelectStmt(fieldsExpr, tablesExpr, whereExpr, groupExpr, orderExpr);
    }

};

CreateStmt *parseCreate(ParserT *parser) {};

AlterStmt *parseAlter(ParserT *parser) {};

DeleteStmt *parseDelete(ParserT *parser) {};

UpdateStmt *parseUpdate(ParserT *parser) {};

FieldsExpr *parseFieldsExpr(ParserT *parser) {};

TablesExpr *parseTablesExpr(ParserT *parser) {};

TermExpr *parseTermExpr(ParserT *parser) {};

BinaryExpr *parseBinaryExpr(ParserT *parser) {};

UnaryExpr *parseUnaryExpr(ParserT *parser) {};

GroupExpr *parseGroupExpr(ParserT *parser) {};

OrderExpr *parseOrderExpr(ParserT *parser) {};

ColumnsExpr *parseColumnsExpr(ParserT *parser) {};

SetExpr *parseSetExpr(ParserT *parser) {};

