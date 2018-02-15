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

TEST(test_01_03_select, c1){
    const char *dbname = "demodb";
    const char *strselect = "select sno from student";
    EXPECT_EQ(8, test(dbname, strselect));
}

TEST(test_01_03_select, c2){
    const char *dbname = "demodb";
    const char *strselect = "select sno from student where sage < 25";
    EXPECT_EQ(5, test(dbname, strselect));
}

GTEST_API_ int main(int argc, char** argv){
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}