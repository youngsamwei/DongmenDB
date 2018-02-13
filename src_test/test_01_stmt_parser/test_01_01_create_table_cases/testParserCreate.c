//
// Created by Sam on 2018/2/2.
//


#include <stdio.h>
#include "../../../src/parser/statement.h"
#include "../../../src/parser/parseSelectStmt.h"
#include <mem.h>
#include "../../../src/parser/tokenizer.h"
#include "../../../src/parser/parser.h"
#include "../../../src/parser/statement.h"


int main(int argc, char **argv) {

    char *select = "create table student (sno char(10), sname char(20) sage integer);";
    if (argc == 2) {
        select = argv[1];
    }

    TokenizerT *tokenizer = TKCreate(select);
    ParserT *parser = newParser(tokenizer);
    memset(parser->parserMessage, 0, sizeof(parser->parserMessage));

    sql_stmt_create *createStmt = parse_sql_stmt_create(parser);
    if (createStmt != NULL) {
        printf("correct!");
    } else {
        printf(parser->parserMessage);
    }
}