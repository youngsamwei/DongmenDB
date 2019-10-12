//
// Created by Sam on 2018/2/13.
//

#include <dongmensql/sqlstatement.h>
#include <parser/StatementParser.h>

/**
 * 在现有实现基础上，实现delete from子句
 *
 *  支持的delete语法：
 *
 *  DELETE FROM <table_nbame>
 *  WHERE <logical_expr>
 *
 * 解析获得 sql_stmt_delete 结构
 */

sql_stmt_delete *DeleteParser::parse_sql_stmt_delete(){
  char *tableName = nullptr;
  SRA_t *where = nullptr;
  Token *token = this->parseNextToken();
  //匹配delete
  if(!this->matchToken(TOKEN_RESERVED_WORD, (char*)"delete"))
  {
    strcpy(this->parserMessage, "语法错误.");
    return nullptr;
  }

  //获取表名
  token = this->parseNextToken();
  if(token->type != TOKEN_WORD)
  {
    strcpy(this->parserMessage, "语法错误.");
    return nullptr;
  }
  tableName = new_id_name();
  strcpy(tableName, token->text);

  //如果没有where
  TableReference_t *tableReference = TableReference_make(tableName, nullptr);
  where = SRATable(tableReference);
  token = this->parseNextToken();
  if(token == nullptr || token->type == TOKEN_SEMICOLON)
  {
    auto *sqlStmtDelete = static_cast<sql_stmt_delete *>(calloc(sizeof(sqlStmtDelete), 1));
    sqlStmtDelete->where = where;
    sqlStmtDelete->tableName = tableName;
    return sqlStmtDelete;
  }

  //匹配where
  token = this->parseNextToken();
  if(!this->matchToken(TOKEN_RESERVED_WORD, "where"))
  {
    strcpy(this->parserMessage, "语法错误.");
    return nullptr;
  }

  //解析where子句中的条件表达式
  token = this->parseNextToken();
  Expression *expression = this->parseExpressionRD();
  if(parserStateType == PARSER_WRONG)
    return nullptr;
  where = SRASelect(where, expression);

  auto *sqlStmtDelete = static_cast<sql_stmt_delete *>(calloc(sizeof(sql_stmt_delete), 1));
  sqlStmtDelete->where = where;
  sqlStmtDelete->tableName = tableName;
  return sqlStmtDelete;
};