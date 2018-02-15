//
// Created by Sam on 2018/1/16.
//

#ifndef DONGMENDB_TOKENIZER_H
#define DONGMENDB_TOKENIZER_H
#ifdef __cplusplus
extern "C" {
#endif


/* 词法分析
 * Tokenizer type.  You need to fill in the type as part of your implementation.
 */

/*与operators中的排序一致，方便按照顺序访问operators*/
typedef enum TokenType_ {
    TOKEN_OPEN_PAREN = 0,
    TOKEN_CLOSE_PAREN,
    TOKEN_POWER,
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_DIVIDE,
    TOKEN_MULTIPLY,
    TOKEN_LT,              //less-than operator
    TOKEN_GT,
    TOKEN_EQ,
    TOKEN_NOT_EQUAL,
    TOKEN_LE,               //less-than-or-equal-to operator"
    TOKEN_GE,
    TOKEN_IN,
    TOKEN_LIKE,
    TOKEN_AND,
    TOKEN_OR,
    TOKEN_NOT,
    TOKEN_ASSIGNMENT,
    TOKEN_FUN,
    TOKEN_COMMA,      /*以上是操作符，在表达式解析时使用*/

    TOKEN_INVALID, //"invalid token"
    TOKEN_RESERVED_WORD,
    TOKEN_WORD,
    TOKEN_UNENDED_SRING,
    TOKEN_STRING,
    TOKEN_MOD,
    TOKEN_INCOMPLETE_CHAR,
    TOKEN_CHAR,
    TOKEN_INVALID_CHAR,
    TOKEN_SEMICOLON,
    TOKEN_EXP_FLOAT,
    TOKEN_FLOAT,
    TOKEN_OCTAL,               //"octal integer"
    TOKEN_HEX,                 //"hexadecimal integer"
    TOKEN_DECIMAL,            //"decimal integer"
    TOKEN_ZERO,               //"zero integer"
    TOKEN_NULL
} TokenType;

typedef struct TokenizerT_ {
    char *inputStream;
    char *tokenBuffer;
    char *inputIter;
    char *bufferIter;
    int bufferSize;
    int offset;
    TokenType currTokenType;
} TokenizerT;

typedef struct TokenT_ {
    char *text;
    TokenType type;
} TokenT;


int isOctal(char curr);
TokenizerT *TKCreate(const char *ts);
void clearBuffer(TokenizerT *tk);
void TKDestroy(TokenizerT *tk);
int nextChar(TokenizerT *tk);
int isReservedWord(char *word);
TokenT *makeToken(TokenizerT *tk, TokenType type);
void destroyToken(TokenT *token);
void printToken(TokenT *token);

TokenT *TKGetNextToken(TokenizerT *tk);
TokenT *_invalid(TokenizerT *tk);
TokenT *_word(TokenizerT *tk);
TokenT *_neq(TokenizerT *tk);
TokenT *_double_quote(TokenizerT *tk);
TokenT *_mod(TokenizerT *tk);
TokenT *_single_quote(TokenizerT *tk);
TokenT *_open_paren(TokenizerT *tk);
TokenT *_close_paren(TokenizerT *tk);
TokenT *_mult(TokenizerT *tk);
TokenT *_plus(TokenizerT *tk);
TokenT *_comma(TokenizerT *tk);
TokenT *_minus(TokenizerT *tk);
TokenT *_line_comment(TokenizerT *tk);
TokenT *_block_comment(TokenizerT *tk);
TokenT *_div(TokenizerT *tk);
TokenT *_semicolon(TokenizerT *tk);
TokenT *_lt(TokenizerT *tk);
TokenT *_eq(TokenizerT *tk);
TokenT *_gt(TokenizerT *tk);
TokenT *_expofloat(TokenizerT *tk, int isFirst, int lastWasSign);
TokenT *_float(TokenizerT *tk, int isFirst);
TokenT *_octal(TokenizerT *tk);
TokenT *_hex(TokenizerT *tk, int isFirst);
TokenT *_decimal(TokenizerT *tk);
TokenT *_zero(TokenizerT *tk);
TokenT *TKGetNextToken(TokenizerT *tk);

#ifdef __cplusplus
}
#endif

#endif //DONGMENDB_TOKENIZER_H
