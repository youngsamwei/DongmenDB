//
// Created by Sam on 2018/1/25.
//

#include <dongmendb.h>
#include <dongmensql.h>
#include <mem.h>
#include <malloc.h>
#include "optimizer.h"


int dongmendb_stmt_optimize(dongmendb *db,
                             dongmensql_statement_t *sqlStmt,
                             dongmensql_statement_t **sqlStmtOpt){

    *sqlStmtOpt = malloc(sizeof(dongmensql_statement_t));
    memcpy(*sqlStmtOpt, sqlStmt, sizeof(dongmensql_statement_t));

    return DONGMENDB_OK;

};
