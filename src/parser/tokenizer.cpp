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
#include "parser/tokenizer.h"


int isOctal(char curr) {
    if (curr >= '0' && curr <= '7') {
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
TokenizerT *TKCreate(const char *ts) {
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
    newTokenizer->offset = 0;
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
    tk->offset++;
    tk->bufferIter[0] = tk->inputIter[0];  // copy new char to end of buffer
    tk->inputIter++;                       // move the input iterator over
    tk->bufferIter++;                      // move the buffer iterator over
    tk->bufferIter[0] = '\0';              // add a null to the end of buffer
    tk->bufferSize++;                      // increase bufferSize accordingly
    if (tk->bufferSize == 999) {
        // TODO: exit gracefully
        exit(1);
    }

    assert(strlen(tk->bufferIter) == 0);

    int nextIsNull = 0;
    if (tk->inputIter[0] == '\0') {
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
    const char *reservedWords[] = {
            "select", "from", "where", "order", "by", "group", "create", "table", "index", "and", "not", "or", "null",
            "like", "in", "grant", "integer", "int", "char", "values", "insert", "into", "update", "delete", "set", "on",
            "user", "view", "rule", "default", "check", "between", "trigger", "primary", "key", "foreign"
    };
    int isReservedWord = 0;
    int size = sizeof(reservedWords)/sizeof(1);
    for (int rWordIndex = 0; rWordIndex <= size - 1; rWordIndex++) {
        if (stricmp(word, reservedWords[rWordIndex]) == 0) {
            isReservedWord = 1;
        }
    }

    return isReservedWord;
}


/*
 * Make a token struct from the current state of the tokenizer and the
 * identified type.
 */
TokenT *makeToken(TokenizerT *tk, TokenType type) {
    TokenT *token = (TokenT *) malloc(sizeof(TokenT));

    token->type = type;
    token->text = (char *) malloc(sizeof(char) * 1000);

    /*如果是字符串，需要从token->text去掉单引号*/
    if (type == TOKEN_STRING){
        char *v = tk->tokenBuffer + 1;
        int len = strlen(tk->tokenBuffer) - 1;
        strcpy(token->text, v);
        token->text[len-1] = '\0';
    } else {
        strcpy(token->text, tk->tokenBuffer);
    }

    return token;
}


void destroyToken(TokenT *token) {
    free(token->text);
    free(token);
}


/*
 * Print a token. Used in main loop.
 */
void printToken(TokenT *token) {
    if (token->type == TOKEN_STRING) {
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
    return makeToken(tk, TOKEN_INVALID);
}

TokenT *_word(TokenizerT *tk) {
    nextChar(tk);
    if (isalnum(tk->inputIter[0]) || tk->inputIter[0] == '_'
        || tk->inputIter[0] == '.' || tk->inputIter[0] == '*') {
        /* . 对象限定符, * 对象通配符，如 student.sno 表示student表中的sno字段, sc.* 表示sc表中的所有字段  */
        return _word(tk);
    } else {
        if (isReservedWord(tk->tokenBuffer)) {
            if (strcasecmp(tk->tokenBuffer, "AND") == 0) {
                return makeToken(tk, TOKEN_AND);
            } else if (strcasecmp(tk->tokenBuffer, "NOT") == 0) {
                return makeToken(tk, TOKEN_NOT);
            } else if (strcasecmp(tk->tokenBuffer, "OR") == 0) {
                return makeToken(tk, TOKEN_OR);
            } else if (strcasecmp(tk->tokenBuffer, "NULL") == 0) {
                return makeToken(tk, TOKEN_NULL);
            } else if (strcasecmp(tk->tokenBuffer, "LIKE") == 0) {
                return makeToken(tk, TOKEN_LIKE);
            } else if (strcasecmp(tk->tokenBuffer, "IN") == 0) {
                return makeToken(tk, TOKEN_IN);
            }
            /*TODO:需要处理函数*/
            return makeToken(tk, TOKEN_RESERVED_WORD);
        } else {
            return makeToken(tk, TOKEN_WORD); //标识符
        }
    }
}

/* != */
TokenT *_neq(TokenizerT *tk) {
    nextChar(tk);
    if (tk->inputIter[0] == '=') {
        nextChar(tk);
        return makeToken(tk, TOKEN_NOT_EQUAL);
    }
}

TokenT *_double_quote(TokenizerT *tk) {
    int atEndOfFile = nextChar(tk);
    while (tk->inputIter[0] != '"') {
        if (tk->inputIter[0] == '\\') {
            if (atEndOfFile) {
                return makeToken(tk, TOKEN_UNENDED_SRING);
            }
            atEndOfFile = nextChar(tk);
        }
        if (atEndOfFile) {
            return makeToken(tk, TOKEN_UNENDED_SRING);
        }
        atEndOfFile = nextChar(tk);
    }
    nextChar(tk);
    return makeToken(tk, TOKEN_STRING);
}

TokenT *_mod(TokenizerT *tk) {
    nextChar(tk);
    return makeToken(tk, TOKEN_MOD);

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
/* 使用单引号标注字符串*/
TokenT *_single_quote(TokenizerT *tk) {
    int atEndOfFile = nextChar(tk);
    while (tk->inputIter[0] != '\'') {
        if (tk->inputIter[0] == '\\') {
            if (atEndOfFile) {
                return makeToken(tk, TOKEN_UNENDED_SRING);
            }
            atEndOfFile = nextChar(tk);
        }
        if (atEndOfFile) {
            return makeToken(tk, TOKEN_UNENDED_SRING);
        }
        atEndOfFile = nextChar(tk);
    }
    nextChar(tk);
    return makeToken(tk, TOKEN_STRING);
    /*
    int atEndOfFile = nextChar(tk);
    if (atEndOfFile) {
        return makeToken(tk, TOKEN_INCOMPLETE_CHAR);  // case: 'EOF
    }
    if (tk->inputIter[0] != '\\') {
        atEndOfFile = nextChar(tk);
        if (atEndOfFile) {
            return makeToken(tk, TOKEN_INCOMPLETE_CHAR);  // case: 'cEOF
        }
        if (tk->inputIter[0] == '\'') {
            nextChar(tk);
            return makeToken(tk, TOKEN_CHAR);  // case: 'c'
        } else {  // must be invalid by structure, not just EOF
            nextChar(tk);
            return makeToken(tk, TOKEN_INVALID_CHAR);  // case: 'cc
        }
    } else {
        atEndOfFile = nextChar(tk);
        if (atEndOfFile) {
            return makeToken(tk, TOKEN_INCOMPLETE_CHAR);  // case: '\EOF
        }
        atEndOfFile = nextChar(tk);
        if (atEndOfFile) {
            return makeToken(tk, TOKEN_INCOMPLETE_CHAR);  // case: '\cEOF
        }
        if (tk->inputIter[0] == '\'') {
            nextChar(tk);
            return makeToken(tk, TOKEN_CHAR);  // case: '\c'
        } else {  // must be invalid by structure, not just EOF
            nextChar(tk);
            return makeToken(tk, TOKEN_INVALID_CHAR);  // case: '\cc
        }
    }
     */
}

TokenT *_open_paren(TokenizerT *tk) {
    nextChar(tk);
    return makeToken(tk, TOKEN_OPEN_PAREN);
}

TokenT *_close_paren(TokenizerT *tk) {
    nextChar(tk);
    return makeToken(tk, TOKEN_CLOSE_PAREN);
}

/* TODO: 需要解析 在select子句中单独出现的*，但需要上下文才能处理 */
TokenT *_mult(TokenizerT *tk) {
    nextChar(tk);
    return makeToken(tk, TOKEN_MULTIPLY);
}

TokenT *_plus(TokenizerT *tk) {
    nextChar(tk);
    return makeToken(tk, TOKEN_PLUS);
}

TokenT *_comma(TokenizerT *tk) {
    nextChar(tk);
    return makeToken(tk, TOKEN_COMMA);
}

TokenT *_minus(TokenizerT *tk) {
    nextChar(tk);
    return makeToken(tk, TOKEN_MINUS);
}

/*
 * Because the two functions below are for comments and NOT for tokens, we
 * can't store the string in the tokenBuffer. Comments could be arbitrarily
 * long and we don't have to recognize them, so it makes sense to skip them
 * while clearing the tokenBuffer at each new character.
 */
TokenT *_line_comment(TokenizerT *tk) {
    while (1) {
        nextChar(tk);
        clearBuffer(tk);
        if (tk->inputIter[0] == '\n') {
            nextChar(tk);
            clearBuffer(tk);
            return TKGetNextToken(tk);
        }
        if (tk->inputIter[0] == '\0') {
            return TKGetNextToken(tk);
        }
    }
}

/*
 * See above comment.
 */
TokenT *_block_comment(TokenizerT *tk) {
    while (1) {
        nextChar(tk);
        clearBuffer(tk);
        while (tk->inputIter[0] == '*') {
            nextChar(tk);
            clearBuffer(tk);
            if (tk->inputIter[0] == '/') {
                nextChar(tk);
                clearBuffer(tk);
                return TKGetNextToken(tk);
            }
        }
        if (tk->inputIter[0] == '\0') {
            return TKGetNextToken(tk);
        }
    }
}

TokenT *_div(TokenizerT *tk) {
    nextChar(tk);

    if (tk->inputIter[0] == '/') {
        nextChar(tk);
        return _line_comment(tk);
    }
    if (tk->inputIter[0] == '*') {
        nextChar(tk);
        return _block_comment(tk);
    }
    return makeToken(tk, TOKEN_DIVIDE);
}

TokenT *_semicolon(TokenizerT *tk) {
    nextChar(tk);
    return makeToken(tk, TOKEN_SEMICOLON);
}

/* <= , < */
TokenT *_lt(TokenizerT *tk) {
    nextChar(tk);
    if (tk->inputIter[0] == '=') {
        nextChar(tk);
        return makeToken(tk, TOKEN_LE);
    }
    return makeToken(tk, TOKEN_LT);
}

/*不解析赋值语句*/
TokenT *_eq(TokenizerT *tk) {
    nextChar(tk);
    return makeToken(tk, TOKEN_EQ);
}

TokenT *_gt(TokenizerT *tk) {
    nextChar(tk);
    if (tk->inputIter[0] == '=') {
        nextChar(tk);
        return makeToken(tk, TOKEN_GE);
    }
    return makeToken(tk, TOKEN_GT);
}

TokenT *_expofloat(TokenizerT *tk, int isFirst, int lastWasSign) {
    nextChar(tk);
    if (isdigit(tk->inputIter[0])) {
        return _expofloat(tk, 0, 0);
    } else if (tk->inputIter[0] == '+' || tk->inputIter[0] == '-') {
        if (isFirst) {
            return _expofloat(tk, 0, 1);
        } else if (lastWasSign) {
            return _invalid(tk);
        } else {
            return makeToken(tk, TOKEN_EXP_FLOAT);
        }
    } else {
        if (isFirst) {
            return _invalid(tk);
        } else if (lastWasSign) {
            return _invalid(tk);
        } else {
            return makeToken(tk, TOKEN_EXP_FLOAT);
        }
    }
}

TokenT *_float(TokenizerT *tk, int isFirst) {
    nextChar(tk);
    if (isdigit(tk->inputIter[0])) {
        return _float(tk, 0);
    } else if (tk->inputIter[0] == 'e' || tk->inputIter[0] == 'E') {
        return _expofloat(tk, 1, 0);
    } else {
        if (isFirst) {
            return _invalid(tk);
        } else {
            return makeToken(tk, TOKEN_FLOAT);
        }
    }
}

TokenT *_octal(TokenizerT *tk) {
    nextChar(tk);
    if (isOctal(tk->inputIter[0])) {
        return _octal(tk);
    } else {
        return makeToken(tk, TOKEN_OCTAL);
    }
}

TokenT *_hex(TokenizerT *tk, int isFirst) {
    nextChar(tk);
    if (isxdigit(tk->inputIter[0])) {
        return _hex(tk, 0);
    } else {
        if (isFirst) {
            return _invalid(tk);
        } else {
            return makeToken(tk, TOKEN_HEX);
        }
    }
}

TokenT *_decimal(TokenizerT *tk) {
    nextChar(tk);
    if (isdigit(tk->inputIter[0])) {
        return _decimal(tk);
    } else if (tk->inputIter[0] == '.') {
        return _float(tk, 1);
    } else if (tk->inputIter[0] == 'e' || tk->inputIter[0] == 'E') {
        return _expofloat(tk, 1, 0);
    } else {
        return makeToken(tk, TOKEN_DECIMAL);
    }
}

/*
 * Handle being given a zero as the first char in a new token.
 */
TokenT *_zero(TokenizerT *tk) {
    nextChar(tk);
    if (isOctal(tk->inputIter[0])) {
        return _octal(tk);
    } else if (tk->inputIter[0] == 'x' || (tk->inputIter[0]) == 'X') {
        return _hex(tk, 1);
    } else if (tk->inputIter[0] == '.') {
        return _float(tk, 1);
    } else {
        return makeToken(tk, TOKEN_ZERO);
    }
}

TokenT *_power(TokenizerT *tk) {
    nextChar(tk);
    return makeToken(tk, TOKEN_POWER);
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
    while (isspace(curr)) {
        nextChar(tk);
        clearBuffer(tk);
        curr = tk->inputIter[0];
    }

    if (curr == '\0') {
        return NULL;
    } else if (isalpha(curr) || curr == '_') {
        return _word(tk);
    } else if (curr == '0') {
        return _zero(tk);
    } else if (isdigit(curr)) {
        return _decimal(tk);
    } else if (curr == '!') { // neq
        return _neq(tk);
    } else if (curr == '"') { // double_quote
        return _double_quote(tk);
    } else if (curr == '%') { // mod, mod_eq
        return _mod(tk);
    } else if (curr == '\'') { // single_quote
        return _single_quote(tk);
    } else if (curr == '(') { // open_paren
        return _open_paren(tk);
    } else if (curr == ')') { // close_paren
        return _close_paren(tk);
    } else if (curr == '*') { // mult, mult_eq, pointer (?)
        return _mult(tk);
    } else if (curr == '+') { // plus, plus_eq, inc
        return _plus(tk);
    } else if (curr == ',') { // comma
        return _comma(tk);
    } else if (curr == '-') { // minus, minus_eq, dec, struct_pointer
        return _minus(tk);
    } else if (curr == '/') { // div, div_eq
        return _div(tk);
    } else if (curr == ';') { // semicolon
        return _semicolon(tk);
    } else if (curr == '<') { // lt, lshift, lt_eq
        return _lt(tk);
    } else if (curr == '=') { // eq, assign
        return _eq(tk);
    } else if (curr == '>') { // gt, rshift, gt_eq
        return _gt(tk);
    } else if (curr == '^') {
        return _power(tk);
    } else {
        return _invalid(tk);
    }
}


/******************************************/
/************ End Token States ************/
/******************************************/

