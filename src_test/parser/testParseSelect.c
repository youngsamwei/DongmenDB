//
// Created by Sam on 2018/1/23.
//

#include <parser.h>
#include <stddef.h>
#include <stdio.h>
#include "testParseSelect.h"
int main(int argc, char **argv) {

    char *select;

    select = "select sno from studnet;";
    if (argc == 2) {
        select = argv[1];
    }

    TokenizerT *tokenizer = TKCreate(select);
    ParserT *parser = newParser(tokenizer);

    SelectStmt *selectStmt = parseSelect(parser);
    if(selectStmt != NULL){
        printExpression(selectStmt->fieldsExpr->expr);
    }else{
        printf("error");
    }
}