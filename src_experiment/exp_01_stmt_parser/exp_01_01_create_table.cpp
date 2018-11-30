//
// Created by Sam on 2018/2/13.
//

#include <parser/statement.h>
#include <parser/parser.h>
#include <utils/utils.h>

/**
 * 在现有create table基础上，修改代码以支持pk，fk，check，unique，not null约束。
 */

sql_stmt_create *parse_sql_stmt_create(Parser *parser) {
    char *tableName = NULL;
    map<string, field_info*> *columns = new map<string, field_info*>();
    vector<char*> fieldsName ;
    if (!parser->matchToken(TOKEN_RESERVED_WORD, "create")) {
        return NULL;
    }
    if (!parser->matchToken( TOKEN_RESERVED_WORD, "table")) {
        strcpy(parser->parserMessage, "invalid sql: should be table.");
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

    while (token->type != TOKEN_CLOSE_PAREN) {
        field_info *field = parse_sql_stmt_columnexpr(parser);
        if (field == NULL) {
            break;
        } else {
            columns->insert(pair<string, field_info*>(field->fieldName, field));
            fieldsName.push_back(field->fieldName);
        }
        token = parser->parseNextToken();
        if (token->type == TOKEN_COMMA) {
            token = parser->parseEatAndNextToken();
        } else {
            break;
        }
    }
    token = parser->parseNextToken();
    if (!parser->matchToken( TOKEN_CLOSE_PAREN, ")")) {
        strcpy(parser->parserMessage, "invalid sql: missing ).");
        return NULL;
    }

    return sql_stmt_create_create(tableName, fieldsName, columns, NULL);
};

field_info *parse_sql_stmt_columnexpr(Parser *parser) {
    Token *token = parser->parseNextToken();
    char *columnName = NULL;
    enum data_type type;
    int length;
    if (token->type == TOKEN_WORD) {
        columnName = new_id_name();
        strcpy(columnName, token->text);
    } else {
        strcpy(parser->parserMessage, "invalid sql: missing field name.");
        return NULL;
    }
    token = parser->parseEatAndNextToken();
    if (token->type == TOKEN_RESERVED_WORD) {
        if (stricmp(token->text, "int") == 0 || stricmp(token->text, "integer") == 0) {
            type = DATA_TYPE_INT;
            length = INT_SIZE;
            token = parser->parseEatAndNextToken();
        } else if (stricmp(token->text, "char") == 0) {
            type = DATA_TYPE_CHAR;
            token = parser->parseEatAndNextToken();
            if (parser->matchToken( TOKEN_OPEN_PAREN, "(")) {
                token = parser->parseNextToken();
                if (token->type == TOKEN_DECIMAL) {
                    length = atoi(token->text);
                    token = parser->parseEatAndNextToken();
                    if (parser->matchToken( TOKEN_CLOSE_PAREN, ")")) {
                        token = parser->parseNextToken();
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
