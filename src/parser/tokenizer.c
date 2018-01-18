//
// Created by Sam on 2018/1/16.
//
/*
 *  词法解析
 *  将sql语句解析为：
 *  关键字（DML，DDL，DCL等相关的关键字），
 *  常量（数值，字符串等），
 *  标识符（对象名称，字段名，数据库名称，约束名称等），
 *  运算符（算术运算，逻辑运算，比较运算等）等。
 *
 * from: https://github.com/MarcoGiancarli/tokenizer/blob/master/tokenizer.c
 * tokenizer.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>
#include "tokenizer.h"


int isOctal(char curr) {
    if(curr >= '0' && curr <= '7') {
        return 1;
    } else {
        return 0;
    }
}


/* 创建词法解析器
 * TKCreate creates a new TokenizerT object for a given token stream
 * (given as a string).
 *
 * TKCreate should copy the arguments so that it is not dependent on
 * them staying immutable after returning.  (In the future, this may change
 * to increase efficiency.)
 *
 * If the function succeeds, it returns a non-NULL TokenizerT.
 * Else it returns NULL.
 *
 * You need to fill in this function as part of your implementation.
 */
TokenizerT *TKCreate(char *ts) {
    // TODO: use strcpy to copy input stream ts
    TokenizerT *newTokenizer = (TokenizerT *) malloc(sizeof(TokenizerT));

    long int streamSize = strlen(ts) + 2;
    newTokenizer->inputStream = (char *) malloc(sizeof(char) * streamSize);
    strcpy(newTokenizer->inputStream, ts);
    newTokenizer->inputIter = newTokenizer->inputStream;

    newTokenizer->bufferSize = 0;
    newTokenizer->tokenBuffer = (char *) malloc(sizeof(char) * 1000);
    strcpy(newTokenizer->tokenBuffer, "");  // end the buffer with this null byte
    newTokenizer->bufferIter = newTokenizer->tokenBuffer;

    return newTokenizer;
}


/*
 * TKDestroy destroys a TokenizerT object.  It should free all dynamically
 * allocated memory that is part of the object being destroyed.
 *
 * You need to fill in this function as part of your implementation.
 */
void TKDestroy(TokenizerT *tk) {
    assert(tk != NULL);

    free(tk->inputStream);
    free(tk->tokenBuffer);
    free(tk);
}


/*
 * Copy one character from the inputStream to the tokenBuffer. Then, move the
 * iterators forward to the next character.
 *
 * tk->inputIter[0] contains the character to be added to the current token.
 * tk->bufferIter[0] contains the most recent character added to the token.
 *
 * Returns 1 if inputIter[0] is '\0', else 0.
 */
int nextChar(TokenizerT *tk) {
    tk->bufferIter[0] = tk->inputIter[0];  // copy new char to end of buffer
    tk->inputIter++;                       // move the input iterator over
    tk->bufferIter++;                      // move the buffer iterator over
    tk->bufferIter[0] = '\0';              // add a null to the end of buffer
    tk->bufferSize++;                      // increase bufferSize accordingly
    if(tk->bufferSize == 999) {
        // TODO: exit gracefully
        exit(1);
    }

    assert(strlen(tk->bufferIter) == 0);

    int nextIsNull = 0;
    if(tk->inputIter[0] == '\0') {
        nextIsNull = 1;
    }
    return nextIsNull;
}

void clearBuffer(TokenizerT *tk) {
    tk->bufferSize = 0;
    tk->bufferIter = tk->tokenBuffer;
    tk->bufferIter[0] = '\0';  // immediately end the string

    assert(strlen(tk->tokenBuffer) == 0);
}


/*
 * Returns 1 if a word is a reserved word. Otherwise, returns 0.
 */
