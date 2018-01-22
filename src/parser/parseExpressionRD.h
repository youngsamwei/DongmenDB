//
// Created by Sam on 2018/1/22.
//

#ifndef DONGMENDB_PARSEEXPRESSIONRD_H
#define DONGMENDB_PARSEEXPRESSIONRD_H

/*使用递归下降法解析表达式*/

#include <expression.h>
#include "tokenizer.h"
#include "parser.h"


Expression *parseExpressionRD(ParserT *parser);

#endif //DONGMENDB_PARSEEXPRESSIONRD_H
