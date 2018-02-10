//
// Created by Sam on 2018/1/26.
//

#ifndef DONGMENDB_METADATA_MANAGER_H
#define DONGMENDB_METADATA_MANAGER_H

#include "recordfile.h"
#include "transaction.h"

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

typedef struct table_manager_ table_manager;
typedef struct metadata_manager_{
    table_manager *tableManager;

}metadata_manager;

typedef struct table_manager_ {
    table_info *tcatInfo;
    table_info *fcatInfo;
}table_manager;

int metadata_manager_create(metadata_manager *metadataManager, const char *file, transaction *tx, int isNew);
table_manager *table_manager_create(int isNew, transaction *tx) ;
int table_manager_create_table(table_manager *tableManager, char *tableName, arraylist *fieldsName, hmap_t fields, transaction *tx);
table_info *table_manager_get_tableinfo(table_manager *tableManager, char *tableName, transaction *tx);

#endif //DONGMENDB_METADATA_MANAGER_H
