//
// Created by Sam on 2018/1/25.
//

#include <dongmengdb.h>
#include <dongmengsql.h>
#include <mem.h>
#include <malloc.h>
#include "optimizer.h"


int dongmengdbStmtOptimize(dongmengdb *db,
                             dongmengsqlStatement_t *sqlStmt,
                             dongmengsqlStatement_t **sqlStmtOpt){

    *sqlStmtOpt = malloc(sizeof(dongmengsqlStatement_t));
    memcpy(*sqlStmtOpt, sqlStmt, sizeof(dongmengsqlStatement_t));

    return DONGMENGDB_OK;

};
