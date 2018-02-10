//
// Created by Sam on 2018/1/18.
//

#ifndef DONGMENDB_SQLEXPRESSION_H
#define DONGMENDB_SQLEXPRESSION_H

#include "dongmengsql.h"

/*
typedef struct TablesExpr_ TablesExpr;
typedef struct TermExpr_ TermExpr;
typedef struct BinaryExpr_ BinaryExpr;
typedef struct UnaryExpr_ UnaryExpr;
typedef struct FieldsExpr_ FieldsExpr;
*/

/*在select子句中使用的field*/
/*
typedef struct FieldsExpr_ {

    Expression *expr;
    char *alias;
    FieldsExpr *nextField;
} FieldsExpr;
*/

/*当next为null时结尾*/
/*
typedef struct TablesExpr_ {
    char *db;
    char *schema;
    char *name;

    Expression *joinExpr;
    TablesExpr *nextTable;
} TablesExpr;
*/
/*
typedef struct GroupExpr_ GroupExpr;
typedef struct OrderExpr_ OrderExpr;
typedef struct OrderExpr_ {
    enum OrderBy orderType;
    Expression *expr;
    OrderExpr *next;
} OrderExpr;
*/
//typedef struct Constraints_ Constraints;
/*描述在create table中使用的column*/
/*
typedef struct ColumnsExpr_ {
    enum data_type type;
    char *columnName;
    int length;
    Constraints *constraints;
} ColumnsExpr;
 */

/*
typedef struct ForeignKeyRef {
    const char *col_name, *table_name, *table_col_name;
} ForeignKeyRef;

typedef struct Constraints_ {
    enum constraint_type type;
    union {
        ForeignKeyRef ref;
        Literal_t *default_val;
        unsigned size;

        Expression *check;
    } constraint;
    struct Constraints *next;
} Constraints;

typedef struct SetExpr_ {

} SetExpr;
*/
/*用于表示insert语句中的values*/
/*
typedef struct ValueList_ ValueList;
typedef struct ValueList_ {
    union LitVal val;
    ValueList *next;
} ValueList;
*/
#endif //DONGMENDB_SQLEXPRESSION_H
