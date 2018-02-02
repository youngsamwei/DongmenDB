//
// Created by Sam on 2018/1/23.
//

#include <stddef.h>
#include <malloc.h>
#include <mem.h>
#include <stdio.h>
#include "parseSelectStmt.h"
#include "parseExpressionRD.h"


/** 17096052888
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
    if (parser->parserStateType == PARSER_WRONG) {
        return NULL;
    }
    token = parseNextToken(parser);
    if (stricmp(token->text, "from") != 0) {
        strcpy(parser->parserMessage, "语法错误.");
        return NULL;
    } else {
        token = parseEatAndNextToken(parser);
    }
    TablesExpr *tablesExpr = parseTablesExpr(parser);
    if (parser->parserStateType == PARSER_WRONG) {
        return NULL;
    }
    token = parseNextToken(parser);
    if (token == NULL || token->type == TOKEN_SEMICOLON) {
        return createSelectStmt(fieldsExpr, tablesExpr, NULL, NULL, NULL);
    }
    if (stricmp(token->text, "where") != 0) {
        strcpy(parser->parserMessage, "语法错误.");
        return NULL;
    } else {
        token = parseEatAndNextToken(parser);
    }
    Expression *whereExpr = parseExpressionRD(parser);
    if (parser->parserStateType == PARSER_WRONG) {
        return NULL;
    }
    token = parseNextToken(parser);;
    if (token == NULL || token->type == TOKEN_SEMICOLON) {
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
    if (parser->parserStateType == PARSER_WRONG) {
        return NULL;
    }
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
    if (parser->parserStateType == PARSER_WRONG) {
        return NULL;
    }
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
        while (token != NULL && token->type == TOKEN_COMMA) {
            token = parseEatAndNextToken(parser);/*跳过comma*/
            TablesExpr *tablesExpr1 = (TablesExpr *) malloc(sizeof(TablesExpr));
            tablesExpr1->db = NULL;
            tablesExpr1->joinExpr = NULL;
            tablesExpr1->name = token->text;
            tablesExpr1->nextTable = tablesExpr;
            tablesExpr1->schema = NULL;
            tablesExpr = tablesExpr1;
            token = parseEatAndNextToken(parser);
        }

        return tablesExpr;
    }

    return NULL;
};

sql_stmt_create *parse_sql_stmt_create(ParserT *parser) {
    char *tableName = NULL;
    arraylist *columns = arraylist_create();
    if (!matchToken(parser, TOKEN_RESERVED_WORD, "create")) {
        return NULL;
    }
    if (!matchToken(parser, TOKEN_RESERVED_WORD, "table")) {
        strcpy(parser->parserMessage, "invalid sql: should be table.");
        return NULL;
    }
    TokenT *token = parseNextToken(parser);
    if (token->type == TOKEN_WORD) {
        tableName = (char *) calloc(MAX_ID_NAME_LENGTH, 1);
        strcpy(tableName, token->text);
    } else {
        strcpy(parser->parserMessage, "invalid sql: missing table name.");
        return NULL;
    }
    token = parseEatAndNextToken(parser);
    if (!matchToken(parser, TOKEN_OPEN_PAREN, "(")) {
        strcpy(parser->parserMessage, "invalid sql: missing (.");
        return NULL;
    }
    token = parseNextToken(parser);

    while (token->type != TOKEN_CLOSE_PAREN) {
        ColumnsExpr *column = parse_sql_stmt_columnexpr(parser);
        if (column == NULL) {
            break;
        } else {
            arraylist_add(columns, column);
        }
        token = parseNextToken(parser);
        if (token->type==TOKEN_COMMA){
            token = parseEatAndNextToken(parser);
        } else {
            break;
        }
    }
    token = parseNextToken(parser);
    if (!matchToken(parser, TOKEN_CLOSE_PAREN, ")")) {
        strcpy(parser->parserMessage, "invalid sql: missing ).");
        return NULL;
    }
    return sql_stmt_create_create(tableName, columns, NULL);
};

ColumnsExpr *parse_sql_stmt_columnexpr(ParserT *parser) {
    TokenT *token = parseNextToken(parser);
    char *columnName = NULL;
    DATA_TYPE type;
    int length;
    if (token->type == TOKEN_WORD) {
        columnName = (char *) calloc(MAX_ID_NAME_LENGTH, 1);
        strcpy(columnName, token->text);
    } else {
        strcpy(parser->parserMessage, "invalid sql: missing field name.");
        return NULL;
    }
    token = parseEatAndNextToken(parser);
    if (token->type == TOKEN_RESERVED_WORD) {
        if (stricmp(token->text, "int") == 0 || stricmp(token->text, "integer") == 0) {
            type = DATA_TYPE_INT;
            length = INT_SIZE;
            token = parseEatAndNextToken(parser);
        } else if (stricmp(token->text, "char") == 0) {
            type = DATA_TYPE_CHAR;
            token = parseEatAndNextToken(parser);
            if (matchToken(parser, TOKEN_OPEN_PAREN, "(")) {
                token = parseNextToken(parser);
                if (token->type == TOKEN_DECIMAL) {
                    length = atoi(token->text);
                    token = parseEatAndNextToken(parser);
                    if(matchToken(parser, TOKEN_CLOSE_PAREN, ")")){
                        token = parseNextToken(parser);
                    }else{
                        strcpy(parser->parserMessage, "invalid sql: missing ).");
                        return NULL;
                    }
                } else {
                    strcpy(parser->parserMessage, "invalid sql: missing char length.");
                    return NULL;
                }
            } else {
                strcpy(parser->parserMessage, "invalid sql: missing char length.");
                return NULL;
            }
        } else {
            strcpy(parser->parserMessage, "invalid sql: wrong data type : ");
            strcat(parser->parserMessage, token->text);
            return NULL;
        }
    } else {
        strcpy(parser->parserMessage, "invalid sql : missing field name.");
        return NULL;
    }
    ColumnsExpr *column = (ColumnsExpr *) malloc(sizeof(ColumnsExpr));
    column->type = type;
    column->columnName = columnName;
    column->length = length;
    column->constraints = NULL;
    return column;
};