//
// Created by Sam on 2018/1/19.
//

#include <malloc.h>
#include "parseExpression.h"

/*使用逆波兰法解析表达式*/
Expression *parseExpression(TokenizerT *tk){
    op_stack *opstack ;
    TokenT *token = TKGetNextToken(tk);
    if(token->type ==TOKEN_OPEN_PAREN){

    }
};

void stackPush(op_stack *opstack , operator_type opType){
    op_stack *stack = (op_stack *)malloc(sizeof(op_stack));
    stack->operatorType = opType;
    stack->next = opstack->next;
    opstack->next = stack;
}

operator_type stackPop(op_stack *opstack){

}