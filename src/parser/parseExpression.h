//
// Created by Sam on 2018/1/22.
//

#ifndef DONGMENDB_PARSEREXPRESSION_H
#define DONGMENDB_PARSEREXPRESSION_H

/*定义解析表达式时用到的公共结构和函数*/

#include <expression.h>
#include "tokenizer.h"

/*操作符堆栈*/
typedef  struct op_stack_ op_stack;
typedef struct op_stack_{
    TokenType operatorType;
    op_stack *next;
} op_stack;

op_stack *stackPush(op_stack *opstack, TokenType opType);
op_stack *stackPop(op_stack *opstack) ;

Expression *newExpression(TokenType type, Expression *nextexpr);
TermExpr *newTermExpr();

#endif //DONGMENDB_PARSEREXPRESSION_H
