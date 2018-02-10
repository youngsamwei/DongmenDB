//
// Created by Sam on 2018/2/2.
//

#include "../dongmengdb/transaction.h"
#include <expression.h>
#include <sra.h>
#include "physicalscan.h"

#ifndef DONGMENDB_PLAN_H
#define DONGMENDB_PLAN_H

/**
 * 根据sra，选择每个操作的算法, 返回物理计划。
 *
 * @param db
 * @param sra 关系代数表达式
 * @param tx
 * @return physical_scan 物理计划
 */
physical_scan *plan_execute_select(dongmengdb *db, SRA_t *sra, transaction *tx);

int plan_execute_delete(dongmengdb *db, char *tableName, Expression *condition, transaction *tx);

int plan_execute_update(dongmengdb *db, char *tableName, arraylist *fieldNames, hmap_t values, Expression *condition, transaction *tx);

int plan_execute_insert(dongmengdb *db, char *tableName, arraylist *fieldNames, arraylist *values, transaction *tx);

int plan_execute_create_table(char *tableName, table_info *tableInfo, transaction *tx);


#endif //DONGMENDB_PLAN_H
