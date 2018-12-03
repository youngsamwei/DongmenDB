#ifndef __LITERAL_H_
#define __LITERAL_H_

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


#endif
