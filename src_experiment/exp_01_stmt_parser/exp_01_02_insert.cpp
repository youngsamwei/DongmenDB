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

sql_stmt_insert *parse_sql_stmt_insert(Parser *parser) {
    char *tableName = NULL;
    vector<char*> fields ;
    vector<variant*> values ;

    if (!parser->matchToken( TOKEN_RESERVED_WORD, "insert")) {
        return NULL;
    }
    if (!parser->matchToken( TOKEN_RESERVED_WORD, "into")) {
        strcpy(parser->parserMessage, "invalid sql: should be into.");
        return NULL;
    }
    Token *token = parser->parseNextToken();
    if (token->type == TOKEN_WORD) {
        tableName = new_id_name();
        strcpy(tableName, token->text);
    } else {
        strcpy(parser->parserMessage, "invalid sql: missing table name.");
        return NULL;
    }
    token = parser->parseEatAndNextToken();
    if (!parser->matchToken( TOKEN_OPEN_PAREN, "(")) {
        strcpy(parser->parserMessage, "invalid sql: missing (.");
        return NULL;
    }
    token = parser->parseNextToken();
    if (token->type == TOKEN_WORD) {
        while (token->type == TOKEN_WORD) {
            char *fieldName = new_id_name();
            strcpy(fieldName, token->text);
            fields.push_back(fieldName);
            token = parser->parseEatAndNextToken();
            if (token->type==TOKEN_COMMA){
                token = parser->parseEatAndNextToken();
            }else{
                break;
            }
        }
    } else {
        strcpy(parser->parserMessage, "invalid sql: missing field name.");
        return NULL;
    }
    if (!parser->matchToken( TOKEN_CLOSE_PAREN, ")")) {
        strcpy(parser->parserMessage, "invalid sql: missing ).");
        return NULL;
    }
    if (!parser->matchToken( TOKEN_RESERVED_WORD, "values")) {
        strcpy(parser->parserMessage, "invalid sql: missing values.");
        return NULL;
    }
    if (!parser->matchToken( TOKEN_OPEN_PAREN, "(")) {
        strcpy(parser->parserMessage, "invalid sql: missing (.");
        return NULL;
    }
    token = parser->parseNextToken();
    if (token->type == TOKEN_STRING || token->type == TOKEN_DECIMAL) {
        while (token->type == TOKEN_STRING || token->type == TOKEN_DECIMAL) {
            if (token->type == TOKEN_STRING){
                /*去掉引号, 已经在tokenizer.c中完成*/
               /* int len = strlen(token->text) - 1;
                char *v = token->text + 1;
                char *value = (char *) calloc(len, 1);
                strcpy(value, v);
                value[len-1] = '\0';*/

                variant *v = (variant*)malloc(sizeof(variant*));
                v->type == DATA_TYPE_CHAR;
                v->strValue=token->text;
                values.push_back(v);
            } else{
                integer *i = (integer *)calloc(sizeof(integer *), 1);
                i->val = atoi(token->text);

                variant *v = (variant*)malloc(sizeof(variant*));
                v->type == DATA_TYPE_INT;
                v->intValue = i->val;
                values.push_back(v);
            }

            token = parser->parseEatAndNextToken();
            if (token->type==TOKEN_COMMA){
                token = parser->parseEatAndNextToken();
            }else{
                break;
            }
        }
    } else {
        strcpy(parser->parserMessage, "invalid sql: missing a value.");
        return NULL;
    }
    if (!parser->matchToken( TOKEN_CLOSE_PAREN, ")")) {
        strcpy(parser->parserMessage, "invalid sql: missing ).");
        return NULL;
    }
    sql_stmt_insert *sqlStmtInsert = (sql_stmt_insert *)calloc(sizeof(sql_stmt_insert),1);
    sqlStmtInsert->tableName = tableName;
    sqlStmtInsert->fields = fields;
    sqlStmtInsert->values = values;
    return sqlStmtInsert;
};

