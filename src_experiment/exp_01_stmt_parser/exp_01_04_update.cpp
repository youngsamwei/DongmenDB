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
//  fprintf(stderr, "TODO: update is not implemented yet. in parse_sql_stmt_update \n");
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
    memmove(tableName, token->text, strlen(token->text));
    //将表名用SRA_t包裹并传入where
    SRA_t *sraTableName = static_cast<SRA_t *>(malloc(sizeof(SRA_t)));
    sraTableName->t = SRA_TABLE;
    SRA_Table_t *sraTable = static_cast<SRA_Table_t *>(malloc(sizeof(sraTable)));
    sraTable->ref = static_cast<TableReference_t *>(malloc(sizeof(TableReference_t)));
    memmove(sraTable->ref->table_name, tableName, strlen(tableName));
    where = sraTableName;
  }
  else
  {
    char const *message = std::string("invalid sql: missing table name.").c_str();
    memmove(this->parserMessage, message, strlen(message));
    return nullptr;
  }

  //使用matchToken匹配set
  token = parseEatAndNextToken();
  if(!this->matchToken(TOKEN_RESERVED_WORD, "set"))
  {
    char const *message = std::string("invalid sql: should be 'set'").c_str();
    memmove(this->parserMessage, message, strlen(message));
    return nullptr;
  }

  /*
   * 循环获取更新的表达式：
   * 1.获取字段名（TOKEN_WORD类型），存入fields中;
   * 2.识别并跳过'='（TOKEN_EQ类型）;
   * 3.使用parseExpressionRD()获得新值（或表达式）,存入fieldsExpr中即可获得一个完整的表达式
   */
  while((token = this->parseEatAndNextToken()))
  {
    if(this->matchToken(TOKEN_RESERVED_WORD, "where"))
      break;
    if(token->type == TOKEN_EQ)
      continue;
  }
};