//
// Created by Sam on 2018/2/2.
//


#include <stdio.h>
#include <statement.h>
#include <parser.h>
#include <parseSelectStmt.h>
#include <mem.h>


int main(int argc, char **argv) {

    char *select = "insert into student(sno, sname, sage) values(\"2012010101\",\"tom simith\",20);";
    if (argc == 2) {
        select = argv[1];
    }

    TokenizerT *tokenizer = TKCreate(select);
    ParserT *parser = newParser(tokenizer);
    memset(parser->parserMessage, 0, sizeof(parser->parserMessage));

    sql_stmt_insert *insertStmt = parse_sql_stmt_insert(parser);
    if (insertStmt != NULL) {
        printf("correct!");
    } else {
        printf(parser->parserMessage);
    }
}