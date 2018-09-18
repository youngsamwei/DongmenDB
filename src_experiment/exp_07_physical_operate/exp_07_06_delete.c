//
// Created by sam on 2018/9/18.
//
#include "physicalplan/physicalplan.h"

/*执行delete语句*/

int plan_execute_delete(dongmendb *db, sql_stmt_delete *sqlStmtDelete,  transaction *tx){

    /*删除语句以select的物理操作为基础实现。
     * 1. 使用 sql_stmt_delete 的条件参数，调用 physical_scan_select_create 创建select的物理计划并初始化;
     * 2. 执行 select 的物理计划，完成 delete 操作
     * */
};
