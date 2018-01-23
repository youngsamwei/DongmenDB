//
// Created by Sam on 2018/1/23.
//

#include <stdio.h>
#include <statement.h>
#include <parser.h>
#include <parseSelectStmt.h>


int main(int argc, char **argv) {

    char *select;

    select = "select sno,sname,ssex from student, sc, course;";
    if (argc == 2) {
        select = argv[1];
    }

    TokenizerT *tokenizer = TKCreate(select);
    ParserT *parser = newParser(tokenizer);
    char selectStr[256];
    SelectStmt *selectStmt = parseSelectStmt(parser);
    if (selectStmt != NULL) {
        printSelectStmt(selectStmt, selectStr);
        printf(selectStr);
    } else {
        printf("error");
    }
}