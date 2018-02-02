//
// Created by Sam on 2018/1/25.
//

#ifndef DONGMENDB_OPTIMIZER_H
#define DONGMENDB_OPTIMIZER_H
int dongmengdb_stmt_optimize(dongmengdb *db,
                             dongmengsql_statement_t *sqlStmt,
                             dongmengsql_statement_t **sqlStmtOpt);

#endif //DONGMENDB_OPTIMIZER_H
