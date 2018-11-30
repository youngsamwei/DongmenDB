//
// Created by Sam on 2018/1/22.
//

#ifndef DONGMENDB_OPSTACK_H
#define DONGMENDB_OPSTACK_H

/*定义解析表达式时用到的公共结构和函数*/

#include "parser/Tokenizer.h"

/*操作符堆栈*/
typedef struct op_stack_ op_stack;
typedef struct op_stack_ {
    TokenType operatorType;
    op_stack *next;
} op_stack;

op_stack *stackPush(op_stack *opstack, TokenType opType);

op_stack *stackPop(op_stack *opstack);



#endif //DONGMENDB_OPSTACK_H
