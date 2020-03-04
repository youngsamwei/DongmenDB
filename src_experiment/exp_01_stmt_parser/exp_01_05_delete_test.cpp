//
// Created by Sam on 2018/2/11.
//

#include <cstdio>
#include <gtest/gtest.h>
#include <map>
#include <parser/StatementParser.h>
#include <parser/Tokenizer.h>
#include <test/test_stmt_parser.h>

class Exp_01_05_DeleteTest : public testing::Test {
protected:
    void SetUp() override {
        sqlList[0] = "delete student where sname = 'tom simith'";;

        expectList[0] = setupTest0();
    }

    static sql_stmt_delete parse(const char *sql) {
        auto *tokenizer = new Tokenizer(sql);
        auto *ip = new DeleteParser(tokenizer);
        auto *stmt = ip->parse_sql_stmt_delete();
        return *stmt;
    }

    char *sqlList[10]{};
    sql_stmt_delete expectList[10]{};

private:
    static sql_stmt_delete setupTest0();
};

TEST_F(Exp_01_05_DeleteTest, Correct) {

    try {
        sql_stmt_delete resultList[10];

        resultList[0] = parse(sqlList[0]);
        EXPECT_TRUE(equal_table_name(resultList[0], expectList[0]));
        EXPECT_TRUE(equal_where(resultList[0], expectList[0]));
        EXPECT_TRUE(equal(resultList[0], expectList[0]));

    } catch (const exception &e) {
        cout << e.what() << endl;
    }
}

// region 测试用例

sql_stmt_delete Exp_01_05_DeleteTest::setupTest0() {
    char *tableName{"student"};
    vector<char *> fields{"sname"};
    vector<Expression *> fieldsExpr{
            (new Parser(new Tokenizer("'Tom Cruise'")))->parseExpressionRD()
    };
    SRA_t *where = nullptr;

    auto stmt = sql_stmt_delete();
    stmt.tableName = tableName;
    stmt.where = where;

    return stmt;
}

// endregion
