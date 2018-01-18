//
// Created by Sam on 2018/1/16.
//

#ifndef DONGMENDB_TOKENIZER_H
#define DONGMENDB_TOKENIZER_H


/*
 * Tokenizer type.  You need to fill in the type as part of your implementation.
 */
typedef struct TokenizerT_ {
    char *inputStream;
    char *tokenBuffer;
    char *inputIter;
    char *bufferIter;
    int bufferSize;
} TokenizerT;

typedef struct TokenT_ {
    char *text;
    char *type;
} TokenT ;


int isOctal(char curr);
TokenizerT *TKCreate(char *ts);
void clearBuffer(TokenizerT *tk);
void TKDestroy(TokenizerT *tk);
int nextChar(TokenizerT *tk);
int isReservedWord(char *word);
TokenT *makeToken(TokenizerT *tk, char *type);
void destroyToken(TokenT *token);
void printToken(TokenT *token);

TokenT *TKGetNextToken(TokenizerT *tk);
TokenT *_invalid(TokenizerT *tk);
TokenT *_word(TokenizerT *tk);
TokenT *_neq(TokenizerT *tk);
TokenT *_double_quote(TokenizerT *tk);
TokenT *_mod(TokenizerT *tk);
TokenT *_bit_and(TokenizerT *tk);
TokenT *_single_quote(TokenizerT *tk);
TokenT *_open_paren(TokenizerT *tk);
TokenT *_close_paren(TokenizerT *tk);
TokenT *_mult(TokenizerT *tk);
TokenT *_plus(TokenizerT *tk);
TokenT *_comma(TokenizerT *tk);
TokenT *_minus(TokenizerT *tk);
TokenT *_dot(TokenizerT *tk);
TokenT *_line_comment(TokenizerT *tk);
TokenT *_block_comment(TokenizerT *tk);
TokenT *_div(TokenizerT *tk);
TokenT *_ternary_colon(TokenizerT *tk);
TokenT *_semicolon(TokenizerT *tk);
TokenT *_lt(TokenizerT *tk);
TokenT *_eq(TokenizerT *tk);
TokenT *_gt(TokenizerT *tk);
TokenT *_open_bracket(TokenizerT *tk);
TokenT *_close_bracket(TokenizerT *tk);
TokenT *_bit_xor(TokenizerT *tk);
TokenT *_open_brace(TokenizerT *tk);
TokenT *_bit_or(TokenizerT *tk);
TokenT *_close_brace(TokenizerT *tk);
TokenT *_bit_not(TokenizerT *tk);
TokenT *_pound(TokenizerT *tk);
TokenT *_expofloat(TokenizerT *tk, int isFirst, int lastWasSign);
TokenT *_float(TokenizerT *tk, int isFirst);
TokenT *_octal(TokenizerT *tk);
TokenT *_hex(TokenizerT *tk, int isFirst);
TokenT *_decimal(TokenizerT *tk);
TokenT *_zero(TokenizerT *tk);
TokenT *TKGetNextToken(TokenizerT *tk);



#endif //DONGMENDB_TOKENIZER_H