int isReservedWord(char *word) {
    const char *reservedWords[33];
    reservedWords[0] = "auto";
    reservedWords[1] = "break";
    reservedWords[2] = "case";
    reservedWords[3] = "char";
    reservedWords[4] = "const";
    reservedWords[5] = "continue";
    reservedWords[6] = "default";
    reservedWords[7] = "do";
    reservedWords[8] = "double";
    reservedWords[9] = "else";
    reservedWords[10] = "enum";
    reservedWords[11] = "entry";
    reservedWords[12] = "extern";
    reservedWords[13] = "float";
    reservedWords[14] = "for";
    reservedWords[15] = "goto";
    reservedWords[16] = "if";
    reservedWords[17] = "int";
    reservedWords[18] = "long";
    reservedWords[19] = "register";
    reservedWords[20] = "return";
    reservedWords[21] = "short";
    reservedWords[22] = "signed";
    reservedWords[23] = "sizeof";
    reservedWords[24] = "static";
    reservedWords[25] = "struct";
    reservedWords[26] = "switch";
    reservedWords[27] = "typedef";
    reservedWords[28] = "union";
    reservedWords[29] = "unsigned";
    reservedWords[30] = "void";
    reservedWords[31] = "volatile";
    reservedWords[32] = "unsigned";
    reservedWords[33] = "while";

    int isReservedWord = 0;
    int rWordIndex = 0;
    for(; rWordIndex < 28; rWordIndex++) {
        if(strcmp(word, reservedWords[rWordIndex]) == 0) {
            isReservedWord = 1;
        }
    }

    return isReservedWord;
}


/*
 * Make a token struct from the current state of the tokenizer and the
 * identified type.
 */
TokenT *makeToken(TokenizerT *tk, char *type) {
    TokenT *token = (TokenT *) malloc(sizeof(TokenT));

    token->text = (char *) malloc(sizeof(char) * 1000);
    strcpy(token->text, tk->tokenBuffer);

    token->type = (char *) malloc(sizeof(char) * 50);
    strcpy(token->type, type);

    return token;
}


void destroyToken(TokenT *token) {
    free(token->text);
    free(token->type);
    free(token);
}


/*
 * Print a token. Used in main loop.
 */
void printToken(TokenT *token) {
    if(strcmp(token->type, "string literal") == 0) {
        printf("%s '%s'\n", token->type, token->text);
    } else {
        printf("%s \"%s\"\n", token->type, token->text);
    }
}


/******************************************/
/*************** Token States *************/
/******************************************/


TokenT *_invalid(TokenizerT *tk) {
    nextChar(tk);
    return makeToken(tk, "invalid token");
}

TokenT *_word(TokenizerT *tk) {
    nextChar(tk);
    if(isalnum(tk->inputIter[0]) || tk->inputIter[0] == '_') {
        return _word(tk);
    } else {
        if(isReservedWord(tk->tokenBuffer)) {
            return makeToken(tk, "reserved word");
        } else {
            return makeToken(tk, "word");
        }
    }
}

TokenT *_neq(TokenizerT *tk) {
    int atEndOfFile = nextChar(tk);
    if(atEndOfFile) {
        return makeToken(tk, "logical-not operator");
    }
    if(tk->inputIter[0] == '=') {
        nextChar(tk);
        return makeToken(tk, "not-equals operator");
    } else {
        return makeToken(tk, "logical-not operator");
    }
}

TokenT *_double_quote(TokenizerT *tk) {
    int atEndOfFile = nextChar(tk);
    while(tk->inputIter[0] != '"') {
        if(tk->inputIter[0] == '\\') {
            if(atEndOfFile) {
                return makeToken(tk, "unended string literal");
            }
            atEndOfFile = nextChar(tk);
        }
        if(atEndOfFile) {
            return makeToken(tk, "unended string literal");
        }
        atEndOfFile = nextChar(tk);
    }
    nextChar(tk);
    return makeToken(tk, "string literal");
}

TokenT *_mod(TokenizerT *tk) {
    nextChar(tk);
    if(tk->inputIter[0] == '=') {
        nextChar(tk);
        return makeToken(tk, "mod-equals operator");
    } else {
        return makeToken(tk, "mod operator");
    }
}

