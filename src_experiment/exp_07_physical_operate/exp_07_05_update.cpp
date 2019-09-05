//
// Created by sam on 2018/9/18.
//

#include <physicalplan/ExecutionPlan.h>
#include <physicalplan/TableScan.h>
#include <physicalplan/Select.h>
#include <physicalplan/Project.h>
#include <physicalplan/Join.h>

/*执行 update 语句的物理计划，返回修改的记录条数
 * 返回大于等于0的值，表示修改的记录条数；
 * 返回小于0的值，表示修改过程中出现错误。
 * */
/*TODO: plan_execute_update， update语句执行*/


int ExecutionPlan::executeUpdate(DongmenDB *db, sql_stmt_update *sqlStmtUpdate, Transaction *tx){
    /*删除语句以select的物理操作为基础实现。
     * 1. 使用 sql_stmt_update 的条件参数，调用 physical_scan_select_create 创建select的物理计划并初始化;
     * 2. 执行 select 的物理计划，完成update操作
     * */
    return -1;
};