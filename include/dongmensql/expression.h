//
// Created by Sam on 2018/1/17.
//

#ifndef DONGMENDB_EXPRESSION_H
#define DONGMENDB_EXPRESSION_H

#include <malloc.h>
#include <string.h>
#include <stdio.h>
#include <dongmensql/literal.h>

#include <utils/utils.h>
#include "parser/Tokenizer.h"
#include "parser/parser.h"

#include <vector>

using namespace std;


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

class Operator {
public:
    int numbers;        // 操作数
    int icp;            // 优先级
    int isp;            // 优先级
    associativity ass;  // 结合性
    TokenType oper; // 操作符

    Operator(int numbers, int icp, int isp, associativity ass, TokenType oper) {
        this->numbers = numbers;        // 操作数
        this->icp = icp;            // 优先级
        this->isp = isp;            // 优先级
        this->ass = ass;  // 结合性
        this->oper = oper; // 操作符
    };
};

// [操作数，入栈优先级，出栈优先级，结合性，运算符编号]
static const Operator operators[] = {
        /* 算数运算 */
        Operator(2, 18, 1, left2right, TOKEN_OPEN_PAREN),     // 左括号
        Operator(2, 18, 18, left2right, TOKEN_CLOSE_PAREN),    // 右括号
        Operator(2, 14, 14, left2right, TOKEN_POWER),     // 幂

        Operator(2, 12, 12, left2right, TOKEN_PLUS),      // 加
        Operator(2, 12, 12, left2right, TOKEN_MINUS),     // 减
        Operator(2, 13, 13, left2right, TOKEN_MULTIPLY),  // 乘
        Operator(2, 13, 13, left2right, TOKEN_DIVIDE),    // 除

//        {2, 13, 13, left2right, TOKEN_MOD},       // 模

//        {1, 16, 15, right2left, TOKEN_POSITIVE},  // 正号
//        {1, 16, 15, right2left, TOKEN_NEGATIVE},  // 负号
//        {1, 16, 15, left2right, TOKEN_FACTORIAL}, // 阶乘
//        {2, 12, 12, left2right, TOKEN_CONCAT},

        /* 关系运算 */
        Operator(2, 10, 10, left2right, TOKEN_LT),        // 小于
        Operator(2, 10, 10, left2right, TOKEN_GT),        // 大于
        Operator(2, 10, 9, left2right, TOKEN_EQ),          // 等于
        Operator(2, 10, 9, left2right, TOKEN_NOT_EQUAL),          // 不等于
        Operator(2, 10, 10, left2right, TOKEN_LE),        // 不大于
        Operator(2, 10, 10, left2right, TOKEN_GE),        // 不小于
        Operator(2, 10, 9, left2right, TOKEN_IN),
        Operator(2, 10, 9, left2right, TOKEN_LIKE),
        /* 逻辑运算 */
        Operator(2, 5, 5, left2right, TOKEN_AND),         // 且
        Operator(2, 5, 4, left2right, TOKEN_OR),          // 或
        Operator(1, 15, 15, right2left, TOKEN_NOT),       // 非
        /* 赋值 */
        Operator(2, 2, 2, right2left, TOKEN_ASSIGNMENT),  // 赋值
        /* 最小优先级 */
        Operator(2, 17, 17, right2left, TOKEN_FUN),        //把函数解析为一个操作符
        Operator(2, 1, 1, right2left, TOKEN_COMMA)
};

class Expression;

/*函数*/
class Func {
    /*函数类型*/
    FuncType t;
    /*函数参数*/
    Expression *expr;
};

class ColumnReference;

/*终结符：标识符，常量*/
class TermExpr {
public:
    TermType t;
    union {
        /*标识符：表名等*/
        char *id;
        /*常量*/
        Literal *val;
        /*字段变量*/
        ColumnReference *ref;
        /*函数*/
        Func f;
    };
};

class Expression {
public:
    /*当term不为空时，表示是term*/
    TokenType opType;
    TermExpr *term;
    /*如果出现在select子句中，则可以有别名*/
    char *alias;
    /*按链表顺序存操作数*/
    Expression *nextexpr;


    int printTermExpression(char *exprs, TermExpr *term);

    const char *getOperatorDesc(TokenType type);

    Expression(TokenType type, Expression *nextexpr);

    TermExpr *newTermExpr();

    int expression_free();

    Expression *expression_print(Expression *expr, char *desc);


};

char *getExpressionNamesTitle(vector<Expression *> fields);

int expression_free_list(vector<Expression *> exprlist);

int expression_print_list(vector<Expression *> exprlist);


#endif //DONGMENDB_EXPRESSION_H
