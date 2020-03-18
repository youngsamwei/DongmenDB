//
// Created by Sam on 2018/2/11.
//

#include <gtest/gtest.h>
#include <parser/StatementParser.h>
#include <parser/Tokenizer.h>
#include <test/test_stmt_parser.h>

sql_stmt_update parser(const string &sql) {
    auto *tokenizer = new Tokenizer(sql.c_str());
    auto *ip = new UpdateParser(tokenizer);
    auto *sqlStmtUpdate = ip->parse_sql_stmt_update();
    return *sqlStmtUpdate;
}

TEST(UpdateParserTest, Test_01_Normal) {
    const string sql = "update student set sage = sage + 1";
    char *tableName{"student"};
    vector<char *> fields{"sname"};
    vector<Expression *> fieldsExpr{
            (new Parser(new Tokenizer("'Tom Cruise'")))->parseExpressionRD()};
    SRA_t *where = nullptr;

    sql_stmt_update expect;
    expect.tableName = tableName;
    expect.fields = fields;
    expect.fieldsExpr = fieldsExpr;
    expect.where = where;

    auto actual = parser(sql);

    EXPECT_TRUE(equal_table_name(expect, actual));
    EXPECT_TRUE(equal_where(expect, actual));
    EXPECT_TRUE(equal_fields_expr(expect, actual));
    EXPECT_TRUE(equal_fields(expect, actual));
    EXPECT_TRUE(equal(expect, actual));
}

TEST(UpdateParserTest, Test_01_Where) {
    const string sql = "update student set sname = 'Tom Cruise' where sno = '2012010101'";
    char *tableName{"student"};
    vector<char *> fields{"sage"};
    vector<Expression *> fieldsExpr{
            (new Parser(new Tokenizer("Tom Cruise")))->parseExpressionRD()};
    Expression *whereExpr =
            (new Parser(new Tokenizer("sno = '2012010101'")))->parseExpressionRD();
    SRA_t *table = SRATable(TableReference_make(tableName, nullptr));
    SRA_t *where = SRASelect(table, whereExpr);

    sql_stmt_update expect;
    expect.tableName = tableName;
    expect.fields = fields;
    expect.fieldsExpr = fieldsExpr;
    expect.where = where;

    auto actual = parser(sql);

    EXPECT_TRUE(equal_table_name(expect, actual));
    EXPECT_TRUE(equal_where(expect, actual));
    EXPECT_TRUE(equal_fields_expr(expect, actual));
    EXPECT_TRUE(equal_fields(expect, actual));
    EXPECT_TRUE(equal(expect, actual));
}

TEST(UpdateParserTest, Test_01_Fields) {
    const string sql = "update student set sname = 'li simith', ssex = 'male' where sname = 'zhang simith'";
    char *tableName{"student"};
    vector<char *> fields{"sname", "ssex"};
    vector<Expression *> fieldsExpr{
            (new Parser(new Tokenizer("li simith")))->parseExpressionRD(),
            (new Parser(new Tokenizer("male")))->parseExpressionRD()};
    Expression *whereExpr = (new Parser(new Tokenizer("sname = 'zhang simith'")))
            ->parseExpressionRD();
    SRA_t *table = SRATable(TableReference_make(tableName, nullptr));
    SRA_t *where = SRASelect(table, whereExpr);

    sql_stmt_update expect;
    expect.tableName = tableName;
    expect.fields = fields;
    expect.fieldsExpr = fieldsExpr;
    expect.where = where;

    auto actual = parser(sql);

    EXPECT_TRUE(equal_table_name(expect, actual));
    EXPECT_TRUE(equal_where(expect, actual));
    EXPECT_TRUE(equal_fields_expr(expect, actual));
    EXPECT_TRUE(equal_fields(expect, actual));
    EXPECT_TRUE(equal(expect, actual));
}
