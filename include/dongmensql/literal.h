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

union LitVal {
    int ival;
    double dval;
    char cval;
    char *strval;
};

typedef struct Literal_t {
    enum data_type t;
    union LitVal val;
    struct Literal_t *next; /* linked list */
} Literal_t;

Literal_t *litInt(int i);
Literal_t *litDouble(double d);
Literal_t *litChar(char c);
Literal_t *litText(char *str);
Literal_t *Literal_append(Literal_t *val, Literal_t *toAppend);

void Literal_free(Literal_t *lval);
void Literal_freeList(Literal_t *lval);

void Literal_print(Literal_t *val);
void Literal_printList(Literal_t *val);


Literal_t *newLiteral(enum data_type t);



#endif
