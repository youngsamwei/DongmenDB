//
// Created by sam on 2018/11/9.
//

#include <test/test_stmt_parser.h>

class Exp_05_02_ConditionPushdownTest : public TestStmtParser {
protected:
    virtual void SetUp() {

        _m_list[0] = "select student.*, sc.*, course.* from student, sc, course where student.sno = sc.sno and sc.cno = course.cno and course.cno = 'c001' and grade >= 80 and student.sname = 'tom simith'";
        _m_list[1] = "select student.sno, sc.cno,sname,grade, cname from student, sc, course where student.sno = sc.sno and sc.cno = course.cno and grade >= 80";
        _m_list[2] = "select sno,cno,grade, cname from sc, course where sc.cno = course.cno and grade > 80";
        _m_list[3] = "select student.*, grade from student, sc where student.sno = sc.sno and sname = 'tom simith' and grade >= 80";
        _m_list[4] = "select student.*, sc.* from student, sc where student.sno = sc.sno";
        _m_list[5] = "select student.* from student, sc, course where student.sno = sc.sno and sc.cno = course.cno";
        _m_list[6] = "select student.*, sc.*, course.* from student, sc, course where student.sno = sc.sno and sc.cno = course.cno and course.cno = 'c001'";
        _m_list[7] = "select student.*, sc.*, course.* from student, sc, course where student.sno = sc.sno and sc.cno = course.cno and course.cno = 'c001' and grade >= 80";

    }

    const char *_m_list[14];
    const char *dbname = "test_demodb";
};

TEST_F(Exp_05_02_ConditionPushdownTest, Correct) {
/*据指定的数据库名称创建数据库*/
    createDB(dbname);
/*创建表*/
    createTable();
/*增加数据*/
    insertData();

    EXPECT_EQ(0, opt_condition_pushdown_test(_m_list[0]));
    EXPECT_EQ(0, opt_condition_pushdown_test(_m_list[1]));
    EXPECT_EQ(0, opt_condition_pushdown_test(_m_list[2]));
    EXPECT_EQ(0, opt_condition_pushdown_test(_m_list[3]));
    EXPECT_EQ(0, opt_condition_pushdown_test(_m_list[4]));
    EXPECT_EQ(0, opt_condition_pushdown_test(_m_list[5]));
    EXPECT_EQ(0, opt_condition_pushdown_test(_m_list[6]));
    EXPECT_EQ(0, opt_condition_pushdown_test(_m_list[7]));
//EXPECT_EQ(0, opt_condition_pushdown_test(_m_list[8]));
//EXPECT_EQ(0, opt_condition_pushdown_test(_m_list[9]));
//EXPECT_EQ(0, opt_condition_pushdown_test(_m_list[10]));
//EXPECT_EQ(0, opt_condition_pushdown_test(_m_list[11]));
//EXPECT_EQ(0, opt_condition_pushdown_test(_m_list[12]));
//EXPECT_EQ(0, opt_condition_pushdown_test(_m_list[13]));

/*删除数据库*/
    dropDB();
}
