//
// Created by Sam on 2018/1/17.
//

#include <malloc.h>
#include "dongmensql/sqlstatement.h"


sql_stmt_create *parseCreate(Parser *parser) { return NULL; };


/* 打印 sql_stmt_update */
int sql_stmt_update_print(sql_stmt_update *sqlStmtUpdate){
    SRA_print(sqlStmtUpdate->where);
};

