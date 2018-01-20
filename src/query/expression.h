//
// Created by Sam on 2018/1/17.
//

#ifndef DONGMENDB_EXPRESSION_H
#define DONGMENDB_EXPRESSION_H

#include <tokenizer.h>
#include "literal.h"
#include "sqlexpression.h"

/*解析表达式。表达式支持：
 * 运算：算术运算，比较运算，逻辑运算，字符串连接
 * 标识符：字段变量，常量，函数
 * 数据类型：数值，字符串，布尔型
 *
 * 算法：逆波兰法
 * TODO：递归下降法
 * */

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

// in expression.c
static const OPERATOR operators[] = {
        /* 算数运算 */
        {2, 18, 1,  left2right, TOKEN_OPEN_PAREN},     // 左括号
        {2, 18, 18, left2right, TOKEN_CLOSE_PAREN},    // 右括号
        {2, 12, 12, left2right, TOKEN_PLUS},      // 加
        {2, 12, 12, left2right, TOKEN_MINUS},     // 减
        {2, 13, 13, left2right, TOKEN_MULTIPLY},  // 乘
        {2, 13, 13, left2right, TOKEN_DIVIDE},    // 除
        /* 关系运算 */
        {2, 10, 10, left2right, TOKEN_LT},        // 小于
        {2, 10, 10, left2right, TOKEN_GT},        // 大于
        {2, 10,  9,  left2right, TOKEN_EQ},          // 等于
        {2, 10,  9,  left2right, TOKEN_NOT_EQUAL},          // 不等于
        {2, 10, 10, left2right, TOKEN_LE},        // 不大于
        {2, 10, 10, left2right, TOKEN_GE},        // 不小于
        {2, 10,  9,  left2right, TOKEN_IN},
        {2, 10,  9,  left2right, TOKEN_LIKE},
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

/*终结符类型*/
typedef enum {
    TERM_LITERAL, /*值（数值，字符串）*/
    TERM_ID,  /*标识符（字段)*/
    TERM_NULL,
    TERM_COLREF
} TermType;

/*终结符：标识符，常量*/
typedef struct TermExpr_ {
    TermType t;
    union {
        char *id;
        Literal *val;
    };
} TermExpr;

typedef struct Expression_ Expression;
typedef struct Expression_ {
    /*当为oper_term时，表示是term*/
    TokenType opType;
    TermExpr *term;
    /*按链表顺序存操作数*/
    Expression *nextexpr;

} Expression;


void printExpression(Expression *expr);
char *getExpressionDesc(Expression *expr);

#endif //DONGMENDB_EXPRESSION_H
