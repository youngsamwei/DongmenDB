//
// Created by sam on 2018/11/30.
//

#ifndef DONGMENDB_TOKEN_H
#define DONGMENDB_TOKEN_H

#include <stdlib.h>
#include <string.h>

/*与operators中的排序一致，方便按照顺序访问operators*/
typedef enum TokenType_ {
  TOKEN_OPEN_PAREN = 0,    //左括号
  TOKEN_CLOSE_PAREN,       //右括号
  TOKEN_POWER,             //
  TOKEN_PLUS,              //加号
  TOKEN_MINUS,             //减号
  TOKEN_DIVIDE,            //除
  TOKEN_MULTIPLY,          //乘
  TOKEN_LT,                //less-than operator 小于
  TOKEN_GT,                //大于
  TOKEN_EQ,                //等于
  TOKEN_NOT_EQUAL,         //不等于
  TOKEN_LE,                //小于等于 less-than-or-equal-to operator"
  TOKEN_GE,                //大于等于
  TOKEN_IN,                //in运算
  TOKEN_LIKE,              //like运算
  TOKEN_AND,
  TOKEN_OR,
  TOKEN_NOT,               //匹配not
  TOKEN_ASSIGNMENT,        //赋值
  TOKEN_FUN,               //函数
  TOKEN_COMMA,             /*以上是操作符，在表达式解析时使用*/ //逗号
  TOKEN_INVALID, //"invalid token"
  TOKEN_RESERVED_WORD,  //sql中的保留字，比如create,set,where 等等
  TOKEN_WORD,           //非保留字的词
  TOKEN_UNENDED_SRING,  //没有结尾的字符串
  TOKEN_STRING,         //字符串
  TOKEN_MOD,            //模
  TOKEN_INCOMPLETE_CHAR,//不完整的字符
  TOKEN_CHAR,           //字符
  TOKEN_INVALID_CHAR,   //非法字符
  TOKEN_SEMICOLON,      //分号
  TOKEN_EXP_FLOAT,
  TOKEN_FLOAT,
  TOKEN_OCTAL,               //"octal integer"
  TOKEN_HEX,                 //"hexadecimal integer"
  TOKEN_DECIMAL,            //"decimal integer"
  TOKEN_ZERO,               //"zero integer"
  TOKEN_NULL                 //匹配null
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
