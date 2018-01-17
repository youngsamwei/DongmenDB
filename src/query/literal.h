//
// Created by Sam on 2018/1/17.
//

#ifndef DONGMENDB_LITERAL_H
#define DONGMENDB_LITERAL_H

union LiteralVal {
    int ival;
    double dval;
    char cval;
    char *strval;
};
enum data_type {
    TYPE_INT,
    TYPE_DOUBLE,
    TYPE_CHAR,
    TYPE_TEXT
};

typedef struct Literal_ Literal;

typedef struct Literal_ {
    enum data_type t;
    union LiteralVal val;
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

#endif //DONGMENDB_LITERAL_H
