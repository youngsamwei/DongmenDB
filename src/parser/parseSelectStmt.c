//
// Created by Sam on 2018/1/23.
//

#include <stddef.h>
#include <malloc.h>
#include <mem.h>
#include <stdio.h>
#include <utils.h>
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
    hmap_t columns = hashmap_create();
    arraylist *fieldsName = arraylist_create();
    if (!matchToken(parser, TOKEN_RESERVED_WORD, "create")) {
        return NULL;
    }
    if (!matchToken(parser, TOKEN_RESERVED_WORD, "table")) {
        strcpy(parser->parserMessage, "invalid sql: should be table.");
        return NULL;
    }
    TokenT *token = parseNextToken(parser);
    if (token->type == TOKEN_WORD) {
        tableName = new_id_name();
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
        field_info *field = parse_sql_stmt_columnexpr(parser);
        if (field == NULL) {
            break;
        } else {
            hashmap_put(columns, field->fieldName, field);
            arraylist_add(fieldsName, field->fieldName);
        }
        token = parseNextToken(parser);
        if (token->type == TOKEN_COMMA) {
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

    return sql_stmt_create_create(tableName, fieldsName, columns, NULL);
};

field_info *parse_sql_stmt_columnexpr(ParserT *parser) {
    TokenT *token = parseNextToken(parser);
    char *columnName = NULL;
    DATA_TYPE type;
    int length;
    if (token->type == TOKEN_WORD) {
        columnName = new_id_name();
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
                    if (matchToken(parser, TOKEN_CLOSE_PAREN, ")")) {
                        token = parseNextToken(parser);
                    } else {
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
    field_info *field = (field_info *) calloc(sizeof(field_info), 1);
    field->type = type;
    field->length = length;
    field->fieldName = columnName;
    return field;
};

sql_stmt_insert *parse_sql_stmt_insert(ParserT *parser) {
    char *tableName = NULL;
    arraylist *fields = arraylist_create();
    arraylist *values = arraylist_create();

    if (!matchToken(parser, TOKEN_RESERVED_WORD, "insert")) {
        return NULL;
    }
    if (!matchToken(parser, TOKEN_RESERVED_WORD, "into")) {
        strcpy(parser->parserMessage, "invalid sql: should be into.");
        return NULL;
    }
    TokenT *token = parseNextToken(parser);
    if (token->type == TOKEN_WORD) {
        tableName = new_id_name();
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
    if (token->type == TOKEN_WORD) {
        while (token->type == TOKEN_WORD) {
            char *fieldName = new_id_name();
            strcpy(fieldName, token->text);
            arraylist_add(fields, fieldName);
            token = parseEatAndNextToken(parser);
            if (token->type==TOKEN_COMMA){
                token = parseEatAndNextToken(parser);
            }else{
                break;
            }
        }
    } else {
        strcpy(parser->parserMessage, "invalid sql: missing field name.");
        return NULL;
    }
    if (!matchToken(parser, TOKEN_CLOSE_PAREN, ")")) {
        strcpy(parser->parserMessage, "invalid sql: missing ).");
        return NULL;
    }
    if (!matchToken(parser, TOKEN_RESERVED_WORD, "values")) {
        strcpy(parser->parserMessage, "invalid sql: missing values.");
        return NULL;
    }
    if (!matchToken(parser, TOKEN_OPEN_PAREN, "(")) {
        strcpy(parser->parserMessage, "invalid sql: missing (.");
        return NULL;
    }
    token = parseNextToken(parser);
    if (token->type == TOKEN_STRING || token->type == TOKEN_DECIMAL) {
        while (token->type == TOKEN_STRING || token->type == TOKEN_DECIMAL) {
            if (token->type == TOKEN_STRING){
                int len = strlen(token->text) - 1;
                char *v = token->text + 1;
                char *value = (char *) calloc(len, 1);
                strcpy(value, v);
                value[len-1] = '\0';

                arraylist_add(values, value);
            } else{
                integer *i = (integer *)calloc(sizeof(integer *), 1);
                i->val = atoi(token->text);
                arraylist_add(values, i);
            }

            token = parseEatAndNextToken(parser);
            if (token->type==TOKEN_COMMA){
                token = parseEatAndNextToken(parser);
            }else{
                break;
            }
        }
    } else {
        strcpy(parser->parserMessage, "invalid sql: missing a value.");
        return NULL;
    }
    if (!matchToken(parser, TOKEN_CLOSE_PAREN, ")")) {
        strcpy(parser->parserMessage, "invalid sql: missing ).");
        return NULL;
    }
    sql_stmt_insert *sqlStmtInsert = (sql_stmt_insert *)calloc(sizeof(sql_stmt_insert),1);
    sqlStmtInsert->tableName = tableName;
    sqlStmtInsert->fields = fields;
    sqlStmtInsert->values = values;
    return sqlStmtInsert;
};

