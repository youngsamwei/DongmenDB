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
#include "parser/Tokenizer.h"

using namespace std;

/* 创建词法解析器
 * TKCreate creates a new Tokenizer object for a given token stream
 * (given as a string).
 *
 * TKCreate should copy the arguments so that it is not dependent on
 * them staying immutable after returning.  (In the future, this may change
 * to increase efficiency.)
 *
 * If the function succeeds, it returns a non-NULL Tokenizer.
 * Else it returns NULL.
 *
 * You need to fill in this function as part of your implementation.
 */
Tokenizer::Tokenizer(const char *ts) {
    // TODO: use strcpy to copy input stream ts

    long int streamSize = strlen(ts) + 2;
    inputStream = (char *) malloc(sizeof(char) * streamSize);
    strcpy(inputStream, ts);
    inputIter = inputStream;

    bufferSize = 0;
    tokenBuffer = (char *) malloc(sizeof(char) * 1000);
    strcpy(tokenBuffer, "");  // end the buffer with this null byte
    bufferIter = tokenBuffer;
    offset = 0;
}


/*
 * TKDestroy destroys a Tokenizer object.  It should free all dynamically
 * allocated memory that is part of the object being destroyed.
 *
 * You need to fill in this function as part of your implementation.
 */
Tokenizer::~Tokenizer() {
    free(inputStream);
    free(tokenBuffer);
}


/*
 * Copy one character from the inputStream to the tokenBuffer. Then, move the
 * iterators forward to the next character.
 *
 * this->inputIter[0] contains the character to be added to the current token.
 * this->bufferIter[0] contains the most recent character added to the token.
 *
 * Returns 1 if inputIter[0] is '\0', else 0.
 */
int Tokenizer::nextChar() {
    this->offset++;
    this->bufferIter[0] = this->inputIter[0];  // copy new char to end of buffer
    this->inputIter++;                       // move the input iterator over
    this->bufferIter++;                      // move the buffer iterator over
    this->bufferIter[0] = '\0';              // add a null to the end of buffer
    this->bufferSize++;                      // increase bufferSize accordingly
    if (this->bufferSize == 999) {
        // TODO: exit gracefully
        exit(1);
    }

    assert(strlen(this->bufferIter) == 0);

    int nextIsNull = 0;
    if (this->inputIter[0] == '\0') {
        nextIsNull = 1;
    }
    return nextIsNull;
}

void Tokenizer::clearBuffer() {
    this->bufferSize = 0;
    this->bufferIter = this->tokenBuffer;
    this->bufferIter[0] = '\0';  // immediately end the string

    assert(strlen(this->tokenBuffer) == 0);
}


int Tokenizer::isOctal(char curr) {
    if (curr >= '0' && curr <= '7') {
        return 1;
    } else {
        return 0;
    }
}


/*
 * Returns 1 if a word is a reserved word. Otherwise, returns 0.
 */