TokenT *_bit_and(TokenizerT *tk) {
    nextChar(tk);
    if(tk->inputIter[0] == '=') {
        nextChar(tk);
        return makeToken(tk, "bitwise-and-equals operator");
    } else if(tk->inputIter[0] == '&') {
        nextChar(tk);
        return makeToken(tk, "logical-and operator");
    } else {
        return makeToken(tk, "bitwise-and operator/address operator");
    }
}

/*
 * All 8 possibilities:
 *
 * 'EOF   --> invalid via EOF
 * 'cEOF  --> invalid via EOF
 * 'c'    --> valid
 * 'cc    --> invalid via structure
 * '\EOF  --> invalid via EOF
 * '\cEOF --> invalid via EOF
 * '\c'   --> valid
 * '\cc   --> invalid via structure
 *
 * incomplete char literals end abruptly.
 * invalid char literals don't close properly or at all.
 */
TokenT *_single_quote(TokenizerT *tk) {
    int atEndOfFile = nextChar(tk);
    if(atEndOfFile) {
        return makeToken(tk, "incomplete char literal");  // case: 'EOF
    }
    if(tk->inputIter[0] != '\\') {
        atEndOfFile = nextChar(tk);
        if(atEndOfFile) {
            return makeToken(tk, "incomplete char literal");  // case: 'cEOF
        }
        if(tk->inputIter[0] == '\'') {
            nextChar(tk);
            return makeToken(tk, "char literal");  // case: 'c'
        } else {  // must be invalid by structure, not just EOF
            nextChar(tk);
            return makeToken(tk, "invalid char literal");  // case: 'cc
        }
    } else {
        atEndOfFile = nextChar(tk);
        if(atEndOfFile) {
            return makeToken(tk, "incomplete char literal");  // case: '\EOF
        }
        atEndOfFile = nextChar(tk);
        if(atEndOfFile) {
            return makeToken(tk, "incomplete char literal");  // case: '\cEOF
        }
        if(tk->inputIter[0] == '\'') {
            nextChar(tk);
            return makeToken(tk, "char literal");  // case: '\c'
        } else {  // must be invalid by structure, not just EOF
            nextChar(tk);
            return makeToken(tk, "invalid char literal");  // case: '\cc
        }
    }
}

TokenT *_open_paren(TokenizerT *tk) {
    nextChar(tk);
    return makeToken(tk, "opening parenthesis");
}

TokenT *_close_paren(TokenizerT *tk) {
    nextChar(tk);
    return makeToken(tk, "closing parenthesis");
}

TokenT *_mult(TokenizerT *tk) {
    nextChar(tk);
    if(tk->inputIter[0] == '=') {
        nextChar(tk);
        return makeToken(tk, "multiply-equals operator");
    }
    return makeToken(tk, "multiplication operator/pointer operator");
}

TokenT *_plus(TokenizerT *tk) {
    nextChar(tk);
    if(tk->inputIter[0] == '=') {
        nextChar(tk);
        return makeToken(tk, "plus-equals operator");
    } else if(tk->inputIter[0] == '+') {
        nextChar(tk);
        return makeToken(tk, "increment operator");
    }
    return makeToken(tk, "addition operator");
}

TokenT *_comma(TokenizerT *tk) {
    nextChar(tk);
    return makeToken(tk, "comma");
}

TokenT *_minus(TokenizerT *tk) {
    nextChar(tk);
    if(tk->inputIter[0] == '=') {
        nextChar(tk);
        return makeToken(tk, "minus-equals operator");
    } else if(tk->inputIter[0] == '-') {
        nextChar(tk);
        return makeToken(tk, "decrement operator");
    } else if(tk->inputIter[0] == '>') {
        nextChar(tk);
        return makeToken(tk, "struct pointer operator");
    }
    return makeToken(tk, "subtraction operator");
}

TokenT *_dot(TokenizerT *tk) {
    nextChar(tk);
    return makeToken(tk, "struct member operator");
}

