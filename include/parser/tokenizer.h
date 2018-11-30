//
// Created by Sam on 2018/1/16.
//

#ifndef DONGMENDB_TOKENIZER_H
#define DONGMENDB_TOKENIZER_H

#include "Token.h"

/* 词法分析
 * Tokenizer type.  You need to fill in the type as part of your implementation.
 */

class Tokenizer {
public:
    char *inputStream;
    char *tokenBuffer;
    char *inputIter;
    char *bufferIter;
    int bufferSize;
    int offset;
    TokenType currTokenType;


    Tokenizer(const char *ts);
    ~Tokenizer();

    int isOctal(char curr);
    void clearBuffer();

    int nextChar();
    int isReservedWord(char *word);

    void destroyToken(Token *token);
    void printToken(Token *token);
    Token *makeToken(TokenType type);

    Token *TKGetNextToken();
    Token *_invalid();
    Token *_word();
    Token *_neq();
    Token *_double_quote();
    Token *_mod();
    Token *_single_quote();
    Token *_open_paren();
    Token *_close_paren();
    Token *_mult();
    Token *_plus();
    Token *_comma();
    Token *_minus();
    Token *_line_comment();
    Token *_block_comment();
    Token *_div();
    Token *_semicolon();
    Token *_lt();
    Token *_eq();
    Token *_gt();
    Token *_expofloat( int isFirst, int lastWasSign);
    Token *_float( int isFirst);
    Token *_octal();
    Token *_hex( int isFirst);
    Token *_decimal();
    Token *_zero();
    Token *_power();

} ;





#endif //DONGMENDB_TOKENIZER_H
