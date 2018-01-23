//
// Created by Sam on 2018/1/22.
//

#ifndef DONGMENDB_PARSEEXPRESSIONRD_H
#define DONGMENDB_PARSEEXPRESSIONRD_H

#include <expression.h>
#include "tokenizer.h"
#include "parser.h"


/*使用递归下降法解析表达式
 *
 * 支持：
 * 算术运算 + - * /
 * 比较运算: > < != <= >=
 * 逻辑运算: NOT AND OR
 * 函数:  round, ltrim （仅作为例子，没有实现）
 * 字段变量: 类似student.sname
 * 支持简单的语法检查。
 * 支持的数据类型:数值，字符串
 *
 * 尚未支持的运算符： like， between..and 等
 * 暂不支持函数
 * 此功能属于语法检查，尚不支持语义检查。比如函数操作符操作数类型匹配，函数传参类型匹配检查等。
 *
 * 入口函数：parseExpressionRD
 * */


/**
 * @brief 入口函数
 * @param parser ParserT结构的解析器
 * @return 表达式
 */
Expression *parseExpressionRD(ParserT *parser);

/**
 * @brief 合并两个后缀表达式，
  @param[in] expr0 作为左操作数，
  @param[in] expr1 作为右操作数\
  @return 合并后的后缀表达式
*/
Expression *concatExpression(Expression *expr0, Expression *expr1);

/**
 @brief  解析函数的参数列表
 @param[in] parser ParserT结构的解析器
 @return 读到的参数表达式
 */
Expression *parseReadArgument( ParserT *parser );

/**
 @brief 解析函数, 如 sqrt(.), pow(.), 等
 @param[in] parser ParserT结构的解析器
 @return 函数表达式
*/
Expression *parseReadBuiltin( ParserT *parser );

/**
 @brief  解析括号内的表达式
 @param[in] parser ParserT结构的解析器
 @return 表达式
 */
Expression *parseReadParen( ParserT *parser );

/**
 @brief 解析单目运算表达式
 @param[in] parser ParserT结构的解析器
 @return 表达式
*/
Expression *parseReadUnary( ParserT *parser );

/**
 @brief 解析指数表达式
 @param[in] parser ParserT结构的解析器
 @return 表达式
 */
Expression *parseReadPower( ParserT *parser );

/**
 @brief 解析终结符表达式
 @param[in] parser  ParserT结构的解析器
 @return 表达式
 */
Expression *parseReadTerm( ParserT *parser );

/**
 @brief 解析算术表达式
 @param[in] parser ParserT结构的解析器
 @return 表达式
 */
Expression *parseReadExpr( ParserT *parser );

/**
 @brief 解析比较表达式
 @param[in] parser  ParserT结构的解析器
 @return 表达式
 */
Expression *parseReadBooleanComparison( ParserT *parser );

/**
 * @brief 解析等比较表达式
 * @param parser ParserT结构的解析器
 * @return  表达式
 */
Expression *parseReadBooleanEquality( ParserT *parser );

/**
 @brief 解析and比较表达式
 @param[in] parser  ParserT结构的解析器
 @return 表达式
*/
Expression *parseReadBooleanAnd( ParserT *parser );

/**
 @brief 解析or比较表达式
 @param[in] parser ParserT结构的解析器
 @return 表达式
*/
Expression *parseReadBooleanOr( ParserT *parser );

#endif //DONGMENDB_PARSEEXPRESSIONRD_H
