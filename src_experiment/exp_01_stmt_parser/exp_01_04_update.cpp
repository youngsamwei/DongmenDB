//
// Created by Sam on 2018/2/13.
//

#include <dongmensql/sqlstatement.h>
#include <parser/StatementParser.h>
#include <cstring>
#include <string>

/**
 * 在现有实现基础上，实现update from子句
 *
 * 支持的update语法：
 *
 * UPDATE <table_name> SET <field1> = <expr1>[, <field2 = <expr2>, ..]
 * WHERE <logical_expr>
 *
 * 解析获得 sql_stmt_update 结构
 */

/*TODO: parse_sql_stmt_update， update语句解析*/
sql_stmt_update *UpdateParser::parse_sql_stmt_update()
{
  char *tableName = nullptr;
  vector<char *> fields;
  vector<Expression *> fieldsExpr;
  SRA_t *where = nullptr;

  //使用matchToken匹配update
  if(!this->matchToken(TOKEN_RESERVED_WORD, "update"))
    return nullptr;

  //获取表名tableName
  Token *token = parseNextToken();
  //判断表名是否是TOKEN_WORD类型
  if(token->type == TOKEN_WORD)
  {
    // 给字符串指针开新空间
    tableName = new_id_name();
    strcpy(tableName, token->text);
  }
  else
  {
    strcpy(this->parserMessage, "invalid sql: missing table name.");
    return nullptr;
  }

  //使用matchToken匹配set
  token = this->parseEatAndNextToken();
  if(!this->matchToken(TOKEN_RESERVED_WORD, "set"))
  {
    strcpy(this->parserMessage, "invalid sql: should be 'set'");
    return nullptr;
  }
  /*
   * 循环获取更新的表达式：
   * 1.获取字段名（TOKEN_WORD类型），存入fields中;
   * 2.识别并跳过'='（TOKEN_EQ类型）;
   * 3.使用parseExpressionRD()获得新值（或表达式）,存入fieldsExpr中即可获得一个完整的表达式
   */
  token = this->parseNextToken();
  if(token->type == TOKEN_WORD)
  {
    while(token->type == TOKEN_WORD)
    {
      char *fieldName = new_id_name();
      strcpy(fieldName, token->text);
      fields.push_back(fieldName);
      token = this->parseEatAndNextToken();

      //遇到等号
      if(token->type == TOKEN_EQ)
      {
        token = this->parseEatAndNextToken();
        //解析等号后面的表达式
        Expression *expression = this->parseExpressionRD();
        fieldsExpr.push_back(expression);
        token = this->parseNextToken();
        if(token == nullptr)
          break;
        if(token->type == TOKEN_COMMA)
          token = this->parseEatAndNextToken();
        else
          break;
      }
      else
      {
        strcpy(this->parserMessage, "invalid sql: missing '='.");
        return nullptr;
      }
    }
  }
  else
  {
    strcpy(this->parserMessage, "invalid sql: missing field name.");
    return nullptr;
  }

  TableReference_t *tableReference = TableReference_make(tableName, nullptr);
  SRA_t *table = SRATable(tableReference);
  where = table;

  //如果没有where
  if(token == nullptr || token->type == TOKEN_SEMICOLON)
    where = table;
  else if(token->type == TOKEN_RESERVED_WORD)
  {
    //匹配where
    if(!this->matchToken(TOKEN_RESERVED_WORD, "where"))
    {
      strcpy(this->parserMessage, "invalid sql: missing where.");
      return nullptr;
    }
    //解析where语句
    SRA_t *table = SRATable(tableReference);
    Expression *whereExpression = this->parseExpressionRD();
    if(this->parserStateType == PARSER_WRONG)
      return nullptr;
    SRA_t *select = SRASelect(table, whereExpression);
    where = select;
  }

  sql_stmt_update *sqlStmtUpdate = static_cast<sql_stmt_update *>(calloc(sizeof(sql_stmt_update), 1));
  sqlStmtUpdate->tableName = tableName;
  sqlStmtUpdate->fields = fields;
  sqlStmtUpdate->fieldsExpr = fieldsExpr;
  sqlStmtUpdate->where = where;
  return sqlStmtUpdate;
}