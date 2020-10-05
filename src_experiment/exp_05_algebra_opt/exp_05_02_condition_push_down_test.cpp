//
// Created by sam on 2018/11/9.
//

#include <gtest/gtest.h>
#include <test/test_stmt_parser.h>
#include <test/test_stmt_optimized.h>

class ConditionPushDownOptimizeTest : public TestStmtOptimized {
protected:
    void SetUp() override {
        createDB(dbname);
        createTable();
    }

    void TearDown() override {
        dropDB();
    }

    const char *dbname = "test_demodb";
};


TEST_F(ConditionPushDownOptimizeTest, Test1) {
    EXPECT_EQ(0, opt_condition_pushdown_test(
            "select student.*, sc.*, course.* from student, sc, course where student.sno = sc.sno and sc.cno = course.cno and course.cno = 'c001' and grade >= 80 and student.sname = 'tom simith'"));
}

TEST_F(ConditionPushDownOptimizeTest, Test2) {
    EXPECT_EQ(0, opt_condition_pushdown_test(
            "select student.sno, sc.cno,sname,grade, cname from student, sc, course where student.sno = sc.sno and sc.cno = course.cno and grade >= 80"));
}

TEST_F(ConditionPushDownOptimizeTest, Test3) {
    EXPECT_EQ(0, opt_condition_pushdown_test(
            "select sno,cno,grade, cname from sc, course where sc.cno = course.cno and grade > 80"));
}

TEST_F(ConditionPushDownOptimizeTest, Test4) {
    EXPECT_EQ(0, opt_condition_pushdown_test(
            "select student.*, grade from student, sc where student.sno = sc.sno and sname = 'tom simith' and grade >= 80"));
}

TEST_F(ConditionPushDownOptimizeTest, Test5) {
    EXPECT_EQ(0, opt_condition_pushdown_test("select student.*, sc.* from student, sc where student.sno = sc.sno"));
}

TEST_F(ConditionPushDownOptimizeTest, Test6) {
    EXPECT_EQ(0, opt_condition_pushdown_test(
            "select student.* from student, sc, course where student.sno = sc.sno and sc.cno = course.cno"));
}

TEST_F(ConditionPushDownOptimizeTest, Test7) {
    EXPECT_EQ(0, opt_condition_pushdown_test(
            "select student.*, sc.*, course.* from student, sc, course where student.sno = sc.sno and sc.cno = course.cno and course.cno = 'c001'"));
}

TEST_F(ConditionPushDownOptimizeTest, Test8) {
    EXPECT_EQ(0, opt_condition_pushdown_test(
            "select student.*, sc.*, course.* from student, sc, course where student.sno = sc.sno and sc.cno = course.cno and course.cno = 'c001' and grade >= 80"));
}
