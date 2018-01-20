//
// Created by Sam on 2018/1/17.
//

#ifndef DONGMENDB_LITERAL_H
#define DONGMENDB_LITERAL_H

typedef union {
    int ival;
    double dval;
    char cval;
    char *strval;
} LiteralVal;

typedef enum  {
    TYPE_INT,
    TYPE_DOUBLE,
    TYPE_CHAR,
    TYPE_TEXT
} data_type;

typedef struct Literal_ Literal;

typedef struct Literal_ {
    data_type t;
    LiteralVal val;
    Literal *next; /* linked list */
} Literal;

Literal *litInt(int i);
Literal *litDouble(double d);
Literal *litChar(char c);
Literal *litText(char *str);
Literal *Literal_append(Literal *val, Literal *toAppend);

void Literal_free(Literal *lval);
void Literal_freeList(Literal *lval);

void Literal_print(Literal *val);
void Literal_printList(Literal *val);


Literal *newLiteral(data_type t);
#endif //DONGMENDB_LITERAL_H
