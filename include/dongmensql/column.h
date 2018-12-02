#ifndef __COLUMN_H_
#define __COLUMN_H_

#include "parser/expression.h"
#include "literal.h"

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

class ForeignKeyRef {
public:
    const char *col_name, *table_name, *table_col_name;

/* constraints on single columns */
    ForeignKeyRef(const char *cname);
    ForeignKeyRef(const char *foreign_tname,
                                     const char *foreign_cname);
} ;

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
