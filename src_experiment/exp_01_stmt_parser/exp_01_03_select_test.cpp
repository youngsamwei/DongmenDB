//
// Created by Sam on 2018/2/11.
//

#include <stdio.h>
#include <stdlib.h>
#include <gtest/gtest.h>
#include <parser/statement.h>
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

class Exp_01_03_SelectTest : public testing::Test {
protected:
    virtual void SetUp() {
        _m_list[0] = "select sno from student";
        _m_list[1] = "select sno from student where sage < 25";
        _m_list[2] = "select sno from student where sage = 25";
        _m_list[3] = "select sno from student where sno ='2012010106'";
        _m_list[4] = "select sno from student where sage >= 25 and sage <= 26";
        _m_list[5] = "select student.sno, sc.cno,sname,grade, cname from student, sc, course where student.sno = sc.sno and sc.cno = course.cno";
        _m_list[6] = "select sno,cno,grade, cname from sc, course where sc.cno = course.cno and grade > 80";
        _m_list[7] = "select student.*, grade from student, sc where student.sno = sc.sno";
        _m_list[8] = "select student.*, sc.* from student, sc where student.sno = sc.sno";
        _m_list[9] = "select student.* from student, sc, course where student.sno = sc.sno and sc.cno = course.cno";
        _m_list[10] = "select student.*, sc.*, course.* from student, sc, course where student.sno = sc.sno and sc.cno = course.cno";
        _m_list[11] = "select sno from student where sname ='tom cruise'";
        _m_list[12] = "select sno from student where sname = 'li simith' or sname='tom cruise'";
        _m_list[13] = "select sno from student where sname = 'zhang simith'";
    }
    const char *_m_list[14];
    const char *dbname = "demodb";
};

TEST_F(Exp_01_03_SelectTest, Correct){
    EXPECT_EQ(9, test(dbname, _m_list[0]));
    EXPECT_EQ(5, test(dbname, _m_list[1]));
    EXPECT_EQ(1, test(dbname, _m_list[2]));
    EXPECT_EQ(1, test(dbname, _m_list[3]));
    EXPECT_EQ(2, test(dbname, _m_list[4]));
    EXPECT_EQ(14, test(dbname, _m_list[5]));
    EXPECT_EQ(6, test(dbname, _m_list[6]));
    EXPECT_EQ(14, test(dbname, _m_list[7]));
    EXPECT_EQ(14, test(dbname, _m_list[8]));
    EXPECT_EQ(14, test(dbname, _m_list[9]));
    EXPECT_EQ(14, test(dbname, _m_list[10]));
    EXPECT_EQ(0, test(dbname, _m_list[11]));
    EXPECT_EQ(0, test(dbname, _m_list[12]));
    EXPECT_EQ(0, test(dbname, _m_list[13]));
}
