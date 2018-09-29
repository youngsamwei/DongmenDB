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

/* 这个测试用例设计的还是不完善，只能做简单的测试，尚不能准确的测试是否解析的正确.*/

int update(dongmendb *db, const char *strupdate) {
    char *sql = (char *) calloc(strlen(strupdate), 1);
    strcpy(sql, strupdate);
    TokenizerT *tokenizer = TKCreate(sql);
    ParserT *parser = newParser(tokenizer);
    memset(parser->parserMessage, 0, sizeof(parser->parserMessage));

    sql_stmt_update *sqlStmtUpdate  = parse_sql_stmt_update(parser);

    int count = 1;

    if (sqlStmtUpdate == NULL){
        count = 0;
    }
    dongmendb_close(db);
    return count;
}

int test(const char *dbname, const char *strupdate) {
    dongmendb *newdb = (dongmendb *) calloc(sizeof(dongmendb), 1);
    int rc = dongmendb_open(dbname, newdb);
    int count = update(newdb, strupdate);
    return count;
}

class Exp_01_04_UpdateTest : public testing::Test {
protected:
    virtual void SetUp() {
        _m_list[0] = R"(update student set sname = "Tom Cruise" where sno = "001")";
        _m_list[1] = R"(update student set sname = "张鹏" where sname = "张三")";
        _m_list[2] = R"(update student set sname = "张鹏", ssex="male" where sname = "张三")";
        _m_list[3] = R"(update student set sname = "张鹏", ssex="male", sage = sage + 1 where sname = "张三")";
        _m_list[4] = R"(update student set sage = sage + 1 where sname ="'张三")";
        _m_list[5] = R"(update student set sage = sage + 1 )";
    }
    const char *_m_list[11];
    const char *dbname = "demodb";
};

TEST_F(Exp_01_04_UpdateTest, Correct){
    EXPECT_EQ(1, test(dbname, _m_list[0]));
    EXPECT_EQ(1, test(dbname, _m_list[1]));
    EXPECT_EQ(1, test(dbname, _m_list[2]));
    EXPECT_EQ(1, test(dbname, _m_list[3]));
    EXPECT_EQ(1, test(dbname, _m_list[4]));
    EXPECT_EQ(1, test(dbname, _m_list[5]));

}
