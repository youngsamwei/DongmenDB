//
// Created by Sam on 2018/2/13.
//
#include <parser/statement.h>
#include <parser/parser.h>
#include <utils/utils.h>

/**
 * 在现有实现的基础上，修改parser以支持insert values多条的特性。
 * insert into table(f1,f2,...,fn) values (v1,v2,...,vn),(v1,v2,...,vn),(v1,v2,...,vn)(v1,v2,...,vn);
 */

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
                /*去掉引号, 已经在tokenizer.c中完成*/
               /* int len = strlen(token->text) - 1;
                char *v = token->text + 1;
                char *value = (char *) calloc(len, 1);
                strcpy(value, v);
                value[len-1] = '\0';*/

                arraylist_add(values, token->text);
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

