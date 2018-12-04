//
// Created by Sam on 2018/2/13.
//

#include <parser/StatementParser.h>
#include <dongmensql/sqlstatement.h>

/**
 * 在现有create table基础上，修改代码以支持pk，fk，check，unique，not null约束。
 */

sql_stmt_create *CreateParser::parse_sql_stmt_create() {
    char *tableName = NULL;
    map<string, FieldInfo *> *columns = new map<string, FieldInfo *>();
    vector<char*> fieldsName ;
    if (!this->matchToken(TOKEN_RESERVED_WORD, "create")) {
        return NULL;
    }
    if (!this->matchToken( TOKEN_RESERVED_WORD, "table")) {
        strcpy(this->parserMessage, "invalid sql: should be table.");
        return NULL;
    }
    Token *token = this->parseNextToken();
    if (token->type == TOKEN_WORD) {
        tableName = new_id_name();
        strcpy(tableName, token->text);
    } else {
        strcpy(this->parserMessage, "invalid sql: missing table name.");
        return NULL;
    }
    token = this->parseEatAndNextToken();
    if (!this->matchToken( TOKEN_OPEN_PAREN, "(")) {
        strcpy(this->parserMessage, "invalid sql: missing (.");
        return NULL;
    }
    token = this->parseNextToken();

    while (token->type != TOKEN_CLOSE_PAREN) {
        FieldInfo *field = parse_sql_stmt_columnexpr();
        if (field == NULL) {
            break;
        } else {
            columns->insert(pair<string, FieldInfo *>(field->fieldName, field));
            fieldsName.push_back(field->fieldName);
        }
        token = this->parseNextToken();
        if (token->type == TOKEN_COMMA) {
            token = this->parseEatAndNextToken();
        } else {
            break;
        }
    }
    token = this->parseNextToken();
    if (!this->matchToken( TOKEN_CLOSE_PAREN, ")")) {
        strcpy(this->parserMessage, "invalid sql: missing ).");
        return NULL;
    }
    sql_stmt_create *sqlStmtCreate = (sql_stmt_create *)malloc(sizeof(sql_stmt_create));
    sqlStmtCreate->tableInfo =  new TableInfo(tableName, fieldsName, columns);
    sqlStmtCreate->constraints = NULL;
    return sqlStmtCreate;
};

FieldInfo *CreateParser::parse_sql_stmt_columnexpr() {
    Token *token = this->parseNextToken();
    char *columnName = NULL;
    enum data_type type;
    int length;
    if (token->type == TOKEN_WORD) {
        columnName = new_id_name();
        strcpy(columnName, token->text);
    } else {
        strcpy(this->parserMessage, "invalid sql: missing field name.");
        return NULL;
    }
    token = this->parseEatAndNextToken();
    if (token->type == TOKEN_RESERVED_WORD) {
        if (stricmp(token->text, "int") == 0 || stricmp(token->text, "integer") == 0) {
            type = DATA_TYPE_INT;
            length = INT_SIZE;
            token = this->parseEatAndNextToken();
        } else if (stricmp(token->text, "char") == 0) {
            type = DATA_TYPE_CHAR;
            token = this->parseEatAndNextToken();
            if (this->matchToken( TOKEN_OPEN_PAREN, "(")) {
                token = this->parseNextToken();
                if (token->type == TOKEN_DECIMAL) {
                    length = atoi(token->text);
                    token = this->parseEatAndNextToken();
                    if (this->matchToken( TOKEN_CLOSE_PAREN, ")")) {
                        token = this->parseNextToken();
                    } else {
                        strcpy(this->parserMessage, "invalid sql: missing ).");
                        return NULL;
                    }
                } else {
                    strcpy(this->parserMessage, "invalid sql: missing char length.");
                    return NULL;
                }
            } else {
                strcpy(this->parserMessage, "invalid sql: missing char length.");
                return NULL;
            }
        } else {
            strcpy(this->parserMessage, "invalid sql: wrong data type : ");
            strcat(this->parserMessage, token->text);
            return NULL;
        }
    } else {
        strcpy(this->parserMessage, "invalid sql : missing field name.");
        return NULL;
    }
    FieldInfo *field = new FieldInfo(type, length, columnName);

    return field;
};
