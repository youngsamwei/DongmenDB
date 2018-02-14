//
// Created by Sam on 2018/2/2.
//

#include "dongmendb/transaction.h"
#include <parser/expression.h>
#include <dongmensql/sra.h>
#include "physicalscan.h"

#ifndef DONGMENDB_PLAN_H
#define DONGMENDB_PLAN_H
#ifdef __cplusplus
extern "C" {
#endif

/**
 * 根据sra，选择每个操作的算法, 返回物理计划。
 *
 * @param db
 * @param sra 关系代数表达式
 * @param tx
 * @return physical_scan 物理计划
 */
physical_scan *plan_execute_select(dongmendb *db, SRA_t *sra, transaction *tx);

int plan_execute_delete(dongmendb *db, char *tableName, Expression *condition, transaction *tx);

int plan_execute_update(dongmendb *db, char *tableName, arraylist *fieldNames, hmap_t values, Expression *condition,
                        transaction *tx);

int plan_execute_insert(dongmendb *db, char *tableName, arraylist *fieldNames, arraylist *values, transaction *tx);

int plan_execute_create_table(char *tableName, table_info *tableInfo, transaction *tx);

#ifdef __cplusplus
}
#endif

#endif //DONGMENDB_PLAN_H
