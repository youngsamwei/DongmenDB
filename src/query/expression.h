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
    oper_concat,        //字符串拼接
    oper_lt,            // 小于
    oper_gt,            // 大于
    oper_eq,            // 等于
    oper_ne,            // 不等于
    oper_le,            // 不大于
    oper_ge,            // 不小于
    oper_in,
    oper_like,
    oper_and,           // 且
    oper_or,            // 或
    oper_not,           // 非
    oper_assignment,    // 赋值
    oper_min,            // 栈底
    oper_term,           //终结符
    oper_fun,            //把函数解析为一个操作符
    oper_comma           //逗号 在select中，insert values中，fun(,,,),(1,2,3)等四种用法
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
        {2, 18, 1,  left2right, oper_lparen},     // 左括号
        {2, 18, 18, left2right, oper_rparen},    // 右括号
        {2, 12, 12, left2right, oper_plus},      // 加
        {2, 12, 12, left2right, oper_minus},     // 减
        {2, 13, 13, left2right, oper_multiply},  // 乘
        {2, 13, 13, left2right, oper_divide},    // 除
        {2, 13, 13, left2right, oper_mod},       // 模
        {2, 14, 14, left2right, oper_power},     // 幂
        {1, 16, 15, right2left, oper_positive},  // 正号
        {1, 16, 15, right2left, oper_negative},  // 负号
        {1, 16, 15, left2right, oper_factorial}, // 阶乘
        {2, 12, 12, left2right, oper_concat},
        /* 关系运算 */
        {2, 10, 10, left2right, oper_lt},        // 小于
        {2, 10, 10, left2right, oper_gt},        // 大于
        {2, 9,  9,  left2right, oper_eq},          // 等于
        {2, 9,  9,  left2right, oper_ne},          // 不等于
        {2, 10, 10, left2right, oper_le},        // 不大于
        {2, 10, 10, left2right, oper_ge},        // 不小于
        {2, 9,  9,  left2right, oper_in},
        {2, 9,  9,  left2right, oper_like},
        /* 逻辑运算 */
        {2, 5,  5,  left2right, oper_and},         // 且
        {2, 4,  4,  left2right, oper_or},          // 或
        {1, 15, 15, right2left, oper_not},       // 非
        /* 赋值 */
        // BASIC 中赋值语句不属于表达式！
        {2, 2,  2,  right2left, oper_assignment},  // 赋值
        /* 最小优先级 */
        {2, 0,  0,  right2left, oper_min},          // 栈底
        {2, 0,  0,  right2left, oper_term},
        {2, 17,  17,  right2left, oper_fun},        //把函数解析为一个操作符
        {2, 1,  1,  right2left, oper_comma}
};

/*终结符类型*/
typedef enum  {
    TERM_LITERAL, /*值（数值，字符串）*/
    TERM_ID,  /*标识符（字段)*/
    TERM_NULL,
    TERM_COLREF
}TermType;

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
    operator_type opType;
    union {
        TermExpr *term;
        /*按链表顺序存操作数*/
        Expression *nextexpr;
    };
} Expression;

#endif //DONGMENDB_EXPRESSION_H
