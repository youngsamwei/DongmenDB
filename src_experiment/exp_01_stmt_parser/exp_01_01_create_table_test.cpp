//
// Created by Sam on 2018/2/11.
//

#include <gtest/gtest.h>
#include <parser/StatementParser.h>
#include <parser/Tokenizer.h>
#include <test/test_stmt_parser.h>

class CreateTableParserTest : public testing::Test {
public:
    string sql{"create table typeTest(colChar char(10), colInt integer);"};
    sql_stmt_create expect{};
    sql_stmt_create result{};

    void SetUp() override {
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
        expect.tableInfo = new TableInfo(tableName, fieldsName, columns);
        expect.constraints = nullptr;

        auto *tokenizer = new Tokenizer(sql.c_str());
        auto *cp = new CreateParser(tokenizer);
        auto *stmt = cp->parse_sql_stmt_create();
        result = *stmt;
    }

};

TEST_F(CreateTableParserTest, Test_01_TableName) {

    // 表名
    EXPECT_TRUE(equal_table_name(expect, result));
}

TEST_F(CreateTableParserTest, Test_01_FieldName) {
    // 字段名
    EXPECT_TRUE(equal_fields_name(expect, result));
}

TEST_F(CreateTableParserTest, Test_01_Field) {
    // 字段
    EXPECT_TRUE(equal_fields(expect, result));
}

TEST_F(CreateTableParserTest, Test_01_TableInfo) {
    // 表信息
    EXPECT_TRUE(equal_table_info(expect, result));
}

TEST_F(CreateTableParserTest, Test_01_Full) {
    // 判断
    EXPECT_TRUE(equal(expect, result));

}
