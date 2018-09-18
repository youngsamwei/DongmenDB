//
// Created by sam on 2018/9/18.
//
#include <parser/statement.h>
#include "physicalplan/physicalplan.h"

/*执行 update 语句*/

int plan_execute_update(dongmendb *db, sql_stmt_update *sqlStmtUpdate , transaction *tx){
    /*删除语句以select的物理操作为基础实现。
     * 1. 使用 sql_stmt_update 的条件参数，调用 physical_scan_select_create 创建select的物理计划并初始化;
     * 2. 执行 select 的物理计划，完成update操作
     * */

    fprintf(stderr, "TODO: update is not implemented yet. in plan_execute_update \n");

};