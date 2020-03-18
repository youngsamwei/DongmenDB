//
// Created by Sam on 2018/2/11.
//

#include <gtest/gtest.h>
#include <parser/StatementParser.h>
#include <parser/Tokenizer.h>
#include <test/test_stmt_parser.h>

sql_stmt_update &&parse(const string &sql) {
    auto *tokenizer = new Tokenizer(sql.c_str());
    auto *ip = new UpdateParser(tokenizer);
    auto *sqlStmtUpdate = ip->parse_sql_stmt_update();
    if (sqlStmtUpdate == nullptr) {
        cout << ip->parserMessage << endl;
        throw std::runtime_error("parser failed");
    }
    return std::move(*sqlStmtUpdate);
}

class UpdateParserTest_01_Normal : public testing::Test {
protected:
    void SetUp() override {
        char *tableName{"student"};
        vector<char *> fields{"sage"};
        vector<Expression *> fieldsExpr{
                (new Parser(new Tokenizer("sage + 1")))->parseExpressionRD()
        };
        SRA_t *table = SRATable(TableReference_make(tableName, nullptr));

        expect.tableName = tableName;
        expect.fields = fields;
        expect.fieldsExpr = fieldsExpr;
        expect.where = table;

        actual = parse(sql);
    }

    const string sql = "update student set sage = sage + 1";
    sql_stmt_update expect{};
    sql_stmt_update actual{};
};

TEST_F(UpdateParserTest_01_Normal, TableName) {
    EXPECT_TRUE(equal_table_name(expect, actual));
}

TEST_F(UpdateParserTest_01_Normal, Where) {
    EXPECT_TRUE(equal_where(expect, actual));
}

TEST_F(UpdateParserTest_01_Normal, FieldsExpr) {
    EXPECT_TRUE(equal_fields_expr(expect, actual));
}

TEST_F(UpdateParserTest_01_Normal, Fields) {
    EXPECT_TRUE(equal_fields(expect, actual));
}

TEST_F(UpdateParserTest_01_Normal, Full) {
    EXPECT_TRUE(equal(expect, actual))
                        << "Actual:" << endl
                        << actual << endl
                        << "Expected:" << endl
                        << expect;
}

class UpdateParserTest_02_Where : public testing::Test {
protected:
    void SetUp() override {
        char *tableName{"student"};
        vector<char *> fields{"sname"};
        vector<Expression *> fieldsExpr{
                (new Parser(new Tokenizer("Tom Cruise")))->parseExpressionRD()};
        Expression *whereExpr =
                (new Parser(new Tokenizer("sno = '2012010101'")))->parseExpressionRD();
        SRA_t *table = SRATable(TableReference_make(tableName, nullptr));
        SRA_t *where = SRASelect(table, whereExpr);

        expect.tableName = tableName;
        expect.fields = fields;
        expect.fieldsExpr = fieldsExpr;
        expect.where = where;

        actual = parse(sql);
    }

    const string sql = "update student set sname = 'Tom Cruise' where sno = '2012010101'";
    sql_stmt_update expect{};
    sql_stmt_update actual{};
};

TEST_F(UpdateParserTest_02_Where, TableName) {
    EXPECT_TRUE(equal_table_name(expect, actual));
}

TEST_F(UpdateParserTest_02_Where, Where) {
    EXPECT_TRUE(equal_where(expect, actual));
}

TEST_F(UpdateParserTest_02_Where, FieldsExpr) {
    EXPECT_TRUE(equal_fields_expr(expect, actual));
}

TEST_F(UpdateParserTest_02_Where, Fields) {
    EXPECT_TRUE(equal_fields(expect, actual));
}

TEST_F(UpdateParserTest_02_Where, Full) {
    EXPECT_TRUE(equal(expect, actual))
                        << "Actual:" << endl
                        << actual << endl
                        << "Expected:" << endl
                        << expect;
}

class UpdateParserTest_03_Fields : public testing::Test {
protected:
    void SetUp() override {
        char *tableName{"student"};
        vector<char *> fields{"sname", "ssex"};
        vector<Expression *> fieldsExpr{
                (new Parser(new Tokenizer("li simith")))->parseExpressionRD(),
                (new Parser(new Tokenizer("male")))->parseExpressionRD()};
        Expression *whereExpr = (new Parser(new Tokenizer("sname = 'zhang simith'")))
                ->parseExpressionRD();
        SRA_t *table = SRATable(TableReference_make(tableName, nullptr));
        SRA_t *where = SRASelect(table, whereExpr);

        expect.tableName = tableName;
        expect.fields = fields;
        expect.fieldsExpr = fieldsExpr;
        expect.where = where;

        actual = parse(sql);
    }

    const string sql = "update student set sname = 'li simith', ssex = 'male' where sname = 'zhang simith'";
    sql_stmt_update expect{};
    sql_stmt_update actual{};
};

TEST_F(UpdateParserTest_03_Fields, TableName) {
    EXPECT_TRUE(equal_table_name(expect, actual));
}

TEST_F(UpdateParserTest_03_Fields, Where) {
    EXPECT_TRUE(equal_where(expect, actual));
}

TEST_F(UpdateParserTest_03_Fields, FieldsExpr) {
    EXPECT_TRUE(equal_fields_expr(expect, actual));
}

TEST_F(UpdateParserTest_03_Fields, Fields) {
    EXPECT_TRUE(equal_fields(expect, actual));
}

TEST_F(UpdateParserTest_03_Fields, Full) {
    EXPECT_TRUE(equal(expect, actual))
                        << "Actual:" << endl
                        << actual << endl
                        << "Expected:" << endl
                        << expect;
}
