//
// Created by Sam on 2018/1/17.
//

#include <malloc.h>
#include "literal.h"



Literal *newLiteral(data_type t){
    Literal *literal = (Literal *)malloc(sizeof(Literal));
    literal->next = NULL;
    literal ->t = t;
    return literal;
};