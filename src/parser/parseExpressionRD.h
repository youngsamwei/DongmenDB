//
// Created by Sam on 2018/1/22.
//

#ifndef DONGMENDB_PARSEEXPRESSIONRD_H
#define DONGMENDB_PARSEEXPRESSIONRD_H

/*使用递归下降法解析表达式*/

#include <expression.h>
#include "tokenizer.h"
#include "parser.h"


Expression *concatExpression(Expression *expr0, Expression *expr1);

Expression *parseExpressionRD(ParserT *parser);

Expression *parseReadDouble( ParserT *parser );

/**
 @brief reads arguments for the builtin functions, auxilliary function for
 parseRead_builtin()
 @param[in] parser input ParserT structure to operate upon
 @return expr of the argument that was read
 */
Expression *parseReadArgument( ParserT *parser );

/**
 @brief reads and calls built-in functions, like sqrt(.), pow(.), etc.
 @param[in] parser input ParserT structure to operate upon
 @return resulting expr
*/
Expression *parseReadBuiltin( ParserT *parser );

/**
 @brief attempts to read an expression in parentheses, or failing that a literal value
 @param[in] parser input ParserT structure to operate upon
 @return expression/literal
 */
Expression *parseReadParen( ParserT *parser );

/**
 @brief attempts to read a unary operation, or failing that, a parenthetical or literal value
 @param[in] parser input ParserT structure to operate upon
 @return expression/literal
*/
Expression *parseReadUnary( ParserT *parser );

/**
 @brief attempts to read an exponentiation operator, or failing that, a parenthetical expression
 @param[in] parser input ParserT structure to operate upon
 @return exponentiation
 */
Expression *parseReadPower( ParserT *parser );

/**
 @brief reads a term in an expression
 @param[in] parser input ParserT structure to operate on
 @return expr of the term
 */
Expression *parseReadTerm( ParserT *parser );

/**
 @brief attempts to read an expression
 @param[in] parser input ParserT structure
 @return expression
 */
Expression *parseReadExpr( ParserT *parser );

/**
 @brief reads and performs a boolean comparison operations (<,>,<=,>=,==) if found
 @param[in] parser input ParserT structure
 @return sub-expression
 */
Expression *parseReadBooleanComparison( ParserT *parser );

Expression *parseReadBooleanEquality( ParserT *parser );

/**
 @brief reads and performs a boolean 'and' operation (if found)
 @param[in] parser input ParserT structure
 @return sub-expression
*/
Expression *parseReadBooleanAnd( ParserT *parser );

/**
 @brief reads and performs a boolean or operation (if found)
 @param[in] parser input ParserT structure
 @return expression
*/
Expression *parseReadBooleanOr( ParserT *parser );

#endif //DONGMENDB_PARSEEXPRESSIONRD_H
