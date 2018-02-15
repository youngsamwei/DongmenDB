//
// Created by Sam on 2018/2/13.
//

#include <parser/statement.h>
#include <parser/parser.h>
#include <utils/utils.h>

/**
 * 在现有create table基础上，修改代码以支持pk，fk，check，unique，not null约束。
 */

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
    enum data_type type;
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
