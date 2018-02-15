//
// Created by Sam on 2018/2/11.
//

#include <stdio.h>
#include <stdlib.h>
#include <gtest/gtest.h>
#include "dongmendb/dongmendb.h"
#include "parser/tokenizer.h"
#include "parser/parser.h"
#include "dongmensql/sra.h"
#include "parser/parseSelectStmt.h"
#include "physicalplan/physicalplan.h"


int select(dongmendb *db, const char *sqlselect) {
    char *sql = (char *) calloc(strlen(sqlselect), 1);
    strcpy(sql, sqlselect);
    TokenizerT *tokenizer = TKCreate(sql);
    ParserT *parser = newParser(tokenizer);
    memset(parser->parserMessage, 0, sizeof(parser->parserMessage));

    SRA_t *selectStmt = parse_sql_stmt_select(parser);
    physical_scan *plan = plan_execute_select(db, selectStmt, db->tx);
    plan->beforeFirst(plan);
    int count = 0;
    while (plan->next(plan)) {
        count++;
    }
    plan->close(plan);
    dongmendb_close(db);
    return count;
}

int test(const char *dbname, const char *strselect) {
    dongmendb *newdb = (dongmendb *) calloc(sizeof(dongmendb), 1);
    int rc = dongmendb_open(dbname, newdb);
    int count = select(newdb, strselect);
    return count;
}

class ListTest : public testing::Test {
protected:
    virtual void SetUp() {
        _m_list[0] = "select sno from student";
        _m_list[1] = "select sno from student where sage < 25";
        _m_list[2] = "select sno from student where sage = 25";
    }
    char *_m_list[3];
    const char *dbname = "demodb";
};

TEST_F(ListTest, c1){
    EXPECT_EQ(8, test(dbname, _m_list[0]));
}

TEST_F(ListTest, c2){
    EXPECT_EQ(5, test(dbname, _m_list[1]));
}

TEST_F(ListTest, c3){
    EXPECT_EQ(1, test(dbname, _m_list[2]));
}
