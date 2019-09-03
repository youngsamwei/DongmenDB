//
// Created by sam on 2018/9/18.
//
#include <physicalplan/ExecutionPlan.h>
#include <physicalplan/TableScan.h>
#include <physicalplan/Select.h>
#include <physicalplan/Project.h>
#include <physicalplan/Join.h>

/*执行delete语句的物理计划，返回删除的记录条数
 * 返回大于等于0的值，表示删除的记录条数；
 * 返回小于0的值，表示删除过程中出现错误。
 * */

int ExecutionPlan::executeDelete(DongmenDB *db, sql_stmt_delete *sqlStmtDelete, Transaction *tx){
        /*删除语句以select的物理操作为基础实现。
     * 1. 使用 sql_stmt_delete 的条件参数，调用 physical_scan_select_create 创建select的物理计划并初始化;
     * 2. 执行 select 的物理计划，完成 delete 操作
     * */
    return -1;
};