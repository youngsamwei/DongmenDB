//
// Created by Sam on 2018/2/13.
//

#include <dongmendb/integritymanager.h>

/*
 * 完整性管理实验需要实现：
 * 1. 完整性定义：完整性约束的语句解析
 * 2. 完整性约束存储：存储在什么结构的数据表中；
 * 3. 完整性检查：当insert，update，delete时检查完整性约束
 * 4. 完整性处理：违反约束时拒绝或级联
 *
 * */

int integrity_manager_entity_constraint_check(integrity_manager *integrityManager, table_info *tableInfo, record_value *recordValue, sql_update_type sqlUpdateType){

};
int integrity_manager_reference_constraint_check(integrity_manager *integrityManager, table_info *tableInfo, record_value *recordValue, sql_update_type sqlUpdateType){

};
int integrity_manager_check_constraint_check(integrity_manager *integrityManager, table_info *tableInfo, record_value *recordValue){

};
int integrity_manager_null_constraint_check(integrity_manager *integrityManager, table_info *tableInfo, record_value *recordValue){

};
