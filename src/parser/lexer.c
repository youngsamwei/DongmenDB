//
// Created by Sam on 2018/1/17.
//

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>
#include "tokenizer.h"
#include "lexer.h"

/*创建语法解析器*/
LexerT *Lexer(char *sql){
    LexerT *lexer = (LexerT *) malloc(sizeof(LexerT));

    TokenizerT *tokenizer= TKCreate(sql);
    lexer->tokenizer = tokenizer;

    /*获得第一个token*/
    lexer->token = TKGetNextToken(tokenizer);

    return lexer;
}