/*
 * Because the two functions below are for comments and NOT for tokens, we
 * can't store the string in the tokenBuffer. Comments could be arbitrarily
 * long and we don't have to recognize them, so it makes sense to skip them
 * while clearing the tokenBuffer at each new character.
 */
TokenT *_line_comment(TokenizerT *tk) {
    while(1) {
        nextChar(tk);
        clearBuffer(tk);
        if(tk->inputIter[0] == '\n') {
            nextChar(tk);
            clearBuffer(tk);
            return TKGetNextToken(tk);
        }
        if(tk->inputIter[0] == '\0') {
            return TKGetNextToken(tk);
        }
    }
}

/*
 * See above comment.
 */
TokenT *_block_comment(TokenizerT *tk) {
    while(1) {
        nextChar(tk);
        clearBuffer(tk);
        while(tk->inputIter[0] == '*') {
            nextChar(tk);
            clearBuffer(tk);
            if(tk->inputIter[0] == '/') {
                nextChar(tk);
                clearBuffer(tk);
                return TKGetNextToken(tk);
            }
        }
        if(tk->inputIter[0] == '\0') {
            return TKGetNextToken(tk);
        }
    }
}

TokenT *_div(TokenizerT *tk) {
    nextChar(tk);
    if(tk->inputIter[0] == '=') {
        nextChar(tk);
        return makeToken(tk, "divide-equals operator");
    }
    if(tk->inputIter[0] == '/') {
        nextChar(tk);
        return _line_comment(tk);
    }
    if(tk->inputIter[0] == '*') {
        nextChar(tk);
        return _block_comment(tk);
    }
    return makeToken(tk, "division operator");
}

TokenT *_ternary_colon(TokenizerT *tk) {
    nextChar(tk);
    return makeToken(tk, "ternary colon operator");
}

TokenT *_semicolon(TokenizerT *tk) {
    nextChar(tk);
    return makeToken(tk, "end-of-statement operator");
}

TokenT *_lt(TokenizerT *tk) {
    nextChar(tk);
    if(tk->inputIter[0] == '=') {
        nextChar(tk);
        return makeToken(tk, "less-than-or-equal-to operator");
    } else if(tk->inputIter[0] == '<') {
        nextChar(tk);
        if(tk->inputIter[0] == '=') {
            nextChar(tk);
            return makeToken(tk, "left-shift-equals operator");
        }
        return makeToken(tk, "left-shift operator");
    }
    return makeToken(tk, "less-than operator");
}

TokenT *_eq(TokenizerT *tk) {
    nextChar(tk);
    if(tk->inputIter[0] == '=') {
        nextChar(tk);
        return makeToken(tk, "equality operator");
    }
    return makeToken(tk, "assignment operator");
}

TokenT *_gt(TokenizerT *tk) {
    nextChar(tk);
    if(tk->inputIter[0] == '=') {
        nextChar(tk);
        return makeToken(tk, "greater-than-or-equal-to operator");
    } else if(tk->inputIter[0] == '>') {
        nextChar(tk);
        if(tk->inputIter[0] == '=') {
            nextChar(tk);
            return makeToken(tk, "right-shift-equals operator");
        }
        return makeToken(tk, "right-shift operator");
    }
    return makeToken(tk, "greater-than operator");
}

TokenT *_ternary_qmark(TokenizerT *tk) {
    nextChar(tk);
    return makeToken(tk, "ternary question mark operator");
}

TokenT *_open_bracket(TokenizerT *tk) {
    nextChar(tk);
    return makeToken(tk, "opening bracket");
}

TokenT *_close_bracket(TokenizerT *tk) {
    nextChar(tk);
    return makeToken(tk, "closing bracket");
}

TokenT *_bit_xor(TokenizerT *tk) {
    nextChar(tk);
    return makeToken(tk, "bitwise-xor operator");
}

TokenT *_open_brace(TokenizerT *tk) {
    nextChar(tk);
    return makeToken(tk, "opening brace");
}

