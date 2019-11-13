//
// Created by Sam on 2018/2/13.
//

#include <relationalalgebra/optimizer.h>

/*
 * 等价变换：将SRA_SELECT类型的节点进行条件串接
 * 传入指针的指针作为参数便于修改多于一个值
 */
void splitAndConcatenate(SRA_s *sra_point);

/*
 * 判断表达式的操作数链表中为 TOKEN_AND
 */
bool hasLegalKeyword(Expression *pExpression);

/*
 * 拆分一个 sra 表达式，将 AND 左右分拆成子 select 语句
 */
void splitSra(SRA_s *pS);

/*
 * 当前标识符为 TOKEN_AND 时，获取当前的子表达式，也就是两个 AND
 * （或者只有一个 AND 另一个为 nullptr） 中间的子表达式，将其拆下
 * 长表达式，拆分成一个单独的 select 便于第二步中的 select 与 join
 * 的下推交换处理
 */
Expression *interceptSubexpression(Expression *pExpression);

/*
 * 用于根据传入的表达式头部与表达式尾部找到当前子表达式结束的位置
 */
Expression *goSubexpressionEnd(Expression *pFront, Expression *pBack);

/*
 * 将上一步拆分得到的 select 子语句 下推，寻找到包含 select 所需属性的 join 语句
 * 将 select 语句放至 join 之前（下方）执行，也就是先筛选需要的属性再交
 */
void conditionalExchange(SRA_t *pS, TableManager *tableManager, Transaction *transaction);

/*
 * 检查 column 名是否存在于预定的 fieldsName 中
 */
bool hasColumnNameInFieldsName(const vector<char *> &fieldsName,
                               const char *columnName);

bool hasColumnNameInSra(SRA_t *sra, Expression *pExpression, TableManager *tableManager, Transaction *transaction);

SRA_t *dongmengdb_algebra_optimize_condition_pushdown(SRA_t *sra, TableManager *tableManager, Transaction *transaction)
{

  /*初始关系代数语法树sra由三个操作构成：SRA_PROJECT -> SRA_SELECT -> SRA_JOIN，即对应语法树中三个节点。

  第一步：等价变换：将SRA_SELECT类型的节点进行条件串接

  1.1 在sra中找到每个SRA_Select节点
  1.2 检查每个SRA_Select节点中的条件是不是满足串接条件：多条件且用and连接
  1.3 若满足串接条件则：创建一组新的串接的SRA_Select节点，等价替换当前的SRA_Select节点

  第二步：等价变换：条件交换
  2.1 在sra中找到每个SRA_Select节点
  2.2 对每个SRA_Select节点做以下处理：
   * 在sra中查找 SRA_Select 节点应该在的最优位置：
   *     若子操作也是SRA_Select，则可交换；
   *     若子操作是笛卡尔积，则可交换，需要判断SRA_Select所包含的属性属于笛卡尔积的哪个子操作
   * 最后将SRA_Select类型的节点移动到语法树的最优位置。
   * */

  //选择串接
  splitAndConcatenate(sra);

  //选择交换
  conditionalExchange(sra, tableManager, transaction);

  return sra;
}

/**
 * 本函数用于选择串接
 * @param sra_point
 */
void splitAndConcatenate(SRA_s *sra_point)
{
  //复制指针
  SRA_s *sraS = sra_point;
  //如果为空则直接返回
  if(sraS == nullptr)
    return;

  //如果是投影,则继续往下寻找
  if(sraS->t == SRA_PROJECT)
    splitAndConcatenate(sraS->project.sra);
  else if(sraS->t == SRA_JOIN)
  {
    //如果是连接,则连接的双方进行递归寻找
    splitAndConcatenate(sraS->join.sra1);
    splitAndConcatenate(sraS->join.sra2);
  }
  else if(sraS->t == SRA_SELECT)
  {
    //如果是选择,
    if(hasLegalKeyword(sraS->select.cond))
      splitSra(sraS);
  }
}

void splitSra(SRA_s *ps)
{
  if(ps == nullptr)
    return;

  SRA_s *s = ps;
  Expression *condition = s->select.cond;
  //左侧操作数
  Expression *operandListFront = condition->nextexpr;
  Expression *operandListBack = interceptSubexpression(operandListFront);
  Expression *frontEnd = goSubexpressionEnd(operandListFront, operandListBack);
  frontEnd->nextexpr = nullptr;

  s->select.sra = SRASelect(s->select.sra, operandListFront);
  s->select.cond = operandListBack;
}

