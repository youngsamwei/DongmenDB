//
// Created by Sam on 2018/1/23.
//

#include <parser.h>
#include <stdio.h>


int main(int argc, char **argv) {

    char *select;

    select = "select sno,sname,ssex from student, sc, course;";
    if (argc == 2) {
        select = argv[1];
    }

    TokenizerT *tokenizer = TKCreate(select);
    ParserT *parser = newParser(tokenizer);
    char *selectStr;
    SelectStmt *selectStmt = parseSelectStmt(parser);
    if (selectStmt != NULL) {
        selectStr = printSelectStmt(selectStmt);
        printf(selectStr);
    } else {
        printf("error");
    }
}