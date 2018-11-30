//
// Created by Sam on 2018/1/17.
//

#ifndef DONGMENDB_PARSER_H
#define DONGMENDB_PARSER_H

#include <dongmensql/dongmensql.h>
#include "Tokenizer.h"
#include "expression.h"

/*
 * 定义parser的基础数据结构和共用的函数
 *
 * */

#define PARSER_MESSAGE_LENTTH 128

typedef enum{
    SQL_STMT_SELECT,
    SQL_STMT_INSERT,
    SQL_STMT_UPDATE,
    SQL_STMT_DELETE,
    SQL_STMT_CREATE_TABLE,
    SQL_STMT_CREATE_USER,
    SQL_STMT_CREATE_ROLE,
    SQL_STMT_DROP_TABLE,
    SQL_STMT_DROP_USER,
    SQL_STMT_DROP_ROLE,
    SQL_STMT_GRANT,
    SQL_STMT_REVOKE
}sql_stmt_type;

/*语法分析状态类型*/
typedef enum {
    PARSER_WRONG,
    PARSER_CORRECT,
    PARSER_RESERVED_WORD
} ParserStateType;

class Expression;

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





#endif //DONGMENDB_PARSER_H
