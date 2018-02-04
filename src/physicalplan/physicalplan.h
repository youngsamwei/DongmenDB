//
// Created by Sam on 2018/2/2.
//

#include <transaction.h>
#include <expression.h>

#ifndef DONGMENDB_PLAN_H
#define DONGMENDB_PLAN_H

int plan_execute_select();

int plan_execute_delete(dongmengdb *db, char *tableName, Expression *condition, transaction *tx);

int plan_execute_update(dongmengdb *db, char *tableName, arraylist *fieldNames, hmap_t values, Expression *condition, transaction *tx);

int plan_execute_insert(dongmengdb *db, char *tableName, arraylist *fieldNames, arraylist *values, transaction *tx);

int plan_execute_create_table(char *tableName, table_info *tableInfo, transaction *tx);


#endif //DONGMENDB_PLAN_H
