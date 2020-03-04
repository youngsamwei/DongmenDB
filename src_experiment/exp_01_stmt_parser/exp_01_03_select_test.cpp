//
// Created by Sam on 2018/2/11.
//


#include <cstdio>
#include <gtest/gtest.h>
#include <map>
#include <parser/StatementParser.h>
#include <parser/Tokenizer.h>
#include <test/test_stmt_parser.h>

class Exp_01_03_SelectTest : public testing::Test {
protected:
    void SetUp() override {
        // 基本查询
        sqlList[0] = "select * from student";
        // 投影的解析
        sqlList[1] = "select student.* from student";
        sqlList[2] = "select sno from student";
        sqlList[3] = "select sno, sname from student";
        // 条件表达式的解析
        sqlList[4] = "select sno from student where sage < 25";
        // 多表查询
        sqlList[5] = "select sname, grade from student, sc where student.sno = sc.sno";
        sqlList[6] = "select student.sno, sname, grade from student, sc where student.sno = sc.sno";
        // 聚合查询
        sqlList[7] = "select count(*) from students";
        sqlList[8] = "select count(*) from student group by sage";
        // 排序查询
        sqlList[9] = "select sname, sage from student order by sage";

        expectList[0] = setupTest0();
        expectList[1] = setupTest1();
        expectList[2] = setupTest2();
        expectList[3] = setupTest3();
        expectList[4] = setupTest4();
        expectList[5] = setupTest5();
        expectList[6] = setupTest6();
        expectList[7] = setupTest7();
        expectList[8] = setupTest8();
        expectList[9] = setupTest9();
    }

    static SRA_t *parse(const char *sql) {
        auto *tokenizer = new Tokenizer(sql);
        auto *sp = new SelectParser(tokenizer);
        auto *sra = sp->parse_sql_stmt_select();
        return sra;
    }

    char *sqlList[10]{};
    SRA_t *expectList[10]{};

private:
    static SRA_t *setupTest0();

    static SRA_t *setupTest1();

    static SRA_t *setupTest2();

    static SRA_t *setupTest3();

    static SRA_t *setupTest4();

    static SRA_t *setupTest5();

    static SRA_t *setupTest6();

    static SRA_t *setupTest7();

    static SRA_t *setupTest8();

    static SRA_t *setupTest9();

};

TEST_F(Exp_01_03_SelectTest, Correct) {

    try {
        SRA_t *resultList[10];

        resultList[0] = parse(sqlList[0]);
        resultList[1] = parse(sqlList[1]);
        resultList[2] = parse(sqlList[2]);
        resultList[3] = parse(sqlList[3]);
        resultList[4] = parse(sqlList[4]);
        resultList[5] = parse(sqlList[5]);
        resultList[6] = parse(sqlList[6]);
        resultList[7] = parse(sqlList[7]);
        resultList[8] = parse(sqlList[8]);
        resultList[9] = parse(sqlList[9]);

        EXPECT_TRUE(equal_sra(resultList[0], expectList[0]));
        EXPECT_TRUE(equal_sra(resultList[1], expectList[1]));
        EXPECT_TRUE(equal_sra(resultList[2], expectList[2]));
        EXPECT_TRUE(equal_sra(resultList[3], expectList[3]));
        EXPECT_TRUE(equal_sra(resultList[4], expectList[4]));
        EXPECT_TRUE(equal_sra(resultList[5], expectList[5]));
        EXPECT_TRUE(equal_sra(resultList[6], expectList[6]));
        EXPECT_TRUE(equal_sra(resultList[7], expectList[7]));
        EXPECT_TRUE(equal_sra(resultList[8], expectList[8]));
        EXPECT_TRUE(equal_sra(resultList[9], expectList[9]));

    } catch (const exception &e) {
        cout << e.what() << endl;
    }
}


// region 测试用例

SRA_t *Exp_01_03_SelectTest::setupTest0() {
    // select * from student
    TableReference_t *ref = TableReference_make("student", nullptr);
    vector<Expression *> fieldsExpr{
            (new Parser(new Tokenizer("*")))->parseExpressionRD()
    };

    SRA_t *project = SRAProject(
            SRATable(ref),
            fieldsExpr
    );

    return project;
}

SRA_t *Exp_01_03_SelectTest::setupTest1() {
    // select student.* from student
    TableReference_t *ref = TableReference_make("student", nullptr);
    vector<Expression *> fieldsExpr{
            (new Parser(new Tokenizer("student.*")))->parseExpressionRD()
    };

    SRA_t *project = SRAProject(
            SRATable(ref),
            fieldsExpr
    );

    return project;
}

SRA_t *Exp_01_03_SelectTest::setupTest2() {
    // select sno from student
    TableReference_t *ref = TableReference_make("student", nullptr);
    vector<Expression *> fieldsExpr{
            (new Parser(new Tokenizer("sno")))->parseExpressionRD()
    };

    SRA_t *project = SRAProject(
            SRATable(ref),
            fieldsExpr
    );

    return project;
}

