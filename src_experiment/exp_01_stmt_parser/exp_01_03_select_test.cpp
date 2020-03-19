//
// Created by Sam on 2018/2/11.
//

#include <gtest/gtest.h>
#include <parser/StatementParser.h>
#include <parser/Tokenizer.h>
#include <test/test_stmt_parser.h>

SRA_t *parse(const string &sql) {
    auto *tokenizer = new Tokenizer(sql.c_str());
    auto *sp = new SelectParser(tokenizer);
    auto *sra = sp->parse_sql_stmt_select();
    return sra;
}

TEST(SelectParserTest, Test_01_Base) {
    string sql = "select * from student";

    TableReference_t *ref = TableReference_make("student", nullptr);
    vector<Expression *> fieldsExpr{
            (new Parser(new Tokenizer("*")))->parseExpressionRD()
    };
    SRA_t *expect = SRAProject(
            SRATable(ref),
            fieldsExpr
    );

    SRA_t *result = parse(sql);
    EXPECT_TRUE(equal_sra(expect, result))
                        << "Expected:" << endl
                        << expect << endl
                        << "Actual:" << endl
                        << result;
}

TEST(SelectParserTest, Test_02_Project) {
    string sql = "select sno from student";

    TableReference_t *ref = TableReference_make("student", nullptr);
    vector<Expression *> fieldsExpr{
            (new Parser(new Tokenizer("student.*")))->parseExpressionRD()
    };
    SRA_t *expect = SRAProject(
            SRATable(ref),
            fieldsExpr
    );

    SRA_t *result = parse(sql);
    EXPECT_TRUE(equal_sra(expect, result))
                        << "Expected:" << endl
                        << expect << endl
                        << "Actual:" << endl
                        << result;
}

TEST(SelectParserTest, Test_03_Project) {
    string sql = "select sno from student";

    TableReference_t *ref = TableReference_make("student", nullptr);
    vector<Expression *> fieldsExpr{
            (new Parser(new Tokenizer("sno")))->parseExpressionRD()
    };
    SRA_t *expect = SRAProject(
            SRATable(ref),
            fieldsExpr
    );

    SRA_t *result = parse(sql);
    EXPECT_TRUE(equal_sra(expect, result))
                        << "Expected:" << endl
                        << expect << endl
                        << "Actual:" << endl
                        << result;
}

TEST(SelectParserTest, Test_04_Project) {
    string sql = "select sno, sname from student";

    TableReference_t *ref = TableReference_make("student", nullptr);
    vector<Expression *> fieldsExpr{
            (new Parser(new Tokenizer("sno")))->parseExpressionRD(),
            (new Parser(new Tokenizer("sname")))->parseExpressionRD()
    };
    SRA_t *expect = SRAProject(
            SRATable(ref),
            fieldsExpr
    );

    SRA_t *result = parse(sql);
    EXPECT_TRUE(equal_sra(expect, result))
                        << "Expected:" << endl
                        << expect << endl
                        << "Actual:" << endl
                        << result;
}

TEST(SelectParserTest, Test_05_Expression) {
    string sql = "select sno from student where sage < 25";

    TableReference_t *ref = TableReference_make("student", nullptr);
    vector<Expression *> fieldsExpr{
            (new Parser(new Tokenizer("sno")))->parseExpressionRD()
    };
    Expression *whereExpr{
            (new Parser(new Tokenizer("sage < 25")))->parseExpressionRD()
    };
    SRA_t *expect = SRAProject(
            SRASelect(SRATable(ref), whereExpr),
            fieldsExpr
    );

    SRA_t *result = parse(sql);
    EXPECT_TRUE(equal_sra(expect, result))
                        << "Expected:" << endl
                        << expect << endl
                        << "Actual:" << endl
                        << result;
}

TEST(SelectParserTest, Test_06_MultiTable) {
    string sql = "select sname, grade from student, sc where student.sno = sc.sno";

    TableReference_t *ref1 = TableReference_make("student", nullptr);
    TableReference_t *ref2 = TableReference_make("sc", nullptr);
    vector<Expression *> fieldsExpr{
            (new Parser(new Tokenizer("sname")))->parseExpressionRD(),
            (new Parser(new Tokenizer("grade")))->parseExpressionRD()
    };
    Expression *whereExpr{
            (new Parser(new Tokenizer("student.sno = sc.sno")))->parseExpressionRD()
    };
    SRA_t *expect = SRAProject(
            SRASelect(
                    SRAJoin(SRATable(ref1), SRATable(ref2), nullptr),
                    whereExpr
            ),
            fieldsExpr
    );

    SRA_t *result = parse(sql);
    EXPECT_TRUE(equal_sra(expect, result))
                        << "Expected:" << endl
                        << expect << endl
                        << "Actual:" << endl
                        << result;
}

/*

TEST(SelectParserTest, Test_07_MultiTable) {
    string sql = "select student.sno, sname, grade from student, sc where student.sno = sc.sno";

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
    SRA_t *expect = SRAProject(
            SRASelect(
                    SRAJoin(SRATable(ref1), SRATable(ref2), nullptr),
                    whereExpr
            ),
            fieldsExpr
    );

    SRA_t *result = parse(sql);
    EXPECT_TRUE(equal_sra(expect, result))
                        << "Expected:" << endl
                        << expect << endl
                        << "Actual:" << endl
                        << result;
}

TEST(SelectParserTest, Test_08_Group) {
    string sql = "select count(*) from students";

    TableReference_t *ref1 = TableReference_make("student", nullptr);
    vector<Expression *> fieldsExpr{
            (new Parser(new Tokenizer("count(*)")))->parseExpressionRD()
    };
    SRA_t *expect = SRAProject(
            SRATable(ref1),
            fieldsExpr
    );

    SRA_t *result = parse(sql);
    EXPECT_TRUE(equal_sra(expect, result))
                        << "Expected:" << endl
                        << expect << endl
                        << "Actual:" << endl
                        << result;
}

TEST(SelectParserTest, Test_09_Group) {
    string sql = "select count(*) from student group by sage";

    TableReference_t *ref1 = TableReference_make("student", nullptr);
    vector<Expression *> fieldsExpr{
            (new Parser(new Tokenizer("count(*)")))->parseExpressionRD()
    };
    vector<Expression *> groupExpr{
            (new Parser(new Tokenizer("sage")))->parseExpressionRD()
    };

    SRA_t *expect = SRAProject(
            SRATable(ref1),
            fieldsExpr
    );
    expect->project.group_by.assign(groupExpr.begin(), groupExpr.end());

    SRA_t *result = parse(sql);
    EXPECT_TRUE(equal_sra(expect, result))
                        << "Expected:" << endl
                        << expect << endl
                        << "Actual:" << endl
                        << result;
}

*/

TEST(SelectParserTest, Test_10_Sort) {
    string sql = "select sname, sage from student order by sage";

    TableReference_t *ref1 = TableReference_make("student", nullptr);
    vector<Expression *> fieldsExpr{
            (new Parser(new Tokenizer("sname")))->parseExpressionRD(),
            (new Parser(new Tokenizer("sage")))->parseExpressionRD()
    };
    vector<Expression *> orderExpr{
            (new Parser(new Tokenizer("sage")))->parseExpressionRD()
    };
    SRA_t *expect = SRAProject(
            SRATable(ref1),
            fieldsExpr
    );
    expect->project.order_by.assign(orderExpr.begin(), orderExpr.end());

    SRA_t *result = parse(sql);
    EXPECT_TRUE(equal_sra(expect, result))
                        << "Expected:" << endl
                        << expect << endl
                        << "Actual:" << endl
                        << result;
}
