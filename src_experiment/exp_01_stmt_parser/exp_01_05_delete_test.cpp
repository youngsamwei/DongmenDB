//
// Created by Sam on 2018/2/11.
//

#include <stdio.h>
#include <stdlib.h>
#include <gtest/gtest.h>
#include <parser/statement.h>
#include "dongmendb/dongmendb.h"
#include "parser/tokenizer.h"
#include "parser/parser.h"
#include "dongmensql/sra.h"
#include "physicalplan/physicalplan.h"


int delete_(dongmendb *db, const char *strdelete) {
    char *sql = (char *) calloc(strlen(strdelete), 1);
    strcpy(sql, strdelete);
    TokenizerT *tokenizer = TKCreate(sql);
    ParserT *parser = newParser(tokenizer);
    memset(parser->parserMessage, 0, sizeof(parser->parserMessage));

    sql_stmt_delete *sqlStmtDelete  = parse_sql_stmt_delete(parser);

    /*返回修改的记录条数*/
    int count  = 0;
    count = plan_execute_delete(db, sqlStmtDelete, db->tx);

    dongmendb_close(db);
    return count;
}

int test(const char *dbname, const char *strdelete) {
    dongmendb *newdb = (dongmendb *) calloc(sizeof(dongmendb), 1);
    int rc = dongmendb_open(dbname, newdb);
    int count = delete_(newdb, strdelete);
    return count;
}

class Exp_01_05_DeleteTest : public testing::Test {
protected:
    virtual void SetUp() {
        _m_list[0] = "delete from student where sname = 'Tom Cruise'";
        _m_list[1] = "delete from student where sname = '张三'";
        _m_list[2] = "delete from student  where sname = '张三' and ssex='male'";
        _m_list[3] = "delete from student where sname = '张三' and ssex='male' and sage  = 19";
    }
    const char *_m_list[11];
    const char *dbname = "demodb";
};

TEST_F(Exp_01_05_DeleteTest, Correct){
    EXPECT_EQ(1, test(dbname, _m_list[0]));
    EXPECT_EQ(1, test(dbname, _m_list[1]));
    EXPECT_EQ(1, test(dbname, _m_list[2]));
    EXPECT_EQ(1, test(dbname, _m_list[3]));

}
