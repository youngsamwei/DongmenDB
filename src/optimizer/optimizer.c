//
// Created by Sam on 2018/1/25.
//

#include <dongmengdb.h>
#include <dongmengsql.h>
#include <mem.h>
#include <malloc.h>
#include "optimizer.h"


int dongmengdb_stmt_optimize(dongmengdb *db,
                             dongmengsql_statement_t *sqlStmt,
                             dongmengsql_statement_t **sqlStmtOpt){

    *sqlStmtOpt = malloc(sizeof(dongmengsql_statement_t));
    memcpy(*sqlStmtOpt, sqlStmt, sizeof(dongmengsql_statement_t));

    return DONGMENGDB_OK;

};
