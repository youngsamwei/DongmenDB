//
// Created by Sam on 2018/2/3.
//

#ifndef DONGMENDB_TABLEPLAN_H
#define DONGMENDB_TABLEPLAN_H

#include <recordfile.h>
#include "tablescan.h"

typedef struct table_plan_{
    dongmengdb *db;
    table_info *tableInfo;
    //stats_info *statsInfo;
    table_scan *tableScan;
    transaction *tx;
}table_plan;

table_plan *table_plan_create(dongmengdb *db, char *tableName, transaction *tx);
table_scan *table_plan_open(table_plan *tablePlan);

#endif //DONGMENDB_TABLEPLAN_H
