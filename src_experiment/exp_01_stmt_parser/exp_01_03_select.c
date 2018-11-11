//
// Created by Sam on 2018/2/13.
//



#include <dongmensql/sra.h>
#include <parser/statement.h>

/**
 * @brief 解析select语句，对select语句进行语法分析,并将select语句转换为关系代数表达式
 * @param parser 词法解析器
 * @return select语句
 */
SRA_t *parse_sql_stmt_select(ParserT *parser) {
    TokenT *token = parseNextToken(parser);
    /*匹配select关键词*/
    if(!matchToken(parser, TOKEN_RESERVED_WORD, "select")){
        strcpy(parser->parserMessage, "语法错误.");
        return NULL;
    }
    /*解析select子句中的表达式列表*/
    arraylist *fieldsExpr = parseFieldsExpr(parser);
    if (parser->parserStateType == PARSER_WRONG) {
        return NULL;
    }
    /*匹配from关键词*/
    token = parseNextToken(parser);
    if(!matchToken(parser, TOKEN_RESERVED_WORD, "from")){
        strcpy(parser->parserMessage, "语法错误.");
        return NULL;
    }
    /*解析from子句中的数据表列表*/
    token = parseNextToken(parser);
    SRA_t *tablesExpr = parseTablesExpr(parser);
    if (parser->parserStateType == PARSER_WRONG) {
        return NULL;
    }
    /*若select语句结束了,则将select语句转化为关系代数表达式，并返回*/
    token = parseNextToken(parser);
    if (token == NULL || token->type == TOKEN_SEMICOLON) {
        SRA_t *project = SRAProject(tablesExpr, fieldsExpr);
        return project;
    }
    /*若没有结束，则继续匹配where子句*/
    token = parseNextToken(parser);
    if(!matchToken(parser, TOKEN_RESERVED_WORD, "where")){
        strcpy(parser->parserMessage, "语法错误.");
        return NULL;
    }
    /*解析where子句中的条件表达式*/
    token = parseNextToken(parser);
    Expression *whereExpr = parseExpressionRD(parser);
    if (parser->parserStateType == PARSER_WRONG) {
        return NULL;
    }
    SRA_t *select = SRASelect(tablesExpr, whereExpr);
    SRA_t *project = SRAProject(select, fieldsExpr);
    token = parseNextToken(parser);
    /*若select语句结束了，则构造关系代数表达式，并返回*/
    if (token == NULL || token->type == TOKEN_SEMICOLON) {
        return project;
    }
    token = parseNextToken(parser);
    /*如果未结束，则尝试匹配group子句*/
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
    /*如果未结束，则尝试匹配order子句*/
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
    token = parseNextToken(parser);
    /*select语句终于结束了,返回构造的关系代数表达式*/
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
    /*解析from子句中第一个表达式*/
    Expression *expr0 = parseExpressionRD(parser);
    arraylist_add(exprs, expr0);

    TokenT *token = parseNextToken(parser);
    /*若还有其他表达式则进入循环，否则返回*/
    while (token != NULL && token->type == TOKEN_COMMA) {
        parseEatAndNextToken(parser);
        Expression *expr1 = parseExpressionRD(parser);
        arraylist_add(exprs, expr1);
        token = parseNextToken(parser);
    }

    return exprs;
};

/*解析from子句的数据表，并转换为笛卡尔积构成的关系代数表达式。
 * 比如from student 转换为 SRATable(student)
 * from student, sc 转换为 SRAJoin(SRATable(sc), SRATable(student))
 * from studnet, sc, course 转换为 SRAJoin(SRATable(course), SRAJoin(SRATable(student), SRATable(sc)))*
 *
 * */
SRA_t *parseTablesExpr(ParserT *parser) {
    TokenT *token = parseNextToken(parser);
    if (token->type == TOKEN_WORD) {

        /*解析from子句第一个数据表*/
        char *tableName = strdup(token->text);
        TableReference_t *ref =   TableReference_make(tableName, NULL);
        SRA_t *table =  SRATable(ref);

        TokenT *token = parseEatAndNextToken(parser);
        /*若还有其他数据表，则进入循环继续解析，否则返回*/
        while (token != NULL && token->type == TOKEN_COMMA) {
            token = parseEatAndNextToken(parser);/*跳过comma*/
            char *tableName = strdup(token->text);
            TableReference_t *ref1 =   TableReference_make(tableName, NULL);
            SRA_t *table1 =  SRATable(ref1);

            /*将当前解析的数据表与上一个关系代数表达式通过笛卡尔积操作结合*/
            table = SRAJoin(table, table1, NULL);
            token = parseEatAndNextToken(parser);
        }

        return table;
    }

    return NULL;
};
