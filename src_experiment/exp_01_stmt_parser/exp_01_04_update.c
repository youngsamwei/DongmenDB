//
// Created by Sam on 2018/2/13.
//

#include <parser/statement.h>

/**
 * 在现有实现基础上，实现update from子句
 *
 * 支持的update语法：
 *
 * UPDATE <table_name> SET <field1> = <expr1>[, <field2 = <expr2>, ..]
 * WHERE <logical_expr>
 *
 * 解析获得 sql_stmt_update 结构
 */

/*TODO: parse_sql_stmt_update， update语句解析*/
sql_stmt_update *parse_sql_stmt_update(ParserT *parser) {
//    fprintf(stderr, "TODO: update is not implemented yet. in parse_sql_stmt_update \n");
    char *tableName = NULL;
    arraylist *fields = arraylist_create();
    arraylist *fieldsExpr = arraylist_create();

    TokenT *token = parseNextToken(parser);
    if (!matchToken(parser, TOKEN_RESERVED_WORD, "update")) {
        strcpy(parser->parserMessage, "语法错误.");
        return NULL;
    }

    token = parseNextToken(parser);
    if (token->type == TOKEN_WORD) {
        tableName = new_id_name();
        strcpy(tableName, token->text);
    } else {
        strcpy(parser->parserMessage, "invalid sql: missing table name.");
        return NULL;
    }

    token = parseEatAndNextToken(parser);
    if (!matchToken(parser, TOKEN_RESERVED_WORD, "set")) {
        strcpy(parser->parserMessage, "语法错误.");
        return NULL;
    }

    token = parseNextToken(parser);

    while (token != NULL && (token->type == TOKEN_WORD || token->type == TOKEN_COMMA)) {
        if (token->type == TOKEN_COMMA){
            /*若是逗号，则需要跳过逗号*/
            token = parseEatAndNextToken(parser);
        }
        /*获取字段名*/
        if (token->type == TOKEN_WORD) {
            char *fieldName = new_id_name();
            strcpy(fieldName, token->text);
            arraylist_add(fields, fieldName);

        } else {
            strcpy(parser->parserMessage, "invalid sql: missing table name.");
            return NULL;
        }

        token = parseEatAndNextToken(parser);
        /*字段名后面跟着等号*/
        if (!matchToken(parser, TOKEN_EQ, "=")) {
            strcpy(parser->parserMessage, "invalid sql: missing =.");
            return NULL;
        }
        /*等号后面是表达式*/
        Expression *expr0 = parseExpressionRD(parser);
        arraylist_add(fieldsExpr, expr0);

        token = parseNextToken(parser);
        if (token == NULL) {
            break;
        }
    }
    Expression *whereExpr = NULL;
    if (token !=NULL) {
        if (!matchToken(parser, TOKEN_RESERVED_WORD, "where")) {
            strcpy(parser->parserMessage, "语法错误.");
            return NULL;
        }
        whereExpr = parseExpressionRD(parser);

        if (parser->parserStateType == PARSER_WRONG) {
            return NULL;
        }
    }

    sql_stmt_update *sqlStmtUpdate = (sql_stmt_update *) malloc(sizeof(sql_stmt_update));
    sqlStmtUpdate->tableName = tableName;
    sqlStmtUpdate->fields = fields;
    sqlStmtUpdate->fieldsExpr = fieldsExpr;

    TableReference_t *ref =   TableReference_make(tableName, NULL);
    SRA_t *table =  SRATable(ref);
    sqlStmtUpdate->where = table;
    if (whereExpr != NULL) {
        sqlStmtUpdate->where = SRASelect(table, whereExpr);
    }

    return sqlStmtUpdate;
};