//
// Created by Sam on 2018/1/20.
//

/**
 * 测试表达式的解析。
 * 算术表达式，条件表达式，逻辑表达式
 */
#include <parser/tokenizer.h>
#include <stdlib.h>
#include <stdio.h>
#include <gtest/gtest.h>
#include "parser/parser.h"
#include "parser/expression.h"

int parse_expression(const char *sexpr){
    int result = 0;
    TokenizerT *tokenizer = TKCreate(sexpr);
    ParserT *parser = newParser(tokenizer);

    Expression *expr = parseExpressionRD(parser);
    if (parser->parserStateType ==PARSER_WRONG){
        printf(parser->parserMessage);
        result = 0;
    }else {
        char *strexpr = (char *)calloc(1024,1);
        expression_print(expr, strexpr);
        result = 1;
    }
    TKDestroy(tokenizer);
    return result;
}

class ParserExpressionTest : public testing::Test {
    protected:
    virtual void SetUp() {
        _m_list[0] = "1";
        _m_list[1] = "(3) ";
        _m_list[2] = "1 + 2";
        _m_list[3] = "(3 - 4) ";
        _m_list[4] = "student.sno";
        _m_list[5] = "sno ";
        _m_list[6] = "student.*";
        _m_list[7] = "1>2 ";
        _m_list[8] = "a<b";
        _m_list[9] = "(1 - 2 ) > b";
        _m_list[10] = "a and (b or c) and d";
        _m_list[11] = "student.sage > 20 ";
        _m_list[12] = "student.sage + 1 > 20 ";
        _m_list[13] = "20 > student.sage - 1 ";
        _m_list[14] = "1 + 2 * (3 - 4) ";
        _m_list[15] = "a + b * (c - d)";
        _m_list[16] = "a - b * (c - (d + e)) > f.x";
        _m_list[17] = "b < 10 or c = 0 ";//比较运算 =

        _m_list[18] = "student.sage + 1 >  ";
        _m_list[19] = " > student.sage - 1 ";
        _m_list[20] = "1 + * (3 - 4) ";
        _m_list[21] = "a + b * (c - d";
        _m_list[22] = "a - b * c - (d + e) f.x";
        _m_list[23] = "b < 10 o c = 0 ";

    }
    const char *_m_list[24];
};

TEST_F(ParserExpressionTest, Correct){
    EXPECT_TRUE(parse_expression(_m_list[0]));
    EXPECT_TRUE(parse_expression(_m_list[1]));
    EXPECT_TRUE(parse_expression(_m_list[2]));
    EXPECT_TRUE(parse_expression(_m_list[3]));
    EXPECT_TRUE(parse_expression(_m_list[4]));
    EXPECT_TRUE(parse_expression(_m_list[5]));
    EXPECT_TRUE(parse_expression(_m_list[6]));
    EXPECT_TRUE(parse_expression(_m_list[7]));
    EXPECT_TRUE(parse_expression(_m_list[8]));
    EXPECT_TRUE(parse_expression(_m_list[9]));
    EXPECT_TRUE(parse_expression(_m_list[10]));
    EXPECT_TRUE(parse_expression(_m_list[11]));
    EXPECT_TRUE(parse_expression(_m_list[12]));
    EXPECT_TRUE(parse_expression(_m_list[13]));
    EXPECT_TRUE(parse_expression(_m_list[14]));
    EXPECT_TRUE(parse_expression(_m_list[15]));
    EXPECT_TRUE(parse_expression(_m_list[16]));
    EXPECT_TRUE(parse_expression(_m_list[17]));
}

TEST_F(ParserExpressionTest, Wrong){
    EXPECT_EQ(0, parse_expression(_m_list[18]));
    EXPECT_EQ(0, parse_expression(_m_list[19]));
    EXPECT_EQ(0, parse_expression(_m_list[20]));
    EXPECT_EQ(0, parse_expression(_m_list[21]));
    EXPECT_EQ(0, parse_expression(_m_list[22]));
    EXPECT_EQ(0, parse_expression(_m_list[23]));
}

