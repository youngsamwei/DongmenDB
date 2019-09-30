//
// Created by Sam on 2018/2/11.
//

#include <stdio.h>
#include <stdlib.h>
#include <gtest/gtest.h>
#include <dongmensql/sqlstatement.h>
#include <physicalplan/Scan.h>
#include <physicalplan/ExecutionPlan.h>
#include <physicalplan/Project.h>
#include "dongmendb/DongmenDB.h"
#include "parser/Tokenizer.h"
#include "parser/parser.h"
#include "relationalalgebra/sra.h"


int select(const char *sqlselect) {
    char *sql = (char *) calloc(1, strlen(sqlselect));
    strcpy(sql, sqlselect);
    Tokenizer *tokenizer = new Tokenizer(sql);
    Parser *parser = new Parser(tokenizer);
    memset(parser->parserMessage, 0, sizeof(parser->parserMessage));

    SRA_t *selectStmt = parse_sql_stmt_select(parser);
    ExecutionPlan plan;

    Scan* scan = plan.generateSelect(db, selectStmt, db->tx);
    Project *project = (Project*) scan;
    project->beforeFirst();
    int count = 0;
    while (project->next()) {
        count++;
    }
    project->close();
    dongmendb_close(db);
    return count;
}

int test(const char *dbname, const char *strselect) {
    DongmenDB *newdb = (DongmenDB *) calloc(1, sizeof(DongmenDB));
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