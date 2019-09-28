//
// Created by Sam on 2018/2/13.
//

#include <dongmensql/sqlstatement.h>
#include <parser/StatementParser.h>
#include <cstring>

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
  vector<char*> fields;
  vector<Expression*> fieldsExpr;
  SRA_t *where = nullptr;

  //使用matchToken匹配update
  if(!this->matchToken(TOKEN_RESERVED_WORD, "update"))
    return nullptr;

  //获取表名tableName
  Token *token = parseNextToken();
  //判断表名是否是TOKEN_WORD类型
  if(token->type == TOKEN_WORD)
  {
    tableName = new_id_name();
    memmove(tableName, token->text, sizeof(token->text));
    SRA_t *sraTableName = static_cast<SRA_t *>(malloc(sizeof(SRA_t)));
    sraTableName->t = SRA_TABLE;
    SRA_Table_t *sraTable = static_cast<SRA_Table_t *>(malloc(sizeof(sraTable)));
    sraTable->ref = static_cast<TableReference_t *>(malloc(sizeof(TableReference_t)));
    memmove(sraTable->ref->table_name, tableName, sizeof(tableName));

    // TODO:将值传进where
  }
  else
  {
    char *message = "invalid sql: missing table name.";
    memmove(this->parserMessage, message, sizeof(message));
    return nullptr;
  }

  //使用matchToken匹配set
  token = parseEatAndNextToken();
  if(!this->matchToken(TOKEN_RESERVED_WORD, "set"))
  {
    char *message = "invalid sql: should be 'set'";
    memmove(this->parserMessage, message, sizeof(message));
    return nullptr;
  }
};