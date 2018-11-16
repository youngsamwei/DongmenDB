//
// Created by Sam on 2018/1/22.
//

#include <malloc.h>
#include "utils/opstack.h"

/*使用链表表示栈：入栈*/
op_stack *stackPush(op_stack *opstack, TokenType opType) {
    op_stack *stack = (op_stack *) malloc(sizeof(op_stack));
    stack->operatorType = opType;
    stack->next = opstack;
    return stack;
}

/*出栈*/
op_stack *stackPop(op_stack *opstack) {
    op_stack *current = opstack->next;
    free(opstack);
    return current;
}
