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
    /*原子项*/
            OP_TERM,
    /*算术运算*/
            OP_PLUS,
    OP_MINUS,
    OP_MULTIPLY,
    OP_DIVIDE,
    OP_NEG,
    /*字符串连接*/
            OP_CONCAT
};
enum TermType {
    TERM_LITERAL,
    TERM_ID,
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

typedef struct TablesExpr_ TablesExpr;
typedef struct WhereExpr_ WhereExpr;
typedef struct Expression_ Expression;
typedef struct TermExpr_ TermExpr;
typedef struct BinaryExpr_ BinaryExpr;
typedef struct UnaryExpr_ UnaryExpr;
typedef struct FieldsExpr_ FieldsExpr;


typedef struct FieldsExpr_ {
    /*每一个field都是表达式*/
    Expression *expr;
    char *alias;
    FieldsExpr *nextField;
} FieldsExpr;

/*当next为null时结尾*/
typedef struct TablesExpr_ {
    char *db;
    char *schema;
    char *name;
    /*用于连接当前Table与nextTable的条件*/
    Expression *joinExpr;
    TablesExpr *nextTable;
} TablesExpr;

typedef struct Expression_ {
    /*当为OP_TERM时，nextExpression=null*/
    enum OpType opType;
    union {
        TermExpr *term;
        BinaryExpr *binaryExpr;
        UnaryExpr *unaryExpr;
    } expr;

    Expression *nextExpression;
} Expression;

typedef struct Func {
    enum FuncType t;
    Expression *expr;
} Func;

/*原子表达式：标识符，常量，函数*/
typedef struct TermExpr_ {
    enum TermType t;
    union {
        char *id;
        Literal *val;
        Func f;
    };
} TermExpr;

/*二元操作符表达式*/
typedef struct BinaryExpr_ {
    enum OpType opType;
    Expression *leftExpr, *rightExpr;
} BinaryExpr;

/*一元操作符表达式*/
typedef struct UnaryExpr_ {
    enum OpType opType;
    Expression *expr;
} UnaryExpr;

typedef struct GroupExpr_ GroupExpr;
typedef struct GroupExpr_ {
    Expression *expr;
    GroupExpr *next;
} GroupExpr;

enum OrderType {
    ORDER_BY_ASC, ORDER_BY_DESC
};
typedef struct OrderExpr_ OrderExpr;
typedef struct OrderExpr_ {
    enum OrderType orderType;
    Expression *expr;
    OrderExpr *next;
} OrderExpr;

typedef struct Constraints_ Constraints;
typedef struct ColumnsExpr_ {
    enum data_type type;
    char *columnName;
    Constraints *constraints;

} ColumnsExpr;

enum constraint_type {
    CONS_NOT_NULL,
    CONS_UNIQUE,
    CONS_PRIMARY_KEY,
    CONS_FOREIGN_KEY,
    CONS_DEFAULT,
    CONS_AUTO_INCREMENT,
    CONS_CHECK,
    CONS_SIZE
};

typedef struct ForeignKeyRef {
    const char *col_name, *table_name, *table_col_name;
} ForeignKeyRef;

typedef struct Constraints_ {
    enum constraint_type type;
    union {
        ForeignKeyRef ref;
        Literal *default_val;
        unsigned size;
        /*布尔表达式 */
        Expression *check;
    } constraint;
    struct Constraints *next;
} Constraints;

typedef struct SetExpr_ {

} SetExpr;

#endif //DONGMENDB_EXPRESSION_H
