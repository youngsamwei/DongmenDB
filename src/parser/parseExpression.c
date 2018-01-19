//
// Created by Sam on 2018/1/19.
//

#include "parseExpression.h"

/*使用逆波兰法解析表达式*/
Expression *parseExpression(TokenizerT *tk){
    op_stack *opstack ;
    TokenT *token = TKGetNextToken(tk);
    if(token->type)
};
