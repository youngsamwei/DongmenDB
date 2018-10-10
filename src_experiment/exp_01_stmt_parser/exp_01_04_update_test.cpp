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

    if(sqlStmtUpdate == NULL){
        printf(parser->parserMessage);
        return -1;
    }

    sql_stmt_update_print(sqlStmtUpdate);

    /*返回修改的记录条数*/
    int count  = 0;
    count = plan_execute_update(db, sqlStmtUpdate, db->tx);

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
        _m_list[0] = "update student set sname = 'Tom Cruise' where sno = '2012010101' ";
        _m_list[1] = "update student set sname = 'zhang simith' where sname = 'li simith' ";
        _m_list[2] = "update student set sname = 'li simith', ssex= 'male' where sname = 'zhang simith'; ";
        _m_list[3] = "update student set sname = 'zhang simith', ssex= 'male', sage = sage + 1 where sname = 'li simith'";
        _m_list[4] = "update student set sname = 'li simith' where sname = 'zhang simith'";
        _m_list[5] = "update student set sage = sage + 1 where sname = 'li simith'";
        _m_list[6] = "update student set sage = sage + 1 " ;
        _m_list[7] = "update student set sage = 20 where sno = '2012010101'";
        _m_list[8] = "update student set sname = 'To mCruise' where sage > 30; ";
    }
    const char *_m_list[11];
    const char *dbname = "demodb";
};

TEST_F(Exp_01_04_UpdateTest, Correct){
    EXPECT_EQ(1, test(dbname, _m_list[0])); //parse error
    EXPECT_EQ(1, test(dbname, _m_list[1])); //parse error
    EXPECT_EQ(1, test(dbname, _m_list[2])); //parse error
    EXPECT_EQ(1, test(dbname, _m_list[3]));
    EXPECT_EQ(1, test(dbname, _m_list[4]));
    EXPECT_EQ(1, test(dbname, _m_list[5]));
    EXPECT_EQ(9, test(dbname, _m_list[6]));
    EXPECT_EQ(1, test(dbname, _m_list[7]));
    EXPECT_EQ(0, test(dbname, _m_list[8]));

}
