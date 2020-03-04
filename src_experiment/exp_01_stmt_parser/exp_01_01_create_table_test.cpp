//
// Created by Sam on 2018/2/11.
//

#include <cstdio>
#include <gtest/gtest.h>
#include <map>
#include <parser/StatementParser.h>
#include <parser/Tokenizer.h>
#include <test/test_stmt_parser.h>

class Exp_01_01_CreateTableTest : public testing::Test {
protected:
    void SetUp() override {
        sqlList[0] = "create table typeTest(colChar char(10), colInt integer);";
        expectList[0] = setupTest0();
    }

    const char *sqlList[10]{};
    sql_stmt_create expectList[10]{};

    static sql_stmt_create parse(const char *sql) {
        auto *tokenizer = new Tokenizer(sql);
        auto *cp = new CreateParser(tokenizer);
        auto *sqlStmtCreate = cp->parse_sql_stmt_create();
        return *sqlStmtCreate;
    }

private:
    sql_stmt_create setupTest0();
};

TEST_F(Exp_01_01_CreateTableTest, Correct) {

    try {
        sql_stmt_create resultList[10];

        resultList[0] = parse(sqlList[0]);

        // 表名
        EXPECT_TRUE(equal_table_name(resultList[0], expectList[0]));
        // 字段名
        EXPECT_TRUE(equal_fields_name(resultList[0], expectList[0]));
        // 字段
        EXPECT_TRUE(equal_fields(resultList[0], expectList[0]));
        // 表信息
        EXPECT_TRUE(equal_table_info(resultList[0], expectList[0]));
        // 判断
        EXPECT_TRUE(equal(resultList[0], expectList[0]));

    } catch (const exception &e) {
        cout << e.what() << endl;
    }
}

// region 测试用例

sql_stmt_create Exp_01_01_CreateTableTest::setupTest0() {
    /* char *sql = "create table typeTest(colChar char(10), colInt integer, colBoolean boolean, colDouble double, colText text);"; */

    const char *tableName = "typeTest";
    vector<char *> fieldsName{"colChar", "colInt" /* , "colBoolean", "colDouble", "colText" */ };
    auto *columns = new map<string, FieldInfo *>();
    columns->insert(pair<string, FieldInfo *>("colChar", new FieldInfo(DATA_TYPE_CHAR, 10, "colChar")));
    columns->insert(pair<string, FieldInfo *>("colInt", new FieldInfo(DATA_TYPE_INT, INT_SIZE, "colInt")));
    /* NOT IMPLEMENT YET
    columns->insert(pair<string, FieldInfo *>("colBoolean", new Fi  eldInfo(DATA_TYPE_BOOLEAN, BOOLEAN_SIZE, "colBoolean")));
    columns->insert(pair<string, FieldInfo *>("colDouble", new FieldInfo(DATA_TYPE_DOUBLE, DOUBLE_SIZE, "colDouble")));
    columns->insert(pair<string, FieldInfo *>("colText", new FieldInfo(DATA_TYPE_TEXT, TEXT_SIZE, "colText")));
     */
    sql_stmt_create sqlStmtCreate{};
    sqlStmtCreate.tableInfo = new TableInfo(tableName, fieldsName, columns);
    sqlStmtCreate.constraints = nullptr;

    return sqlStmtCreate;
}

// endregion
