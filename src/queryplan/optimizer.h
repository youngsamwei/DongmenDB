//
// Created by Sam on 2018/1/25.
//

#ifndef DONGMENDB_OPTIMIZER_H
#define DONGMENDB_OPTIMIZER_H
int dongmendb_stmt_optimize(dongmendb *db,
                             dongmensql_statement_t *sqlStmt,
                             dongmensql_statement_t **sqlStmtOpt);

#endif //DONGMENDB_OPTIMIZER_H
