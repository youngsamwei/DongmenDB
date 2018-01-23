//
// Created by Sam on 2018/1/23.
//

#include <stddef.h>
#include <malloc.h>
#include <mem.h>
#include "parseSelectStmt.h"
#include "parseExpressionRD.h"


/**
 * @brief 解析select语句
 * @param parser 解析器
 * @return select语句
 */
SelectStmt *parseSelectStmt(ParserT *parser) {
    TokenT *token = parseNextToken(parser);
    if (stricmp(token->text, "select") == 0) {
        token = parseEatAndNextToken(parser);
    } else {
        strcpy(parser->parserMessage, "语法错误.");
        return NULL;
    }
    FieldsExpr *fieldsExpr = parseFieldsExpr(parser);
    token = parseNextToken(parser);
    if (stricmp(token->text, "from") != 0) {
        strcpy(parser->parserMessage, "语法错误.");
        return NULL;
    } else {
        token = parseEatAndNextToken(parser);
    }
    TablesExpr *tablesExpr = parseTablesExpr(parser);
    token = parseNextToken(parser);
    if (token == NULL || token->type == TOKEN_SEMICOLON) {
        return createSelectStmt(fieldsExpr, tablesExpr, NULL, NULL, NULL);
    }
    if (stricmp(token->text, "where") != 0) {
        strcpy(parser->parserMessage, "语法错误.");
        return NULL;
    }
    Expression *whereExpr = parseExpressionRD(parser);
    token = parseNextToken(parser);;
    if (token == NULL) {
        return createSelectStmt(fieldsExpr, tablesExpr, whereExpr, NULL, NULL);
    }
    TokenT *tokenBy = parseNextToken(parser);;
    if (stricmp(token->text, "group") != 0) {
        strcpy(parser->parserMessage, "语法错误.");
        return NULL;
    } else if (stricmp(tokenBy->text, "by") != 0) {
        strcpy(parser->parserMessage, "语法错误.");
        return NULL;
    }
    GroupExpr *groupExpr = parseGroupExpr(parser);
    token = parseNextToken(parser);;
    if (token == NULL) {
        return createSelectStmt(fieldsExpr, tablesExpr, whereExpr, groupExpr, NULL);
    }
    tokenBy = parseNextToken(parser);
    if (stricmp(token->text, "order") != 0) {
        strcpy(parser->parserMessage, "语法错误.");
        return NULL;
    } else if (stricmp(tokenBy->text, "by") != 0) {
        strcpy(parser->parserMessage, "语法错误.");
        return NULL;
    }
    OrderExpr *orderExpr = parseOrderExpr(parser);
    token = parseNextToken(parser);;
    if (token == NULL) {
        return createSelectStmt(fieldsExpr, tablesExpr, whereExpr, groupExpr, orderExpr);
    } else {
        return NULL;
    }

};



FieldsExpr *parseFieldsExpr(ParserT *parser) {
    Expression *expr0;
    expr0 = parseExpressionRD(parser);

    FieldsExpr *fieldsExpr0 = (FieldsExpr *) malloc(sizeof(FieldsExpr));
    fieldsExpr0->alias = NULL;
    fieldsExpr0->nextField = NULL;
    fieldsExpr0->expr = expr0;

    TokenT *token = parseNextToken(parser);
    while (token != NULL && token->type == TOKEN_COMMA) {
        parseEatAndNextToken(parser);
        Expression *expr1 = parseExpressionRD(parser);
        FieldsExpr *fieldsExpr1 = (FieldsExpr *) malloc(sizeof(FieldsExpr));
        fieldsExpr1->alias = NULL;
        fieldsExpr1->expr = expr1;
        fieldsExpr1->nextField = fieldsExpr0;
        fieldsExpr0 = fieldsExpr1;
        token = parseNextToken(parser);
    }

    return fieldsExpr0;
};

TablesExpr *parseTablesExpr(ParserT *parser) {
    TokenT *token = parseNextToken(parser);
    if (token->type == TOKEN_WORD) {

        TablesExpr *tablesExpr = (TablesExpr *) malloc(sizeof(TablesExpr));
        tablesExpr->db = NULL;
        tablesExpr->joinExpr = NULL;
        tablesExpr->name = token->text;
        tablesExpr->nextTable = NULL;
        tablesExpr->schema = NULL;

        TokenT *token = parseEatAndNextToken(parser);
        while (token != NULL && token->type ==TOKEN_COMMA){
            token = parseEatAndNextToken(parser);/*跳过comma*/
            TablesExpr *tablesExpr1 = (TablesExpr *) malloc(sizeof(TablesExpr));
            tablesExpr1->db = NULL;
            tablesExpr1->joinExpr = NULL;
            tablesExpr1->name = token->text;
            tablesExpr1->nextTable = tablesExpr;
            tablesExpr1->schema = NULL;
            tablesExpr= tablesExpr1;
            token = parseEatAndNextToken(parser);
        }

        return tablesExpr;
    }

    return NULL;
};
