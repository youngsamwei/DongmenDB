//
// Created by Sam on 2018/1/17.
//

#ifndef DONGMENDB_EXPRESSION_H
#define DONGMENDB_EXPRESSION_H

/*定义表达式级别的结构*/

typedef struct TableExpr_ TableExpr;
typedef struct WhereExpr_ WhereExpr;

/*当next为null时结尾*/
typedef struct TableExpr_ {
    char *db;
    char *schema;
    char *name;
    /*用于连接当前Table与nextTable的条件*/
    WhereExpr *joinExpr;
    TableExpr *nextTable;
} TableExpr;

typedef struct ValueExpr_ ValueExpr;
typedef struct FieldsExpr_ FieldsExpr;
typedef struct FieldsExpr_ {
    /*每一个field都是表达式*/
    ValueExpr *valueExpr;
    FieldsExpr *nextField;
} FieldsExpr;

/*逻辑运算的操作符*/
enum OpType {
    RA_OP_EQ,
    RA_OP_LT,
    RA_OP_GT,
    RA_OP_LEQ,
    RA_OP_GEQ,
    RA_OP_IN,
    RA_OP_LIKE,
    RA_OP_AND,
    RA_OP_OR,
    RA_OP_NOT
};

/*值表达式：布尔，数值，字符串*/
typedef struct ValueExpr_{

}ValueExpr;

typedef struct LogicalExpr_{
    /*当opType为单目运算符时（如RA_OP_NOT）时，exprr有值*/
    enum OpType opType;
    ValueExpr *exprl, *exprr;
} LogicalExpr;

typedef struct WhereExpr_ {
    /*在where中最外层的cond的opType必须为逻辑运算符*/
    LogicalExpr *cond;
} WhereExpr;

typedef struct GroupExpr_ {

} GroupExpr;

typedef struct OrderExpr_ {

} OrderExpr;

typedef struct ColumnsExpr_ {

} ColumnsExpr;

typedef struct ConstraintsExpr_ {

} ConstraintsExpr;

typedef struct SetExpr_ {

} SetExpr;

#endif //DONGMENDB_EXPRESSION_H
