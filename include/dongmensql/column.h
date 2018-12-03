#ifndef __COLUMN_H_
#define __COLUMN_H_

#include "parser/expression.h"
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
    Literal_t *default_val;
    DefaultConstraint(char *name, enum constraint_type type, Column *column, Literal_t *default_val) : Constraint(name, type, column){
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
        Literal_t *default_val;
        unsigned size;
        Expression *check;
    } constraint;
    Constraint_t *next;
} ;

class Column {
public:
    char *name;
    enum data_type type;
    Constraint_t *constraints;
    size_t offset; /* offset in bytes from the beginning of the row */
    Column *next;

    void *Column_addConstraint(Constraint_t *constraints);
    Column(const char *name, enum data_type type, Constraint_t *constraints);
    Column(){};
    Column *Column_append(Column *column);
    Column *app_col(Column *col2);

    int Column_compareByName(const void *col1, const void *col2);
    void *Column_copy(void *col);

    void Column_getOffsets();
    void Column_getOffsets_r(size_t offset);
    void Column_getOffsets(Column *cols);

    size_t Column_getSize();
/* sets the size of the next column */
    void Column_setSize(ssize_t size);

    void Column_freeList();

} ;

class ColumnReference {
public:
    char *tableName, *columnName, *columnAlias, *allName;
    ColumnReference(const char *tname, const char *cname);
    ColumnReference(char *allName);

} ;



Constraint_t *NotNull(void);
Constraint_t *AutoIncrement(void);
Constraint_t *PrimaryKey(void);
Constraint_t *ForeignKey(ForeignKeyRef *fkr);
Constraint_t *Default(Literal_t *val);
Constraint_t *Unique(void);
Constraint_t *Check(Expression *cond);
Constraint_t *ColumnSize(unsigned size);
Constraint_t *Constraint_append(Constraint_t *constraints, Constraint_t *constraint);


void Constraint_print(void *constraint);
void Constraint_printList(Constraint_t *constraints);

#endif
