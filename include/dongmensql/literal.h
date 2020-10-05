#ifndef __LITERAL_H_
#define __LITERAL_H_


/* sql语句中常量值的定义
 * 分为三类：字符串、整型、double型
 *
 * 基础类Literal，原始值都保存在Literal的original_value中。
 *
 *
 * */
#include "common.h"

class Literal{
public:
    enum data_type t;
    char *original_value;

    Literal(enum data_type t, char *original_value){
        this->t = t;
        this->original_value = strdup(original_value);
    }

    Literal(enum data_type t){
        this->t = t;
        this->original_value = NULL;
    }
};

class IntLiteral : public Literal{
public:
    int value;

    IntLiteral(enum data_type t, char *original_value) : Literal(t, original_value){
        this->value= atoi(original_value);
    }
};

class DoubleLiteral : public Literal{
public:
    double value;

    DoubleLiteral(enum data_type t, char *original_value) : Literal(t, original_value){
        this->value= atof(original_value);
    }
};

std::ostream &operator<<(std::ostream &os, Literal *val);

#endif
