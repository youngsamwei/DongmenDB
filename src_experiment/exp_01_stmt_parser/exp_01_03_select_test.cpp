//
// Created by Sam on 2018/2/11.
//

#include <stdio.h>
#include <stdlib.h>
#include <gtest/gtest.h>
#include <parser/statement.h>
#include <test/test_stmt_parser.h>
#include "physicalplan/physicalplan.h"


class Exp_01_03_SelectTest : public TestStmtParser {
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
    const char *dbname = "test_demodb";
};

TEST_F(Exp_01_03_SelectTest, Correct){
    /*据指定的数据库名称创建数据库*/
    createDB(dbname);
/*创建表*/
    createTable();
/*增加数据*/
    insertData();

    EXPECT_EQ(9, select( _m_list[0]));
    EXPECT_EQ(5, select(_m_list[1]));
    EXPECT_EQ(1, select(_m_list[2]));
    EXPECT_EQ(1, select(_m_list[3]));
    EXPECT_EQ(2, select(_m_list[4]));
    EXPECT_EQ(14, select(_m_list[5]));
    EXPECT_EQ(6, select(_m_list[6]));
    EXPECT_EQ(14, select(_m_list[7]));
    EXPECT_EQ(14, select(_m_list[8]));
    EXPECT_EQ(14, select(_m_list[9]));
    EXPECT_EQ(14, select(_m_list[10]));
    EXPECT_EQ(0, select(_m_list[11]));
    EXPECT_EQ(1, select(_m_list[12]));
    EXPECT_EQ(0, select(_m_list[13]));

    /*删除数据库*/
    dropDB();
}
