#ifndef __COLUMN_H_
#define __COLUMN_H_

#include "expression.h"
#include "literal.h"

#include <map>
#include <string>

using namespace std;

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

class Expression;
class Column;

class ForeignKeyRef {
public:
    const char *col_name, *table_name, *table_col_name;

/* constraints on single columns */
    ForeignKeyRef(const char *cname);
    ForeignKeyRef(const char *foreign_tname,
                                     const char *foreign_cname);
} ;

/*约束的基础类*/
class Constraint{
public:
    /*约束名称*/
    char *name;
    /*约束类型*/
    enum constraint_type type;
    /*约束影响的列,除主键和check外，外键，unique，default，null均是影响单列外*/
    Column *column;

    Constraint(char *name, enum constraint_type type, Column *column){
        this->name = name;
        this->type = type;
        this->column = column;
    }


    void Constraint_print(void *constraint);

};

class NullConstraint : public Constraint{
public:
    bool isNull;

    NullConstraint(char *name, enum constraint_type type, Column *column, bool isNull) : Constraint(name, type, column){
        this->isNull = isNull;
    }
};

class UniqueConstraint : public Constraint{
    UniqueConstraint(char *name, enum constraint_type type, Column *column) : Constraint(name, type, column){
    }
};

class ForeignKeyConstaint : public Constraint{
public:
    ForeignKeyRef *ref;

    ForeignKeyConstaint(char *name, enum constraint_type type, Column *column, ForeignKeyRef *ref) : Constraint(name, type, column){
        this->ref = ref;
    }
};

class DefaultConstraint : public Constraint{
public:
    Literal *default_val;
    DefaultConstraint(char *name, enum constraint_type type, Column *column, Literal *default_val) : Constraint(name, type, column){
        this->default_val = default_val;
    }
};

class CheckConstraint : public Constraint{
public:
    Expression *check;
    CheckConstraint(char *name, enum constraint_type type, Column *column, Expression *check) : Constraint(name, type, column){
        this->check = check;
    }
};

class PrimaryKeyConstraint : public Constraint {
public:
    map<string, Column*> columns;
    PrimaryKeyConstraint(char *name, enum constraint_type type, Column *column,  map<string, Column*> columns) : Constraint(name, type, column){
        this->columns = columns;
    }
};
typedef struct Constraint_s Constraint_t;
typedef struct Constraint_s {
    enum constraint_type t;
    union {
        ForeignKeyRef *ref;
        Literal *default_val;
        unsigned size;
        Expression *check;
    } constraint;
    Constraint_t *next;
} ;

class Column {
public:
    char *name;
    enum data_type type;
//    Constraint_t *constraints;
    map<string, Constraint*> *cons;

    Column(const char *name, enum data_type type,  map<string, Constraint*> *cons);

    size_t Column_getSize();

} ;

class ColumnReference {
public:
    char *tableName;
    char *columnName;
    char *columnAlias;
    char *allName;

    ColumnReference(const char *tname, const char *cname);

    ColumnReference(char *allName);

};

std::ostream &operator<<(std::ostream &os, ColumnReference *ref);

#endif
