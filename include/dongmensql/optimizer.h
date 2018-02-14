//
// Created by Sam on 2018/1/25.
//

#ifndef DONGMENDB_OPTIMIZER_H
#define DONGMENDB_OPTIMIZER_H

#ifdef __cplusplus
extern "C" {
#endif

int dongmendb_stmt_optimize(dongmendb *db,
                            dongmensql_statement_t *sqlStmt,
                            dongmensql_statement_t **sqlStmtOpt);

#ifdef __cplusplus
}
#endif

#endif //DONGMENDB_OPTIMIZER_H
