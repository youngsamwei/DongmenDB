//
// Created by Sam on 2018/1/17.
//

#ifndef DONGMENDB_EXPRESSION_H
#define DONGMENDB_EXPRESSION_H

#include "literal.h"

/*定义表达式级别的结构*/

enum OpType {
    /*比较运算符*/
            OP_EQ,
    OP_LT,
    OP_GT,
    OP_LEQ,
    OP_GEQ,
    OP_IN,
    OP_LIKE,
    /*逻辑运算符*/
            OP_AND,
    OP_OR,
    OP_NOT,
    /*字符串连接*/
            OP_CONCAT,
    /*算术运算*/
            OP_PLUS,
    OP_MINUS,
    OP_MULTIPLY,
    OP_DIVIDE,
    OP_NEG,
    /*原子项*/
            OP_TERM,
};
typedef enum {
    /* 算数运算 */
            oper_lparen = 0,    // 左括号
    oper_rparen,        // 右括号
    oper_plus,          // 加
    oper_minus,         // 减
    oper_multiply,      // 乘
    oper_divide,        // 除
    oper_mod,           // 模
    oper_power,         // 幂
    oper_positive,      // 正号
    oper_negative,      // 负号
    oper_factorial,     // 阶乘
    /* 关系运算 */
            oper_lt,            // 小于
    oper_gt,            // 大于
    oper_eq,            // 等于
    oper_ne,            // 不等于
    oper_le,            // 不大于
    oper_ge,            // 不小于
    /* 逻辑运算 */
            oper_and,           // 且
    oper_or,            // 或
    oper_not,           // 非
    /* 赋值 */
            oper_assignment,    // 赋值
    oper_min            // 栈底
} operator_type;
typedef enum {
    left2right,
    right2left
} associativity;
typedef struct {
    int numbers;        // 操作数
    int icp;            // 优先级
    int isp;            // 优先级
    associativity ass;  // 结合性
    operator_type oper; // 操作符
} OPERATOR;

// in expression.c
static const OPERATOR operators[] = {
        /* 算数运算 */
        {2, 17, 1, left2right, oper_lparen},     // 左括号
        {2, 17, 17, left2right, oper_rparen},    // 右括号
        {2, 12, 12, left2right, oper_plus},      // 加
        {2, 12, 12, left2right, oper_minus},     // 减
        {2, 13, 13, left2right, oper_multiply},  // 乘
        {2, 13, 13, left2right, oper_divide},    // 除
        {2, 13, 13, left2right, oper_mod},       // 模
        {2, 14, 14, left2right, oper_power},     // 幂
        {1, 16, 15, right2left, oper_positive},  // 正号
        {1, 16, 15, right2left, oper_negative},  // 负号
        {1, 16, 15, left2right, oper_factorial}, // 阶乘
        /* 关系运算 */
        {2, 10, 10, left2right, oper_lt},        // 小于
        {2, 10, 10, left2right, oper_gt},        // 大于
        {2, 9, 9, left2right, oper_eq},          // 等于
        {2, 9, 9, left2right, oper_ne},          // 不等于
        {2, 10, 10, left2right, oper_le},        // 不大于
        {2, 10, 10, left2right, oper_ge},        // 不小于
        /* 逻辑运算 */
        {2, 5, 5, left2right, oper_and},         // 且
        {2, 4, 4, left2right, oper_or},          // 或
        {1, 15, 15, right2left, oper_not},       // 非
        /* 赋值 */
        // BASIC 中赋值语句不属于表达式！
        {2, 2, 2, right2left, oper_assignment},  // 赋值
        /* 最小优先级 */
        {2, 0, 0, right2left, oper_min}          // 栈底
};

enum TermType {
    TERM_LITERAL, /*值（数值，字符串）*/
    TERM_ID,  /*标识符（字段*/
    TERM_NULL,
    TERM_COLREF,
    TERM_FUNC
};

enum FuncType {
    FUNC_MAX,
    FUNC_MIN,
    FUNC_COUNT,
    FUNC_AVG,
    FUNC_SUM
};

#endif //DONGMENDB_EXPRESSION_H
