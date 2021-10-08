//
// Created by Sam on 2018/2/11.
//

#include <gtest/gtest.h>
#include <parser/StatementParser.h>
#include <parser/Tokenizer.h>
#include <test/test_stmt_parser.h>

sql_stmt_delete parse(const string &sql) {
    auto *tokenizer = new Tokenizer(sql.c_str());
    auto *ip = new DeleteParser(tokenizer);
    auto *stmt = ip->parse_sql_stmt_delete();
    if (stmt == nullptr) {
        string info{"parser failed: "};
        info += ip->parserMessage;
        throw std::runtime_error(info);
    }
    return *stmt;
}

class DeleteParserTest_01_Normal : public testing::Test {
protected:
    void SetUp() override {
        char *tableName{"student"};
        Expression *whereExpr =
                (new Parser(new Tokenizer("sname = 'tom simith'")))->parseExpressionRD();
        SRA_t *table = SRATable(TableReference_make(tableName, nullptr));
        SRA_t *where = SRASelect(table, whereExpr);

        expect.tableName = tableName;
        expect.where = where;

        actual = parse(sql);
    }

    const string sql = "delete student where sname = 'tom simith'";
    sql_stmt_delete expect{};
    sql_stmt_delete actual{};
};

TEST_F(DeleteParserTest_01_Normal, TableName) {
    EXPECT_TRUE(equal_table_name(expect, actual));
}

TEST_F(DeleteParserTest_01_Normal, Where) {
    EXPECT_TRUE(equal_where(expect, actual));
}

TEST_F(DeleteParserTest_01_Normal, Full) {
    EXPECT_TRUE(equal(expect, actual))
                        << "Actual:" << endl
                        << actual << endl
                        << "Expected:" << endl
                        << expect;
}