int Tokenizer::isReservedWord(char *word) {
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
 * Print a token. Used in main loop.
 */
void Tokenizer::printToken(Token *token) {
    if (token->type == TOKEN_STRING) {
        printf("%s '%s'\n", token->type, token->text);
    } else {
        printf("%s \"%s\"\n", token->type, token->text);
    }
}

Token *Tokenizer::makeToken(TokenType type){
    return new Token(this->tokenBuffer, type);
};

/******************************************/
/*************** Token States *************/
/******************************************/


Token *Tokenizer::_invalid() {
    nextChar();
    return makeToken( TOKEN_INVALID);
}

Token *Tokenizer::_word() {
    nextChar();
    if (isalnum(this->inputIter[0]) || this->inputIter[0] == '_'
        || this->inputIter[0] == '.' || this->inputIter[0] == '*') {
        /* . 对象限定符, * 对象通配符，如 student.sno 表示student表中的sno字段, sc.* 表示sc表中的所有字段  */
        return _word();
    } else {
        if (isReservedWord(this->tokenBuffer)) {
            if (strcasecmp(this->tokenBuffer, "AND") == 0) {
                return makeToken( TOKEN_AND);
            } else if (strcasecmp(this->tokenBuffer, "NOT") == 0) {
                return makeToken( TOKEN_NOT);
            } else if (strcasecmp(this->tokenBuffer, "OR") == 0) {
                return makeToken( TOKEN_OR);
            } else if (strcasecmp(this->tokenBuffer, "NULL") == 0) {
                return makeToken( TOKEN_NULL);
            } else if (strcasecmp(this->tokenBuffer, "LIKE") == 0) {
                return makeToken( TOKEN_LIKE);
            } else if (strcasecmp(this->tokenBuffer, "IN") == 0) {
                return makeToken( TOKEN_IN);
            }
            /*TODO:需要处理函数*/
            return makeToken( TOKEN_RESERVED_WORD);
        } else {
            return makeToken( TOKEN_WORD); //标识符
        }
    }
}

/* != */
Token *Tokenizer::_neq() {
    nextChar();
    if (this->inputIter[0] == '=') {
        nextChar();
        return makeToken( TOKEN_NOT_EQUAL);
    }
}

Token *Tokenizer::_double_quote() {
    int atEndOfFile = nextChar();
    while (this->inputIter[0] != '"') {
        if (this->inputIter[0] == '\\') {
            if (atEndOfFile) {
                return makeToken( TOKEN_UNENDED_SRING);
            }
            atEndOfFile = nextChar();
        }
        if (atEndOfFile) {
            return makeToken( TOKEN_UNENDED_SRING);
        }
        atEndOfFile = nextChar();
    }
    nextChar();
    return makeToken( TOKEN_STRING);
}

Token *Tokenizer::_mod() {
    nextChar();
    return makeToken( TOKEN_MOD);

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
Token *Tokenizer::_single_quote() {
    int atEndOfFile = nextChar();
    while (this->inputIter[0] != '\'') {
        if (this->inputIter[0] == '\\') {
            if (atEndOfFile) {
                return makeToken( TOKEN_UNENDED_SRING);
            }
            atEndOfFile = nextChar();
        }
        if (atEndOfFile) {
            return makeToken( TOKEN_UNENDED_SRING);
        }
        atEndOfFile = nextChar();
    }
    nextChar();
    return makeToken( TOKEN_STRING);
    /*
    int atEndOfFile = nextChar();
    if (atEndOfFile) {
        return makeToken( TOKEN_INCOMPLETE_CHAR);  // case: 'EOF
    }
    if (this->inputIter[0] != '\\') {
        atEndOfFile = nextChar();
        if (atEndOfFile) {
            return makeToken( TOKEN_INCOMPLETE_CHAR);  // case: 'cEOF
        }
        if (this->inputIter[0] == '\'') {
            nextChar();
            return makeToken( TOKEN_CHAR);  // case: 'c'
        } else {  // must be invalid by structure, not just EOF
            nextChar();
            return makeToken( TOKEN_INVALID_CHAR);  // case: 'cc
        }
    } else {
        atEndOfFile = nextChar();
        if (atEndOfFile) {
            return makeToken( TOKEN_INCOMPLETE_CHAR);  // case: '\EOF
        }
        atEndOfFile = nextChar();
        if (atEndOfFile) {
            return makeToken( TOKEN_INCOMPLETE_CHAR);  // case: '\cEOF
        }
        if (this->inputIter[0] == '\'') {
            nextChar();
            return makeToken( TOKEN_CHAR);  // case: '\c'
        } else {  // must be invalid by structure, not just EOF
            nextChar();
            return makeToken( TOKEN_INVALID_CHAR);  // case: '\cc
        }
    }
     */
}

Token *Tokenizer::_open_paren() {
    nextChar();
    return makeToken( TOKEN_OPEN_PAREN);
}

Token *Tokenizer::_close_paren() {
    nextChar();
    return makeToken( TOKEN_CLOSE_PAREN);
}

/* TODO: 需要解析 在select子句中单独出现的*，但需要上下文才能处理 */
Token *Tokenizer::_mult() {
    nextChar();
    return makeToken( TOKEN_MULTIPLY);
}

Token *Tokenizer::_plus() {
    nextChar();
    return makeToken( TOKEN_PLUS);
}

Token *Tokenizer::_comma() {
    nextChar();
    return makeToken( TOKEN_COMMA);
}

Token *Tokenizer::_minus() {
    nextChar();
    return makeToken( TOKEN_MINUS);
}

/*
 * Because the two functions below are for comments and NOT for tokens, we
 * can't store the string in the tokenBuffer. Comments could be arbitrarily
 * long and we don't have to recognize them, so it makes sense to skip them
 * while clearing the tokenBuffer at each new character.
 */
Token *Tokenizer::_line_comment() {
    while (1) {
        nextChar();
        clearBuffer();
        if (this->inputIter[0] == '\n') {
            nextChar();
            clearBuffer();
            return getNextToken();
        }
        if (this->inputIter[0] == '\0') {
            return getNextToken();
        }
    }
}

/*
 * See above comment.
 */
Token *Tokenizer::_block_comment() {
    while (1) {
        nextChar();
        clearBuffer();
        while (this->inputIter[0] == '*') {
            nextChar();
            clearBuffer();
            if (this->inputIter[0] == '/') {
                nextChar();
                clearBuffer();
                return getNextToken();
            }
        }
        if (this->inputIter[0] == '\0') {
            return getNextToken();
        }
    }
}

Token *Tokenizer::_div() {
    nextChar();

    if (this->inputIter[0] == '/') {
        nextChar();
        return _line_comment();
    }
    if (this->inputIter[0] == '*') {
        nextChar();
        return _block_comment();
    }
    return makeToken( TOKEN_DIVIDE);
}

Token *Tokenizer::_semicolon() {
    nextChar();
    return makeToken( TOKEN_SEMICOLON);
}

/* <= , < */
Token *Tokenizer::_lt() {
    nextChar();
    if (this->inputIter[0] == '=') {
        nextChar();
        return makeToken( TOKEN_LE);
    }
    return makeToken( TOKEN_LT);
}

/*不解析赋值语句*/
Token *Tokenizer::_eq() {
    nextChar();
    return makeToken( TOKEN_EQ);
}

Token *Tokenizer::_gt() {
    nextChar();
    if (this->inputIter[0] == '=') {
        nextChar();
        return makeToken( TOKEN_GE);
    }
    return makeToken( TOKEN_GT);
}

Token *Tokenizer::_expofloat( int isFirst, int lastWasSign) {
    nextChar();
    if (isdigit(this->inputIter[0])) {
        return _expofloat( 0, 0);
    } else if (this->inputIter[0] == '+' || this->inputIter[0] == '-') {
        if (isFirst) {
            return _expofloat( 0, 1);
        } else if (lastWasSign) {
            return _invalid();
        } else {
            return makeToken( TOKEN_EXP_FLOAT);
        }
    } else {
        if (isFirst) {
            return _invalid();
        } else if (lastWasSign) {
            return _invalid();
        } else {
            return makeToken( TOKEN_EXP_FLOAT);
        }
    }
}

Token *Tokenizer::_float( int isFirst) {
    nextChar();
    if (isdigit(this->inputIter[0])) {
        return _float( 0);
    } else if (this->inputIter[0] == 'e' || this->inputIter[0] == 'E') {
        return _expofloat( 1, 0);
    } else {
        if (isFirst) {
            return _invalid();
        } else {
            return makeToken( TOKEN_FLOAT);
        }
    }
}

Token *Tokenizer::_octal() {
    nextChar();
    if (isOctal(this->inputIter[0])) {
        return _octal();
    } else {
        return makeToken( TOKEN_OCTAL);
    }
}

Token *Tokenizer::_hex( int isFirst) {
    nextChar();
    if (isxdigit(this->inputIter[0])) {
        return _hex( 0);
    } else {
        if (isFirst) {
            return _invalid();
        } else {
            return makeToken( TOKEN_HEX);
        }
    }
}

Token *Tokenizer::_decimal() {
    nextChar();
    if (isdigit(this->inputIter[0])) {
        return _decimal();
    } else if (this->inputIter[0] == '.') {
        return _float( 1);
    } else if (this->inputIter[0] == 'e' || this->inputIter[0] == 'E') {
        return _expofloat( 1, 0);
    } else {
        return makeToken( TOKEN_DECIMAL);
    }
}

/*
 * Handle being given a zero as the first char in a new token.
 */
Token *Tokenizer::_zero() {
    nextChar();
    if (isOctal(this->inputIter[0])) {
        return _octal();
    } else if (this->inputIter[0] == 'x' || (this->inputIter[0]) == 'X') {
        return _hex( 1);
    } else if (this->inputIter[0] == '.') {
        return _float( 1);
    } else {
        return makeToken( TOKEN_ZERO);
    }
}

Token *Tokenizer::_power() {
    nextChar();
    return makeToken( TOKEN_POWER);
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
Token *Tokenizer::getNextToken() {
    clearBuffer();
    char curr = this->inputIter[0];

    // skip all whitespace before next token
    while (isspace(curr)) {
        nextChar();
        clearBuffer();
        curr = this->inputIter[0];
    }

    if (curr == '\0') {
        return NULL;
    } else if (isalpha(curr) || curr == '_') {
        return _word();
    } else if (curr == '0') {
        return _zero();
    } else if (isdigit(curr)) {
        return _decimal();
    } else if (curr == '!') { // neq
        return _neq();
    } else if (curr == '"') { // double_quote
        return _double_quote();
    } else if (curr == '%') { // mod, mod_eq
        return _mod();
    } else if (curr == '\'') { // single_quote
        return _single_quote();
    } else if (curr == '(') { // open_paren
        return _open_paren();
    } else if (curr == ')') { // close_paren
        return _close_paren();
    } else if (curr == '*') { // mult, mult_eq, pointer (?)
        return _mult();
    } else if (curr == '+') { // plus, plus_eq, inc
        return _plus();
    } else if (curr == ',') { // comma
        return _comma();
    } else if (curr == '-') { // minus, minus_eq, dec, struct_pointer
        return _minus();
    } else if (curr == '/') { // div, div_eq
        return _div();
    } else if (curr == ';') { // semicolon
        return _semicolon();
    } else if (curr == '<') { // lt, lshift, lt_eq
        return _lt();
    } else if (curr == '=') { // eq, assign
        return _eq();
    } else if (curr == '>') { // gt, rshift, gt_eq
        return _gt();
    } else if (curr == '^') {
        return _power();
    } else {
        return _invalid();
    }
}


/******************************************/
/************ End Token States ************/
/******************************************/