TokenT *_bit_or(TokenizerT *tk) {
    nextChar(tk);
    if(tk->inputIter[0] == '|') {
        nextChar(tk);
        return makeToken(tk, "logical-or operator");
    } else if(tk->inputIter[0] == '=') {
        nextChar(tk);
        return makeToken(tk, "bitwise-or-equals operator");
    }
    return makeToken(tk, "bitwise-or operator");
}

TokenT *_close_brace(TokenizerT *tk) {
    nextChar(tk);
    return makeToken(tk, "closing brace");
}

TokenT *_bit_not(TokenizerT *tk) {
    nextChar(tk);
    if(tk->inputIter[0] == '=') {
        nextChar(tk);
        return makeToken(tk, "bitwise-not-equals operator");
    }
    return makeToken(tk, "bitwise-not operator");
}

TokenT *_pound(TokenizerT *tk) {
    nextChar(tk);
    return makeToken(tk, "include operator");
}

TokenT *_expofloat(TokenizerT *tk, int isFirst, int lastWasSign) {
    nextChar(tk);
    if(isdigit(tk->inputIter[0])) {
        return _expofloat(tk, 0, 0);
    } else if(tk->inputIter[0] == '+' || tk->inputIter[0] == '-') {
        if(isFirst) {
            return _expofloat(tk, 0, 1);
        } else if(lastWasSign) {
            return _invalid(tk);
        } else {
            return makeToken(tk, "float with exponent");
        }
    } else {
        if(isFirst) {
            return _invalid(tk);
        } else if(lastWasSign) {
            return _invalid(tk);
        } else {
            return makeToken(tk, "float with exponent");
        }
    }
}

TokenT *_float(TokenizerT *tk, int isFirst) {
    nextChar(tk);
    if(isdigit(tk->inputIter[0])) {
        return _float(tk, 0);
    } else if(tk->inputIter[0] == 'e' || tk->inputIter[0] == 'E') {
        return _expofloat(tk, 1, 0);
    } else {
        if(isFirst) {
            return _invalid(tk);
        } else {
            return makeToken(tk, "float");
        }
    }
}

TokenT *_octal(TokenizerT *tk) {
    nextChar(tk);
    if(isOctal(tk->inputIter[0])) {
        return _octal(tk);
    } else {
        return makeToken(tk, "octal integer");
    }
}

TokenT *_hex(TokenizerT *tk, int isFirst) {
    nextChar(tk);
    if(isxdigit(tk->inputIter[0])) {
        return _hex(tk, 0);
    } else {
        if(isFirst) {
            return _invalid(tk);
        } else {
            return makeToken(tk, "hexadecimal integer");
        }
    }
}

TokenT *_decimal(TokenizerT *tk) {
    nextChar(tk);
    if(isdigit(tk->inputIter[0])) {
        return _decimal(tk);
    } else if(tk->inputIter[0] == '.') {
        return _float(tk, 1);
    } else if(tk->inputIter[0] == 'e' || tk->inputIter[0] == 'E') {
        return _expofloat(tk, 1, 0);
    } else {
        return makeToken(tk, "decimal integer");
    }
}

/*
 * Handle being given a zero as the first char in a new token.
 */
TokenT *_zero(TokenizerT *tk) {
    nextChar(tk);
    if(isOctal(tk->inputIter[0])) {
        return _octal(tk);
    } else if(tk->inputIter[0] == 'x' || (tk->inputIter[0]) == 'X') {
        return _hex(tk, 1);
    } else if(tk->inputIter[0] == '.') {
        return _float(tk, 1);
    } else {
        return makeToken(tk, "zero integer");
    }
}

/*
 * TKGetNextToken returns the next token from the token stream as a
 * character string.  Space for the returned token should be dynamically
 * allocated.  The caller is responsible for freeing the space once it is
 * no longer needed.
 *
 * If the function succeeds, it returns a C string (delimited by '\0')
 * containing the token.  Else it returns 0.
 *
 * You need to fill in this function as part of your implementation.
 */
