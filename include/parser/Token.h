//
// Created by sam on 2018/11/30.
//

#ifndef DONGMENDB_TOKEN_H
#define DONGMENDB_TOKEN_H

#include <stdlib.h>
#include <string.h>

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

class Token{
public:
    char* text;
    TokenType type;

    Token(char* text, TokenType type){
        this->type = type;
        this->text = (char *) malloc(sizeof(char) * 1000);

        /*如果是字符串，需要从token->text去掉单引号*/
        if (type == TOKEN_STRING){
            char *v = text + 1;
            int len = strlen(text) - 1;
            strcpy(this->text, v);
            this->text[len-1] = '\0';
        } else {
            strcpy(this->text, text);
        }

    }
};

#endif //DONGMENDB_TOKEN_H
