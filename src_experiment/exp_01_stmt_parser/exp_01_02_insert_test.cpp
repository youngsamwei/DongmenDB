//
// Created by maokw on 2020-03-02.
//

#include <cstdio>
#include <gtest/gtest.h>
#include <parser/StatementParser.h>
#include <parser/Tokenizer.h>
#include <test/test_stmt_parser.h>

sql_stmt_insert &&parse(const string &sql) {
    auto *tokenizer = new Tokenizer(sql.c_str());
    auto *ip = new InsertParser(tokenizer);
    auto *sqlStmtInsert = ip->parse_sql_stmt_insert();
    return std::move(*sqlStmtInsert);
}

class DeleteParserTest_01_Normal : public testing::Test {
protected:
    void SetUp() override {
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
        expect = x;

        actual = parse(sql);
    }

    const string sql{"insert into student(sname, sage) values ('John', 17)"};
    sql_stmt_insert expect{};
    sql_stmt_insert actual{};
};

TEST_F(DeleteParserTest_01_Normal, TableName) {
    EXPECT_TRUE(equal_table_name(expect, actual));
}

TEST_F(DeleteParserTest_01_Normal, Fields) {
    EXPECT_TRUE(equal_fields(expect, actual));
}

TEST_F(DeleteParserTest_01_Normal, Values) {
    EXPECT_TRUE(equal_values(expect, actual));
}

TEST_F(DeleteParserTest_01_Normal, Full) {
    EXPECT_TRUE(equal(expect, actual))
                        << "Actual:" << endl
                        << actual << endl
                        << "Expected:" << endl
                        << expect;
}


class DeleteParserTest_02_MultiRows : public testing::Test {
protected:
    void SetUp() override {
        char *tableName{"student"};
        vector<char *> fields{"sname", "sage"};
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
        expect = x;

        actual = parse(sql);
    }

    const string sql{"insert into student(sname, sage) values ('John', 17), ('Terry', 19)"};
    sql_stmt_insert expect{};
    sql_stmt_insert actual{};
};

TEST_F(DeleteParserTest_02_MultiRows, TableName) {
    EXPECT_TRUE(equal_table_name(expect, actual));
}

TEST_F(DeleteParserTest_02_MultiRows, Fields) {
    EXPECT_TRUE(equal_fields(expect, actual));
}

TEST_F(DeleteParserTest_02_MultiRows, Values) {
    EXPECT_TRUE(equal_values(expect, actual));
}

TEST_F(DeleteParserTest_02_MultiRows, Full) {
    EXPECT_TRUE(equal(expect, actual))
                        << "Actual:" << endl
                        << actual << endl
                        << "Expected:" << endl
                        << expect;
}