TokenT *TKGetNextToken(TokenizerT *tk) {
    clearBuffer(tk);
    char curr = tk->inputIter[0];

    // skip all whitespace before next token
    while(isspace(curr)) {
        nextChar(tk);
        clearBuffer(tk);
        curr = tk->inputIter[0];
    }

    if(curr == '\0') {
        return NULL;
    } else if(isalpha(curr) || curr == '_') {
        return _word(tk);
    } else if(curr == '0') {
        return _zero(tk);
    } else if(isdigit(curr)) {
        return _decimal(tk);
    } else if(curr == '!') { // neq
        return _neq(tk);
    } else if(curr == '"') { // double_quote
        return _double_quote(tk);
    } else if(curr == '#') {
        return _pound(tk);
    } else if(curr == '$') { // INVALID
        return _invalid(tk);
    } else if(curr == '%') { // mod, mod_eq
        return _mod(tk);
    } else if(curr == '&') { // bit_and, log_and, address (?)
        return _bit_and(tk);
    } else if(curr == '\'') { // single_quote
        return _single_quote(tk);
    } else if(curr == '(') { // open_paren
        return _open_paren(tk);
    } else if(curr == ')') { // close_paren
        return _close_paren(tk);
    } else if(curr == '*') { // mult, mult_eq, pointer (?)
        return _mult(tk);
    } else if(curr == '+') { // plus, plus_eq, inc
        return _plus(tk);
    } else if(curr == ',') { // comma
        return _comma(tk);
    } else if(curr == '-') { // minus, minus_eq, dec, struct_pointer
        return _minus(tk);
    } else if(curr == '.') { // dot
        return _dot(tk);
    } else if(curr == '/') { // div, div_eq
        return _div(tk);
    } else if(curr == ':') { // ternary_colon
        return _ternary_colon(tk);
    } else if(curr == ';') { // semicolon
        return _semicolon(tk);
    } else if(curr == '<') { // lt, lshift, lt_eq
        return _lt(tk);
    } else if(curr == '=') { // eq, assign
        return _eq(tk);
    } else if(curr == '>') { // gt, rshift, gt_eq
        return _gt(tk);
    } else if(curr == '?') { // ternary_qmark
        return _ternary_qmark(tk);
    } else if(curr == '@') { // INVALID
        return _invalid(tk);
    } else if(curr == '[') { // open_bracket
        return _open_bracket(tk);
    } else if(curr == '\\') { // backslash (?)
        return _invalid(tk);
    } else if(curr == ']') { // close_bracket
        return _close_bracket(tk);
    } else if(curr == '^') { // bit_xor
        return _bit_xor(tk);
    } else if(curr == '`') { // INVALID
        return _invalid(tk);
    } else if(curr == '{') { // open_brace
        return _open_brace(tk);
    } else if(curr == '|') { // bit_or, log_or
        return _bit_or(tk);
    } else if(curr == '}') { // close_brace
        return _close_brace(tk);
    } else if(curr == '~') { // bit_not
        return _bit_not(tk);
    } else {
        return _invalid(tk);
    }
}


/******************************************/
/************ End Token States ************/
/******************************************/


/*
 * main will have a string argument (in argv[1]).
 * The string argument contains the tokens.
 * Print out the tokens in the second string in left-to-right order.
 * Each token should be printed on a separate line.
 */
int main(int argc, char **argv) {
    // make sure theres exactly 1 argument
    if(argc != 2) {
        printf("Invalid number of arguments. \n");
        printf("Usage: \n    ./tokenizer <C-code string>\n");
        exit(1);
    }

    TokenizerT *tokenizer = TKCreate(argv[1]);

    TokenT *token;
    while((token = TKGetNextToken(tokenizer)) != NULL) {
        printToken(token);
        destroyToken(token);
    }

    TKDestroy(tokenizer);

    return 0;
}