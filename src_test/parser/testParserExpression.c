//
// Created by Sam on 2018/1/20.
//

#include <tokenizer.h>
#include <stdlib.h>
#include <stdio.h>


#include "parseExpression.h"

/*
 * main will have a string argument (in argv[1]).
 * The string argument contains the tokens.
 * Print out the tokens in the second string in left-to-right order.
 * Each token should be printed on a separate line.
 */
int main(int argc, char **argv) {

    char *sexpr;

//    char *sexpr = " 1 + 2 * (3 - 4)   ";
    //char *sexpr = " a + b * (c - d)  ";
//    char *sexpr = " a - b * (c - (d + e)) > f.x ";
     sexpr = " x = 1 + (fun(\"abc\" + field.name, fun(6, 8), 9)  + 10) < 20 ";
    if (argc == 2) {
        sexpr = argv[1];
    }

    TokenizerT *tokenizer = TKCreate(sexpr);

    Expression *expr = parseExpression(tokenizer);

    printExpression(expr);

    TKDestroy(tokenizer);

    return 0;
}