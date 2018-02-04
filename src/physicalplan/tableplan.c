//
// Created by Sam on 2018/2/3.
//

#include "tableplan.h"
table_plan *table_plan_create(dongmengdb *db, char *tableName, transaction *tx){
    table_plan *tablePlan = (table_plan *)calloc(sizeof(table_scan),1);
    tablePlan->tableInfo =  table_manager_get_tableinfo(db->metadataManager->tableManager, tableName, tx);
    tablePlan->tableScan = table_scan_create(tablePlan->tableInfo, tx);
};
table_scan *table_plan_open(table_plan *tablePlan){
    return tablePlan->tableScan;
};