Expression *goSubexpressionEnd(Expression *pFront, Expression *pBack)
{
  Expression *expression = pFront;
  while(expression != nullptr && expression->nextexpr != pBack
        && expression->nextexpr != nullptr)
    expression = expression->nextexpr;
  return expression;
}

Expression *interceptSubexpression(Expression *expression)
{
  if(expression == nullptr)
    return expression;
  else if(expression->term == nullptr)
  {
    if(expression->opType <= TOKEN_COMMA)
    {
      int numOfOperators = 0;
      numOfOperators = operators[expression->opType].numbers;
      expression = expression->nextexpr;

      while(numOfOperators--)
        expression = interceptSubexpression(expression);
    }
    return expression;
  }
  else if(expression->term)
    return expression->nextexpr;
  return nullptr;
}

bool hasLegalKeyword(Expression *expression)
{
  return expression == nullptr ? false : expression->opType == TOKEN_AND;
}

bool hasColumnNameInFieldsName(const vector<char *> &fieldsName, const char *columnName)
{
  for(char *name : fieldsName)
    if(strcmp(name, columnName) == 0)
      return true;
  return false;
}

bool hasColumnNameInSra(SRA_t *sra, Expression *expression, TableManager *tableManager, Transaction *transaction)
{
  SRAType sraType = sra->t;
  if(sraType == SRA_SELECT)
    return hasColumnNameInSra(sra->select.sra, expression, tableManager, transaction);
  else if(sraType == SRA_JOIN)
  {
    bool f1 = hasColumnNameInSra(sra->join.sra1, expression, tableManager, transaction);
    bool f2 = hasColumnNameInSra(sra->join.sra2, expression, tableManager, transaction);
    return f1 || f2;
  }
  else if(sraType == SRA_TABLE)
  {
    for(auto point = expression; point != nullptr; point = point->nextexpr)
    {
      if(point->term != nullptr && point->term->t == TERM_COLREF)
      {
        if(point->term->ref->tableName != nullptr)
        {
          if(strcmp(point->term->ref->tableName, sra->table.ref->table_name) == 0)
            return true;
        }
        else
        {
          TableInfo *tableInfo = tableManager->table_manager_get_tableinfo(sra->table.ref->table_name, transaction);
          if(hasColumnNameInFieldsName(tableInfo->fieldsName, point->term->ref->columnName))
            return true;
        }
      }
    }
    return false;
  }
  return false;
}

void conditionalExchange(SRA_t *ps, TableManager *tableManager, Transaction *transaction)
{
  SRA_t *sra = ps;
  if(sra == nullptr)
    return;
  else if(sra->t == SRA_SELECT)
  {
    conditionalExchange(sra->select.sra, tableManager, transaction);
    SRA_t *selectSra = sra->select.sra;
    if(selectSra->t == SRA_SELECT)
    {
      sra->select.sra = selectSra->select.sra;
      selectSra->select.sra = sra;
      ps = selectSra;
      conditionalExchange(selectSra->select.sra, tableManager, transaction);
    }
    else if(selectSra->t == SRA_JOIN)
    {
      bool leftBranch = hasColumnNameInSra(selectSra->join.sra1,
              sra->select.cond, tableManager, transaction);
      bool rightBranch = hasColumnNameInSra(selectSra->join.sra2,
              sra->select.cond, tableManager, transaction);

      if(leftBranch == true && rightBranch == false)
      {
        sra->select.sra = selectSra->join.sra1;
        selectSra->join.sra1 = sra;
        ps = selectSra;

        conditionalExchange(selectSra->join.sra1, tableManager, transaction);
      }
      else if(leftBranch == false && rightBranch == true)
      {
        sra->select.sra = selectSra->join.sra2;
        selectSra->join.sra2 = sra;
        ps = selectSra;

        conditionalExchange(selectSra->join.sra2, tableManager, transaction);
      }
    }
  }
  else if(sra->t == SRA_PROJECT)
    conditionalExchange(sra->project.sra, tableManager, transaction);
  else if(sra->t == SRA_JOIN)
  {
    conditionalExchange(sra->join.sra1, tableManager, transaction);
    conditionalExchange(sra->join.sra2, tableManager, transaction);
  }
}