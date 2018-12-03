
#include "dongmensql/dongmensql.h"

static ssize_t size_constraint = -1;

size_t Column::Column_getSize() {

    return 0;
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

//Column::Column(const char *name, enum data_type type, Constraint_t *constraints) {
//    this->name = strdup(name);
//    this->type = type;
//    this->constraints = constraints;
//
//}

Column::Column(const char *name, enum data_type type, map<string, Constraint*> *constraints) {
    this->name = strdup(name);
    this->type = type;
    this->cons = constraints;

}

void Constraint::Constraint_print(void *constraint_voidp) {
    Constraint_t *constraint = (Constraint_t *) constraint_voidp;
    switch (constraint->t) {
        case CONS_DEFAULT:
            printf("Default: ");
//            Literal_print(constraint->constraint.default_val);
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
