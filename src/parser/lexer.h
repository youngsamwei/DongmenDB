//
// Created by Sam on 2018/1/17.
//

#ifndef DONGMENDB_LEXER_H
#define DONGMENDB_LEXER_H

#include "tokenizer.h"

struct LexerT_{
    TokenizerT *tokenizer; /*词法解析器*/
    TokenT *token; /*当前token*/
} ;

typedef struct LexerT_ LexerT ;

#endif //DONGMENDB_LEXER_H
