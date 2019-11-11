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

class TableManager;

class FieldInfo;

class TableManager
{
public:
  TableInfo *tcatInfo;
  TableInfo *fcatInfo;

  TableManager(int isNew, Transaction *tx);

  int table_manager_create_table(char *tableName, vector<char *> fieldsName,
                                 map<string, FieldInfo *> *fields,
                                 Transaction *tx);

  TableInfo *table_manager_get_tableinfo(const char *tableName, Transaction *tx);

/*语义检查：表是否存在，在src_experiment\exp_02_semantic\exp_02_01_table_exists.c中实现*/
  int semantic_check_table_exists(char *tableName, Transaction *tx);

/*语义检查：字段是否存在，在src_experiment\exp_02_semantic\exp_02_01_field_exists.c中实现*/
  int semantic_check_field_exists(char *tableName, char *fieldName, Transaction *tx);

};

#endif //DONGMENDB_METADATA_MANAGER_H
