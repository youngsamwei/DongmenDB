//
// Created by Sam on 2018/1/17.
//

#ifndef DONGMENDB_EXPRESSION_H
#define DONGMENDB_EXPRESSION_H

#include <malloc.h>
#include <mem.h>
#include <stdio.h>
#include <dongmensql/literal.h>
#include <utils/arraylist.h>
#include <utils/utils.h>
#include "utils/opstack.h"
#include "tokenizer.h"
#include "parser.h"


#ifdef __cplusplus
extern "C" {
#endif


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
typedef enum {
    TERM_UNKNOWN = 0,
    TERM_LITERAL,
    TERM_ID,
    TERM_NULL,
    TERM_COLREF,
    TERM_FUNC
} TermType;

typedef enum {
    FUNC_MAX,
    FUNC_MIN,
    FUNC_COUNT,
    FUNC_AVG,
    FUNC_SUM
} FuncType;

typedef enum {
    left2right,
    right2left
} associativity;

typedef struct {
    int numbers;        // 操作数
    int icp;            // 优先级
    int isp;            // 优先级
    associativity ass;  // 结合性
    TokenType oper; // 操作符
} OPERATOR;

// [操作数，入栈优先级，出栈优先级，结合性，运算符编号]
static const OPERATOR operators[] = {
        /* 算数运算 */
        {2, 18, 1,  left2right, TOKEN_OPEN_PAREN},     // 左括号
        {2, 18, 18, left2right, TOKEN_CLOSE_PAREN},    // 右括号
        {2, 14, 14, left2right, TOKEN_POWER},     // 幂

        {2, 12, 12, left2right, TOKEN_PLUS},      // 加
        {2, 12, 12, left2right, TOKEN_MINUS},     // 减
        {2, 13, 13, left2right, TOKEN_MULTIPLY},  // 乘
        {2, 13, 13, left2right, TOKEN_DIVIDE},    // 除

//        {2, 13, 13, left2right, TOKEN_MOD},       // 模

//        {1, 16, 15, right2left, TOKEN_POSITIVE},  // 正号
//        {1, 16, 15, right2left, TOKEN_NEGATIVE},  // 负号
//        {1, 16, 15, left2right, TOKEN_FACTORIAL}, // 阶乘
//        {2, 12, 12, left2right, TOKEN_CONCAT},

        /* 关系运算 */
        {2, 10, 10, left2right, TOKEN_LT},        // 小于
        {2, 10, 10, left2right, TOKEN_GT},        // 大于
        {2, 10, 9,  left2right, TOKEN_EQ},          // 等于
        {2, 10, 9,  left2right, TOKEN_NOT_EQUAL},          // 不等于
        {2, 10, 10, left2right, TOKEN_LE},        // 不大于
        {2, 10, 10, left2right, TOKEN_GE},        // 不小于
        {2, 10, 9,  left2right, TOKEN_IN},
        {2, 10, 9,  left2right, TOKEN_LIKE},
        /* 逻辑运算 */
        {2, 5,  5,  left2right, TOKEN_AND},         // 且
        {2, 5,  4,  left2right, TOKEN_OR},          // 或
        {1, 15, 15, right2left, TOKEN_NOT},       // 非
        /* 赋值 */
        {2, 2,  2,  right2left, TOKEN_ASSIGNMENT},  // 赋值
        /* 最小优先级 */
        {2, 17, 17, right2left, TOKEN_FUN},        //把函数解析为一个操作符
        {2, 1,  1,  right2left, TOKEN_COMMA}
};

typedef struct Expression_ Expression;
typedef struct Func {
    FuncType t;
    Expression *expr;
} Func;

typedef struct ColumnReference_s ColumnReference_t;
/*终结符：标识符，常量*/
typedef struct TermExpr_ {
    TermType t;
    union {
        char *id;
        Literal_t *val;
        ColumnReference_t *ref;
        Func f;
    };
} TermExpr;

typedef struct Expression_ {
    /*当term不为空时，表示是term*/
    TokenType opType;
    TermExpr *term;
    /*如果出现在select子句中，则可以有别名*/
    char *alias;
    /*按链表顺序存操作数*/
    Expression *nextexpr;

} Expression;

int printTermExpression(char *exprs, TermExpr *term);

const char *getOperatorDesc(TokenType type);

Expression *newExpression(TokenType type, Expression *nextexpr);

TermExpr *newTermExpr();

char *getExpressionNamesTitle(arraylist *fields);

int expression_free(Expression *expr);

int expression_free_list(arraylist *exprlist);

Expression *expression_print(Expression *expr, char *desc);

int expression_print_list(arraylist *exprlist);


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
Expression *parseReadArgument(ParserT *parser);

/**
 @brief 解析函数, 如 sqrt(.), pow(.), 等
 @param[in] parser ParserT结构的解析器
 @return 函数表达式
*/
Expression *parseReadBuiltin(ParserT *parser);

/**
 @brief  解析括号内的表达式
 @param[in] parser ParserT结构的解析器
 @return 表达式
 */
Expression *parseReadParen(ParserT *parser);

/**
 @brief 解析单目运算表达式
 @param[in] parser ParserT结构的解析器
 @return 表达式
*/
Expression *parseReadUnary(ParserT *parser);

/**
 @brief 解析指数表达式
 @param[in] parser ParserT结构的解析器
 @return 表达式
 */
Expression *parseReadPower(ParserT *parser);

/**
 @brief 解析终结符表达式
 @param[in] parser  ParserT结构的解析器
 @return 表达式
 */
Expression *parseReadTerm(ParserT *parser);

/**
 @brief 解析算术表达式
 @param[in] parser ParserT结构的解析器
 @return 表达式
 */
Expression *parseReadExpr(ParserT *parser);

/**
 @brief 解析比较表达式
 @param[in] parser  ParserT结构的解析器
 @return 表达式
 */
Expression *parseReadBooleanComparison(ParserT *parser);

/**
 * @brief 解析等比较表达式
 * @param parser ParserT结构的解析器
 * @return  表达式
 */
Expression *parseReadBooleanEquality(ParserT *parser);

/**
 @brief 解析and比较表达式
 @param[in] parser  ParserT结构的解析器
 @return 表达式
*/
Expression *parseReadBooleanAnd(ParserT *parser);

/**
 @brief 解析or比较表达式
 @param[in] parser ParserT结构的解析器
 @return 表达式
*/
Expression *parseReadBooleanOr(ParserT *parser);
#ifdef __cplusplus
}
#endif

#endif //DONGMENDB_EXPRESSION_H
