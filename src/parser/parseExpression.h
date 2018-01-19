//
// Created by Sam on 2018/1/19.
//

#ifndef DONGMENDB_PARSEREXPRESSION_H
#define DONGMENDB_PARSEREXPRESSION_H

#include <expression.h>
#include "tokenizer.h"

/* 表达式解析基本方法：逆波兰法(Reverse Polish)和递归下降法(recursive descent)
 * 这里使用逆波兰法。
 *
 * */

Expression *parseExpression(TokenizerT *tokenizer);

#endif //DONGMENDB_PARSEREXPRESSION_H