SRA_t *Exp_01_03_SelectTest::setupTest3() {
    // select sno, sname from student
    TableReference_t *ref = TableReference_make("student", nullptr);
    vector<Expression *> fieldsExpr{
            (new Parser(new Tokenizer("sno")))->parseExpressionRD(),
            (new Parser(new Tokenizer("sname")))->parseExpressionRD()
    };

    SRA_t *project = SRAProject(
            SRATable(ref),
            fieldsExpr
    );

    return project;
}

SRA_t *Exp_01_03_SelectTest::setupTest4() {
    // select sno from student where sage < 25
    TableReference_t *ref = TableReference_make("student", nullptr);
    vector<Expression *> fieldsExpr{
            (new Parser(new Tokenizer("sno")))->parseExpressionRD()
    };

    Expression *whereExpr{
            (new Parser(new Tokenizer("sage < 25")))->parseExpressionRD()
    };

    SRA_t *project = SRAProject(
            SRASelect(
                    SRATable(ref),
                    whereExpr
            ),
            fieldsExpr
    );

    return project;
}

SRA_t *Exp_01_03_SelectTest::setupTest5() {
    // select sname, grade from student, sc where student.sno = sc.sno
    TableReference_t *ref1 = TableReference_make("student", nullptr);
    TableReference_t *ref2 = TableReference_make("sc", nullptr);
    vector<Expression *> fieldsExpr{
            (new Parser(new Tokenizer("sname")))->parseExpressionRD(),
            (new Parser(new Tokenizer("grade")))->parseExpressionRD()
    };

    Expression *whereExpr{
            (new Parser(new Tokenizer("student.sno = sc.sno")))->parseExpressionRD()
    };

    SRA_t *project = SRAProject(
            SRASelect(
                    SRAJoin(
                            SRATable(ref1),
                            SRATable(ref2),
                            nullptr
                    ),
                    whereExpr
            ),
            fieldsExpr
    );

    return project;
}

SRA_t *Exp_01_03_SelectTest::setupTest6() {
    // select student.sno, sname, grade from student sc where student.sno = sc.sno
    TableReference_t *ref1 = TableReference_make("student", nullptr);
    TableReference_t *ref2 = TableReference_make("sc", nullptr);
    vector<Expression *> fieldsExpr{
            (new Parser(new Tokenizer("student.sno")))->parseExpressionRD(),
            (new Parser(new Tokenizer("sname")))->parseExpressionRD(),
            (new Parser(new Tokenizer("grade")))->parseExpressionRD()
    };

    Expression *whereExpr{
            (new Parser(new Tokenizer("student.sno = sc.sno")))->parseExpressionRD()
    };

    SRA_t *project = SRAProject(
            SRASelect(
                    SRAJoin(
                            SRATable(ref1),
                            SRATable(ref2),
                            nullptr
                    ),
                    whereExpr
            ),
            fieldsExpr
    );

    return project;
}

SRA_t *Exp_01_03_SelectTest::setupTest7() {
    // select count(*) from students
    TableReference_t *ref1 = TableReference_make("student", nullptr);
    vector<Expression *> fieldsExpr{
            (new Parser(new Tokenizer("count(*)")))->parseExpressionRD()
    };

    SRA_t *project = SRAProject(
            SRATable(ref1),
            fieldsExpr
    );

    return project;
}

SRA_t *Exp_01_03_SelectTest::setupTest8() {
    // select count(*) from student group by sage
    TableReference_t *ref1 = TableReference_make("student", nullptr);
    vector<Expression *> fieldsExpr{
            (new Parser(new Tokenizer("count(*)")))->parseExpressionRD()
    };
    vector<Expression *> groupExpr{
            (new Parser(new Tokenizer("sage")))->parseExpressionRD()
    };

    SRA_t *project = SRAProject(
            SRATable(ref1),
            fieldsExpr
    );
    project->project.group_by.assign(groupExpr.begin(), groupExpr.end());

    return project;
}

SRA_t *Exp_01_03_SelectTest::setupTest9() {
    // select sname, sage from student order by sage
    TableReference_t *ref1 = TableReference_make("student", nullptr);
    vector<Expression *> fieldsExpr{
            (new Parser(new Tokenizer("sname")))->parseExpressionRD(),
            (new Parser(new Tokenizer("sage")))->parseExpressionRD()
    };
    vector<Expression *> orderExpr{
            (new Parser(new Tokenizer("sage")))->parseExpressionRD()
    };

    SRA_t *project = SRAProject(
            SRATable(ref1),
            fieldsExpr
    );
    project->project.order_by.assign(orderExpr.begin(), orderExpr.end());

    return project;
}

// endregion
