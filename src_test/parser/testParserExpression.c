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
    // make sure theres exactly 1 argument
    char *sexpr = " a > b";
    TokenizerT *tokenizer = TKCreate(sexpr);

    Expression *expr = parseExpression(tokenizer);

    TKDestroy(tokenizer);

    return 0;
}