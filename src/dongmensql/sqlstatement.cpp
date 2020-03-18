//
// Created by Sam on 2018/1/17.
//

#include <dongmensql/expression.h>
#include <relationalalgebra/sra.h>
#include <ostream>
#include <malloc.h>
#include "dongmensql/sqlstatement.h"


sql_stmt_create *parseCreate(Parser *parser) { return NULL; };


/* 打印 sql_stmt_update */
int sql_stmt_update_print(sql_stmt_update *sqlStmtUpdate) {
    SRA_print(sqlStmtUpdate->where);
};

ostream &operator<<(ostream &os, const sql_stmt_update &stmt) {
    os << '[' << stmt.tableName << ', ';
    if (!stmt.fields.empty()) {
        os << '(' << stmt.fields[0];
        for (auto it = ++stmt.fields.cbegin(); it != stmt.fields.cend(); ++it) {
            os << ", " << *it;
        }
        os << "), ";
    }
    os << stmt.fieldsExpr << ', ';
    os << "where" << std::endl << stmt.where << std::endl << ']';
    return os;
};
