//
// Created by Sam on 2018/1/17.
//

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "parser.h"
#include "parseExpression.h"


SelectStmt *parseSelect(TokenizerT *tk) {
    FieldsExpr *fieldsExpr = parseFieldsExpr(tk);
    TokenT *token = TKGetNextToken(tk);
    if (strcmp(token->text, "from") != 0) {
        printf("语法错误. \n");
        exit(1);
    }
    TablesExpr *tablesExpr = parseTablesExpr(tk);
    token = TKGetNextToken(tk);
    if (token == NULL) {
        return createSelectStmt(fieldsExpr, tablesExpr, NULL, NULL, NULL);
    }
    if (strcmp(token->text, "where") != 0) {
        printf("语法错误. \n");
        exit(1);
    }
    Expression *whereExpr = parseExpression(tk);
    token = TKGetNextToken(tk);
    if (token == NULL) {
        return createSelectStmt(fieldsExpr, tablesExpr, whereExpr, NULL, NULL);
    }
    TokenT *tokenBy = TKGetNextToken(tk);
    if (strcmp(token->text, "group") != 0) {
        printf("语法错误. \n");
        exit(1);
    } else if (strcmp(tokenBy->text, "by") != 0) {
        printf("语法错误. \n");
        exit(1);
    }
    GroupExpr *groupExpr = parseGroupExpr(tk);
    token = TKGetNextToken(tk);
    if (token == NULL) {
        return createSelectStmt(fieldsExpr, tablesExpr, whereExpr, groupExpr, NULL);
    }
    tokenBy = TKGetNextToken(tk);
    if (strcmp(token->text, "order") != 0) {
        printf("语法错误. \n");
        exit(1);
    } else if (strcmp(tokenBy->text, "by") != 0) {
        printf("语法错误. \n");
        exit(1);
    }
    OrderExpr *orderExpr = parseOrderExpr(tk);
    token = TKGetNextToken(tk);
    if (token == NULL) {
        return createSelectStmt(fieldsExpr, tablesExpr, whereExpr, groupExpr, orderExpr);
    }

};

CreateStmt *parseCreate(TokenizerT *tk) {};

AlterStmt *parseAlter(TokenizerT *tk) {};

DeleteStmt *parseDelete(TokenizerT *tk) {};

UpdateStmt *parseUpdate(TokenizerT *tk) {};

FieldsExpr *parseFieldsExpr(TokenizerT *tk) {};

TablesExpr *parseTablesExpr(TokenizerT *tk) {};

Expression *parseExpression(TokenizerT *tk) {};

Func *parseFunc(TokenizerT *tk) {};

TermExpr *parseTermExpr(TokenizerT *tk) {};

BinaryExpr *parseBinaryExpr(TokenizerT *tk) {};

UnaryExpr *parseUnaryExpr(TokenizerT *tk) {};

GroupExpr *parseGroupExpr(TokenizerT *tk) {};

OrderExpr *parseOrderExpr(TokenizerT *tk) {};

ColumnsExpr *parseColumnsExpr(TokenizerT *tk) {};

SetExpr *parseSetExpr(TokenizerT *tk) {};

