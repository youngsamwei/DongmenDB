//
// Created by Sam on 2018/1/26.
//

#ifndef DONGMENDB_METADATA_MANAGER_H
#define DONGMENDB_METADATA_MANAGER_H

#include <vector>
#include <map>

#include "Recordfile.h"
#include "Transaction.h"

using namespace std;


/**
 * 元数据管理
 * 包括：
 * 数据表管理：用于存储对象的描述（如表数据结构：字段，类型，约束等）
 * 视图管理：
 * 统计信息管理： 用于优化的统计信息：字段值的分布，表的大小等。
 * 索引管理：
 *
 *
 *
 */

class Transaction;
typedef struct field_info_ field_info;
typedef struct table_manager_ table_manager;
typedef struct metadata_manager_ {
    table_manager *tableManager;

} metadata_manager;

typedef struct table_manager_ {
    table_info *tcatInfo;
    table_info *fcatInfo;
} table_manager;

int metadata_manager_create(metadata_manager *metadataManager, const char *file, Transaction *tx, int isNew);

table_manager *table_manager_create(int isNew, Transaction *tx);

int table_manager_create_table(table_manager *tableManager, char *tableName, vector<char *> fieldsName,
                               map<string, field_info*> *fields,
                               Transaction *tx);

table_info *table_manager_get_tableinfo(table_manager *tableManager, const char *tableName, Transaction *tx);

/*语义检查：表是否存在，在src_experiment\exp_02_semantic\exp_02_01_table_exists.c中实现*/
int semantic_check_table_exists(table_manager *tableManager, char *tableName, Transaction *tx);

/*语义检查：字段是否存在，在src_experiment\exp_02_semantic\exp_02_01_field_exists.c中实现*/
int semantic_check_field_exists(table_manager *tableManager, char *tableName, char *fieldName, Transaction *tx);


#endif //DONGMENDB_METADATA_MANAGER_H
