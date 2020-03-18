//
// Created by maokw on 2020-03-02.
//

#include <cstdio>
#include <gtest/gtest.h>
#include <parser/StatementParser.h>
#include <parser/Tokenizer.h>
#include <test/test_stmt_parser.h>

class InsertParserTest : public testing::Test {
protected:
    vector<string> sql{"insert into student(sname, sage) values ('John', 17)",
                       "insert into student(sname, sage) values ('John', 17), ('Terry', 19)"};
    vector<sql_stmt_insert> expects{};
    vector<sql_stmt_insert> results{};

    void SetUp() override {
        setupTest0();
        setupTest1();
    }

private:
    void setupTest0();

    void setupTest1();

    static sql_stmt_insert parser(const string &sql);
};

void InsertParserTest::setupTest0() {
    char *tableName{"student"};
    vector<char *> fields{"sname", "sage", "sgender"};
    vector<variant *> values;

    auto *v = new variant();
    v->type = DATA_TYPE_CHAR;
    v->strValue = "John";
    values.push_back(v);
    v = new variant();
    v->type = DATA_TYPE_INT;
    v->intValue = 17;
    values.push_back(v);

    auto x = sql_stmt_insert();
    x.tableName = tableName;
    x.fields = fields;
    x.values = values;
    expects.push_back(x);

    auto y = parser(sql[0]);
    results.push_back(y);

}

void InsertParserTest::setupTest1() {
    char *tableName{"student"};
    vector<char *> fields{"sname", "sage", "sgender"};
    vector<variant *> values;

    auto *v = new variant();
    v->type = DATA_TYPE_CHAR;
    v->strValue = "John";
    values.push_back(v);
    v = new variant();
    v->type = DATA_TYPE_INT;
    v->intValue = 17;
    values.push_back(v);
    v = new variant();
    v->type = DATA_TYPE_CHAR;
    v->strValue = "Terry";
    values.push_back(v);
    v = new variant();
    v->type = DATA_TYPE_INT;
    v->intValue = 19;
    values.push_back(v);

    auto x = sql_stmt_insert();
    x.tableName = tableName;
    x.fields = fields;
    x.values = values;
    expects.push_back(x);

    auto y = parser(sql[0]);
    results.push_back(y);
}

sql_stmt_insert InsertParserTest::parser(const string &sql) {
    auto *tokenizer = new Tokenizer(sql.c_str());
    auto *ip = new InsertParser(tokenizer);
    auto *sqlStmtInsert = ip->parse_sql_stmt_insert();
    return *sqlStmtInsert;
}

TEST_F(InsertParserTest, Test_01_TableName) {
    EXPECT_TRUE(equal_table_name(expects[0], results[0]));
}

TEST_F(InsertParserTest, Test_02_Fields) {
    EXPECT_TRUE(equal_fields(expects[0], results[0]));
}

TEST_F(InsertParserTest, Test_03_Values) {
    EXPECT_TRUE(equal_values(expects[0], results[0]));
}

TEST_F(InsertParserTest, Test_04_Full) {
    EXPECT_TRUE(equal(expects[0], results[0]));
}

TEST_F(InsertParserTest, Test_05_TableName) {
    EXPECT_TRUE(equal_table_name(expects[1], results[1]));
}

TEST_F(InsertParserTest, Test_06_Fields) {
    EXPECT_TRUE(equal_fields(expects[1], results[1]));
}

TEST_F(InsertParserTest, Test_07_Values) {
    EXPECT_TRUE(equal_values(expects[1], results[1]));
}

TEST_F(InsertParserTest, Test_08_Full) {
    EXPECT_TRUE(equal(expects[1], results[1]));
}
