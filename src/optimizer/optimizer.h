//
// Created by Sam on 2018/1/25.
//

#ifndef DONGMENDB_OPTIMIZER_H
#define DONGMENDB_OPTIMIZER_H
int dongmengdbStmtOptimize(dongmengdb *db,
                             dongmengsqlStatement_t *sqlStmt,
                             dongmengsqlStatement_t **sqlStmtOpt);

#endif //DONGMENDB_OPTIMIZER_H
