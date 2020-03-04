//
// Created by Sam on 2018/2/11.
//

#include <cstdio>
#include <gtest/gtest.h>
#include <map>
#include <parser/StatementParser.h>
#include <parser/Tokenizer.h>
#include <test/test_stmt_parser.h>

class Exp_01_04_UpdateTest : public testing::Test {
protected:
    void SetUp() override {
        // 一般更新
        sqlList[0] = "update student set sage = sage + 1";
        // 带 where 的更新
        sqlList[1] = "update student set sname = 'Tom Cruise' where sno = '2012010101'";
        // 多字段
        sqlList[2] = "update student set sname = 'li simith', ssex= 'male' where sname = 'zhang simith'";

        expectList[0] = setupTest0();
        expectList[1] = setupTest1();
        expectList[2] = setupTest2();
    }

    static sql_stmt_update parse(const char *sql) {
        auto *tokenizer = new Tokenizer(sql);
        auto *ip = new UpdateParser(tokenizer);
        auto *sqlStmtUpdate = ip->parse_sql_stmt_update();
        return *sqlStmtUpdate;
    }

    char *sqlList[10]{};
    sql_stmt_update expectList[10]{};

private:
    static sql_stmt_update setupTest0();

    static sql_stmt_update setupTest1();

    static sql_stmt_update setupTest2();
};

TEST_F(Exp_01_04_UpdateTest, Correct) {

    try {
        sql_stmt_update resultList[2];

        resultList[0] = parse(sqlList[0]);
        EXPECT_TRUE(equal_table_name(resultList[0], expectList[0]));
        EXPECT_TRUE(equal_where(resultList[0], expectList[0]));
        EXPECT_TRUE(equal_fields_expr(resultList[0], expectList[0]));
        EXPECT_TRUE(equal_fields(resultList[0], expectList[0]));
        EXPECT_TRUE(equal(resultList[0], expectList[0]));

        resultList[1] = parse(sqlList[1]);
        EXPECT_TRUE(equal_table_name(resultList[1], expectList[1]));
        EXPECT_TRUE(equal_where(resultList[1], expectList[1]));
        EXPECT_TRUE(equal_fields_expr(resultList[1], expectList[1]));
        EXPECT_TRUE(equal_fields(resultList[1], expectList[1]));
        EXPECT_TRUE(equal(resultList[1], expectList[1]));

        resultList[2] = parse(sqlList[2]);
        EXPECT_TRUE(equal_table_name(resultList[2], expectList[2]));
        EXPECT_TRUE(equal_where(resultList[2], expectList[2]));
        EXPECT_TRUE(equal_fields_expr(resultList[2], expectList[2]));
        EXPECT_TRUE(equal_fields(resultList[2], expectList[2]));
        EXPECT_TRUE(equal(resultList[2], expectList[2]));

    } catch (const exception &e) {
        cout << e.what() << endl;
    }
}

// region 测试用例

sql_stmt_update Exp_01_04_UpdateTest::setupTest0() {
    char *tableName{"student"};
    vector<char *> fields{"sname"};
    vector<Expression *> fieldsExpr{
            (new Parser(new Tokenizer("'Tom Cruise'")))->parseExpressionRD()
    };
    SRA_t *where = nullptr;

    auto stmt = sql_stmt_update();
    stmt.tableName = tableName;
    stmt.fields = fields;
    stmt.fieldsExpr = fieldsExpr;
    stmt.where = where;

    return stmt;
}

sql_stmt_update Exp_01_04_UpdateTest::setupTest1() {
    char *tableName{"student"};
    vector<char *> fields{"sage"};
    vector<Expression *> fieldsExpr{
            (new Parser(new Tokenizer("Tom Cruise")))->parseExpressionRD()
    };
    Expression *whereExpr = (new Parser(new Tokenizer("sno = '2012010101'")))->parseExpressionRD();
    SRA_t *table = SRATable(TableReference_make(tableName, nullptr));
    SRA_t *where = SRASelect(table, whereExpr);

    auto stmt = sql_stmt_update();
    stmt.tableName = tableName;
    stmt.fields = fields;
    stmt.fieldsExpr = fieldsExpr;
    stmt.where = where;

    return stmt;
}

sql_stmt_update Exp_01_04_UpdateTest::setupTest2() {
    char *tableName{"student"};
    vector<char *> fields{"sname", "ssex"};
    vector<Expression *> fieldsExpr{
            (new Parser(new Tokenizer("li simith")))->parseExpressionRD(),
            (new Parser(new Tokenizer("male")))->parseExpressionRD()
    };
    Expression *whereExpr = (new Parser(new Tokenizer("sname = 'zhang simith'")))->parseExpressionRD();
    SRA_t *table = SRATable(TableReference_make(tableName, nullptr));
    SRA_t *where = SRASelect(table, whereExpr);

    auto stmt = sql_stmt_update();
    stmt.tableName = tableName;
    stmt.fields = fields;
    stmt.fieldsExpr = fieldsExpr;
    stmt.where = where;

    return stmt;
}

// endregion