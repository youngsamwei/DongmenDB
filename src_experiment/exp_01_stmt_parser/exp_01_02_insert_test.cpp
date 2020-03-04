//
// Created by maokw on 2020-03-02.
//

#include <cstdio>
#include <gtest/gtest.h>
#include <map>
#include <parser/StatementParser.h>
#include <parser/Tokenizer.h>
#include <test/test_stmt_parser.h>

class Exp_01_02_InsertTest : public testing::Test {
protected:
    void SetUp() override {
        sqlList[0] = "insert into student(sname, sage) values ('John', 17)";
        sqlList[1] = "insert into student(sname, sage) values ('John', 17), ('Terry', 19)";
        expectList[0] = setupTest0();
        expectList[1] = setupTest1();
    }

    static sql_stmt_insert parse(const char *sql) {
        auto *tokenizer = new Tokenizer(sql);
        auto *ip = new InsertParser(tokenizer);
        auto *sqlStmtInsert = ip->parse_sql_stmt_insert();
        return *sqlStmtInsert;
    }

    char *sqlList[10]{};
    sql_stmt_insert expectList[10]{};

private:
    static sql_stmt_insert setupTest0();

    static sql_stmt_insert setupTest1();
};

TEST_F(Exp_01_02_InsertTest, Correct) {

    try {
        sql_stmt_insert resultList[2];

        resultList[0] = parse(sqlList[0]);
        EXPECT_TRUE(equal_table_name(resultList[0], expectList[0]));
        EXPECT_TRUE(equal_fields(resultList[0], expectList[0]));
        EXPECT_TRUE(equal_values(resultList[0], expectList[0]));
        EXPECT_TRUE(equal(resultList[0], expectList[0]));


        resultList[1] = parse(sqlList[1]);
        EXPECT_TRUE(equal_table_name(resultList[1], expectList[1]));
        EXPECT_TRUE(equal_fields(resultList[1], expectList[1]));
        EXPECT_TRUE(equal_values(resultList[1], expectList[1]));
        EXPECT_TRUE(equal(resultList[1], expectList[1]));

    } catch (const exception &e) {
        cout << e.what() << endl;
    }

}

// region 测试用例

sql_stmt_insert Exp_01_02_InsertTest::setupTest0() {
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

    auto sqlStmtInsert = sql_stmt_insert();
    sqlStmtInsert.tableName = tableName;
    sqlStmtInsert.fields = fields;
    sqlStmtInsert.values = values;

    return sqlStmtInsert;
}

sql_stmt_insert Exp_01_02_InsertTest::setupTest1() {
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

    auto sqlStmtInsert = sql_stmt_insert();
    sqlStmtInsert.tableName = tableName;
    sqlStmtInsert.fields = fields;
    sqlStmtInsert.values = values;

    return sqlStmtInsert;
}

// endregion
