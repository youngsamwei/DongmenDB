//
// Created by Sam on 2018/1/20.
//


#include <parser/tokenizer.h>
#include <stdlib.h>
#include <stdio.h>

/*
 * main will have a string argument (in argv[1]).
 * The string argument contains the tokens.
 * Print out the tokens in the second string in left-to-right order.
 * Each token should be printed on a separate line.
 */
int main(int argc, char **argv) {
    // make sure theres exactly 1 argument
    if (argc != 2) {
        printf("Invalid number of arguments. \n");
        printf("Usage: \n    ./tokenizer <C-code string>\n");
        exit(1);
    }

    TokenizerT *tokenizer = TKCreate(argv[1]);

    TokenT *token;
    while ((token = TKGetNextToken(tokenizer)) != NULL) {
        printToken(token);
        destroyToken(token);
    }

    TKDestroy(tokenizer);

    return 0;
}