//
// Created by Sam on 2018/1/17.
//

#ifndef DONGMENDB_PARSER_H
#define DONGMENDB_PARSER_H

#include "Tokenizer.h"
#include "dongmensql/expression.h"
#include <dongmensql/sqlstatement.h>
#include <dongmensql/column.h>
#include <dongmendb/SecurityManager.h>

/*
 * 定义parser的基础数据结构和共用的函数
 *
 * */

#define PARSER_MESSAGE_LENTTH 128


/*语法分析状态类型*/
typedef enum {
    PARSER_WRONG,
    PARSER_CORRECT,
    PARSER_RESERVED_WORD
} ParserStateType;

class Expression;
class User;
class Role;
class Right;

class Parser {
public:
    Tokenizer *tokenizer;
    Token *currToken;
    ParserStateType parserStateType;
    char parserMessage[PARSER_MESSAGE_LENTTH];

    Parser(Tokenizer *tokenizer);
    Token *parseNextToken();
    Token *parseEatToken();
    Token *parseEatAndNextToken();
    void *parseError(char *message);

    int matchToken( TokenType type, char *text);


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
    Expression *parseExpressionRD();

    Expression *parseReadLiteral();

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
    Expression *parseReadArgument();

/**
 @brief 解析函数, 如 sqrt(.), pow(.), 等
 @param[in] parser ParserT结构的解析器
 @return 函数表达式
*/
    Expression *parseReadBuiltin();

/**
 @brief  解析括号内的表达式
 @param[in] parser ParserT结构的解析器
 @return 表达式
 */
    Expression *parseReadParen();

/**
 @brief 解析单目运算表达式
 @param[in] parser ParserT结构的解析器
 @return 表达式
*/
    Expression *parseReadUnary();

/**
 @brief 解析指数表达式
 @param[in] parser ParserT结构的解析器
 @return 表达式
 */
    Expression *parseReadPower();

/**
 @brief 解析终结符表达式
 @param[in] parser  ParserT结构的解析器
 @return 表达式
 */
    Expression *parseReadTerm();

/**
 @brief 解析算术表达式
 @param[in] parser ParserT结构的解析器
 @return 表达式
 */
    Expression *parseReadExpr();

/**
 @brief 解析比较表达式
 @param[in] parser  ParserT结构的解析器
 @return 表达式
 */
    Expression *parseReadBooleanComparison();

/**
 * @brief 解析等比较表达式
 * @param parser ParserT结构的解析器
 * @return  表达式
 */
    Expression *parseReadBooleanEquality();

/**
 @brief 解析and比较表达式
 @param[in] parser  ParserT结构的解析器
 @return 表达式
*/
    Expression *parseReadBooleanAnd();

/**
 @brief 解析or比较表达式
 @param[in] parser ParserT结构的解析器
 @return 表达式
*/
    Expression *parseReadBooleanOr();

} ;


class  sql_stmt_grant_revoke;

User *parse_create_user(Parser *parser);

Role *parse_create_role(Parser *parser);

sql_stmt_grant_revoke *parse_grant(Parser *parser);

sql_stmt_grant_revoke *parse_revoke(Parser *parser);




#endif //DONGMENDB_PARSER_H
