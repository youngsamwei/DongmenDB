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


int ExecutionPlan::executeUpdate(DongmenDB *db, sql_stmt_update *sqlStmtUpdate, Transaction *tx)
{
  /*删除语句以select的物理操作为基础实现。
   * 1. 使用 sql_stmt_update 的条件参数，调用 physical_scan_select_create 创建select的物理计划并初始化;
   * 2. 执行 select 的物理计划，完成update操作
   * */

  //记录更新的条数
  size_t updated_count = 0;
  //创建一个扫描
  Scan *scan = generateScan(db, sqlStmtUpdate->where, tx);
  //将扫描指针放到最前
  scan->beforeFirst();
  //利用循环进行遍历
  while(scan->next())
  {
    //针对每一条的每一个字段进行操作
    for(size_t i = 0; i < sqlStmtUpdate->fieldsExpr.size(); ++i)
    {
      //首先获取字段名
      char *currentFieldName = new_id_name();
      memmove(currentFieldName, sqlStmtUpdate->fields[i], strlen(sqlStmtUpdate->fields[i]) * sizeof(char));
      //创建一个可变类型结构体
      auto *var = static_cast<variant *>(calloc(sizeof(variant), 1));
      //获取要更新的字段数据类型
      enum data_type field_type = scan->getField(sqlStmtUpdate->tableName, currentFieldName)->type;
      //计算表达式,获取更新的值的数据类型
      scan->evaluateExpression(sqlStmtUpdate->fieldsExpr[i], scan, var);
      //检查类型是否匹配
      //如果不匹配
      if(var->type != field_type)
      {
        fprintf(stdout, "error");
        return DONGMENDB_EINVALIDSQL;
      }
      //匹配类型为Int
      if(var->type == DATA_TYPE_INT)
      {
        //进行更新
        scan->setInt(sqlStmtUpdate->tableName, currentFieldName, var->intValue);
      }
      //匹配类型为String
      else if(var->type == DATA_TYPE_CHAR)
      {
        //进行更新
        scan->setString(sqlStmtUpdate->tableName, currentFieldName, var->strValue);
        //TODO: 如果字符串长度超过了定义的长度，要进行截取处理
      }
      else
        return DONGMENDB_EINVALIDSQL;
    }
    //增加条数
    ++updated_count;
  }
  //关闭scan
  scan->close();
  return updated_count;
};