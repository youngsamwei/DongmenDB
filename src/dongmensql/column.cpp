
#include "dongmensql/dongmensql.h"

static ssize_t size_constraint = -1;


static void deleteConstraint_ts(Constraint_t *constraint) {
    if (constraint) {
        Constraint_t *next = constraint->next;
        switch (constraint->t) {
            case CONS_DEFAULT:
                Literal_free(constraint->constraint.default_val);
                break;
            case CONS_CHECK:
                constraint->constraint.check->expression_free();
                break;
            default:
                break;
        }
        deleteConstraint_ts(next);
    }
}

size_t Column::Column_getSize() {
    Constraint_t *cons = this->constraints;
    while (cons) {
        if (cons->t == CONS_SIZE)
            return cons->constraint.size;
        cons = cons->next;
    }
    switch (this->type) {
        case DATA_TYPE_CHAR:
            return sizeof(char);
        case DATA_TYPE_DOUBLE:
            return sizeof(double);
        case DATA_TYPE_BOOLEAN:
        case DATA_TYPE_INT:
            return sizeof(int);
        case DATA_TYPE_TEXT:
            return 250; /* default text length */
    }

    return 0;
}


void Constraint_printList(Constraint_t *constraints) {
    int first = 1;
    if (constraints) {
        printf(" [");
        for (; constraints; constraints = constraints->next) {
            if (first) {
                first = 0;
            } else {
                printf(", ");
            }
            Constraint_print(constraints);
        }
        printf("]");
    }
}

ForeignKeyRef::ForeignKeyRef(const char *cname) {
    col_name = cname;
}

ForeignKeyRef:: ForeignKeyRef(const char *foreign_tname,
                                   const char *foreign_cname) {
    col_name = NULL;
    table_name = foreign_tname;
    table_col_name = foreign_cname;
}

Column::Column(const char *name, enum data_type type, Constraint_t *constraints) {
    this->name = strdup(name);
    this->type = type;
    this->constraints = constraints;

}

void Constraint_print(void *constraint_voidp) {
    Constraint_t *constraint = (Constraint_t *) constraint_voidp;
    switch (constraint->t) {
        case CONS_DEFAULT:
            printf("Default: ");
            Literal_print(constraint->constraint.default_val);
            break;
        case CONS_PRIMARY_KEY:
            printf("Primary Key");
            break;
        case CONS_UNIQUE:
            printf("Unique");
            break;
        case CONS_FOREIGN_KEY:
            printf("Foreign key (%s, %s)", constraint->constraint.ref->table_name,
                   constraint->constraint.ref->table_col_name);
            break;
        case CONS_AUTO_INCREMENT:
            printf("Auto increment");
            break;
        case CONS_NOT_NULL:
            printf("Not null");
            break;
        case CONS_CHECK:
            printf("Check: ");
            constraint->constraint.check->expression_print(constraint->constraint.check,NULL);
            break;
        case CONS_SIZE:
            printf("Size: %u", constraint->constraint.size);
            break;
        default:
            printf("Unknown constraint type");
    }
}

Column *Column::Column_append(Column *col2) {
    return next->app_col(Column_append(col2));
}

Column *Column::app_col(Column *col2) {
    next = col2;
}

int Column::Column_compareByName(const void *c1, const void *c2) {
    return strcmp(((Column *) c1)->name, ((Column *) c2)->name);
}

void *Column::Column_copy(void *col) {
    Column *copy = new Column();
    copy->name = strdup(((Column *) col)->name);
    copy->next = NULL; /* just in case */
    return copy;
}

ColumnReference::ColumnReference(const char *tname, const char *cname) {
    if (tname) tableName = strdup(tname);
    if (cname) columnName = strdup(cname);
}

/**
 * 解析字段名，得到ColumnReference  , 两种形式 student.sno,  sno
 * @param allName
 * @return
 */
ColumnReference::ColumnReference(char *allName){
    char delims[] = ".";

    this->allName  = strdup(allName);
    this->tableName = strtok(allName, delims);
    this->columnName = strtok(NULL, delims);
    if (this->columnName == NULL){
        this->columnName = this->tableName;
        this->tableName = NULL;
    }
};
