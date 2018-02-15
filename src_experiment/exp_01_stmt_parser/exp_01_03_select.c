//
// Created by Sam on 2018/2/13.
//



#include "parser/parseSelectStmt.h"
#include <dongmensql/sra.h>
#include <parser/parser.h>
#include <parser/expression.h>
#include <parser/statement.h>

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
