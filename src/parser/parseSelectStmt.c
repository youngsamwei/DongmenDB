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


/**
 * @brief 解析select语句
 * @param parser 解析器
 * @return select语句
 */
SRA_t *parse_sql_stmt_select(ParserT *parser) {
    TokenT *token = parseNextToken(parser);
    if(!matchToken(parser, TOKEN_RESERVED_WORD, "select")){
        strcpy(parser->parserMessage, "语法错误.");
        return NULL;
    }
    arraylist *fieldsExpr = parseFieldsExpr(parser);
    if (parser->parserStateType == PARSER_WRONG) {
        return NULL;
    }
    token = parseNextToken(parser);
    if(!matchToken(parser, TOKEN_RESERVED_WORD, "from")){
        strcpy(parser->parserMessage, "语法错误.");
        return NULL;
    }
    token = parseNextToken(parser);
    SRA_t *tablesExpr = parseTablesExpr(parser);
    if (parser->parserStateType == PARSER_WRONG) {
        return NULL;
    }
    token = parseNextToken(parser);
    if (token == NULL || token->type == TOKEN_SEMICOLON) {
        SRA_t *project = SRAProject(tablesExpr, fieldsExpr);
        return project;
    }
    token = parseNextToken(parser);
    if(!matchToken(parser, TOKEN_RESERVED_WORD, "where")){
        strcpy(parser->parserMessage, "语法错误.");
        return NULL;
    }
    token = parseNextToken(parser);
    Expression *whereExpr = parseExpressionRD(parser);
    if (parser->parserStateType == PARSER_WRONG) {
        return NULL;
    }
    SRA_t *select = SRASelect(tablesExpr, whereExpr);
    SRA_t *project = SRAProject(select, fieldsExpr);
    token = parseNextToken(parser);
    if (token == NULL || token->type == TOKEN_SEMICOLON) {
        return project;
    }
    token = parseNextToken(parser);
    if(!matchToken(parser, TOKEN_RESERVED_WORD, "group")){
        strcpy(parser->parserMessage, "语法错误.");
        return NULL;
    }
    token = parseNextToken(parser);
    if(!matchToken(parser, TOKEN_RESERVED_WORD, "by")){
        strcpy(parser->parserMessage, "语法错误.");
        return NULL;
    }
    token = parseNextToken(parser);
    arraylist *groupExpr = parseFieldsExpr(parser);
    project->project.group_by = groupExpr;
    project->project.group_by = groupExpr;
    if (parser->parserStateType == PARSER_WRONG) {
        return NULL;
    }
    token = parseNextToken(parser);
    if (token == NULL) {
        return project;
    }
    token = parseNextToken(parser);
    if(!matchToken(parser, TOKEN_RESERVED_WORD, "order")){
        strcpy(parser->parserMessage, "语法错误.");
        return NULL;
    }
    token = parseNextToken(parser);
    if(!matchToken(parser, TOKEN_RESERVED_WORD, "by")){
        strcpy(parser->parserMessage, "语法错误.");
        return NULL;
    }
    token = parseNextToken(parser);
    arraylist *orderExpr = parseOrderExpr(parser);
    project->project.order_by = orderExpr;
    if (parser->parserStateType == PARSER_WRONG) {
        return NULL;
    }
    token = parseNextToken(parser);;
    if (token == NULL || token->type == TOKEN_SEMICOLON) {
        return project;
    } else {
        return NULL;
    }

};

/**
 * 解析select后面的表达式列表，以arraylist返回
 * @param parser 语法分析器
 * @return 表达式列表
 */
arraylist *parseFieldsExpr(ParserT *parser) {
    arraylist *exprs = arraylist_create();
    Expression *expr0 = parseExpressionRD(parser);
    arraylist_add(exprs, expr0);

    TokenT *token = parseNextToken(parser);
    while (token != NULL && token->type == TOKEN_COMMA) {
        parseEatAndNextToken(parser);
        Expression *expr1 = parseExpressionRD(parser);
        arraylist_add(exprs, expr1);
        token = parseNextToken(parser);
    }

    return exprs;
};

SRA_t *parseTablesExpr(ParserT *parser) {
    TokenT *token = parseNextToken(parser);
    if (token->type == TOKEN_WORD) {

        char *tableName = strdup(token->text);
        TableReference_t *ref =   TableReference_make(tableName, NULL);
        SRA_t *table =  SRATable(ref);

        TokenT *token = parseEatAndNextToken(parser);
        while (token != NULL && token->type == TOKEN_COMMA) {
            token = parseEatAndNextToken(parser);/*跳过comma*/
            char *tableName = strdup(token->text);
            TableReference_t *ref1 =   TableReference_make(tableName, NULL);
            SRA_t *table1 =  SRATable(ref1);

            table = SRAJoin(table, table1, NULL);
            token = parseEatAndNextToken(parser);
        }

        return table;
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

