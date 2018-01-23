//
// Created by Sam on 2018/1/20.
//

#include <tokenizer.h>
#include <stdlib.h>
#include <stdio.h>
#include <parseExpressionRD.h>
#include "parseExpression.h"

/*
 * main will have a string argument (in argv[1]).
 * The string argument contains the tokens.
 * Print out the tokens in the second string in left-to-right order.
 * Each token should be printed on a separate line.
 */
int main(int argc, char **argv) {

    char *sexpr;

    sexpr = " 1 + 2 * (3 - 4)   ";
     sexpr = " a + b * (c - (d + e))  ";
    sexpr = "1 > 2";
    sexpr = "a < b";
//    sexpr = "1 > 2 and a<b";
     sexpr = " a - b * (c - (d + e)) > f.x ";
    /*解析正确*/
    sexpr = " 1 + (ltrim(\"abc\" + field.name) + round(6, 8)  + 10) < 20 and f >g or b < a";
    //sexpr = "(1 + 2, t2, 3 < 5, t4 + fun(pi, ci))";
//    sexpr = "a and (b or c) and d";
//    sexpr = "a and (b or c) and d or e>6";
//    sexpr = " e > 2";
//    sexpr = "b < 10 or c = 0"; //解析错误:赋值与比较=没有分开
//    sexpr = "c = 0";
//    sexpr = "round 1,"; //解析至round则结束，因为一个合法的表达式结束了
//    sexpr = "ltrim(\" aaaaaa\")";
//    sexpr = " 1 + ltrim(\"abc\" + field.name)  + 10 < 20 ";
//    sexpr = "1 + ltrim(\"abc\" + field.name)  + 10 < 20 ";

/*错误的例子*/
    sexpr = " 1 + ltrim(\"abc\" + field.name) + round(6,8,10)  + 10 < 20 and f >g or b < a";
//    sexpr = "ltrim(\"aaaddd\", 10)"; //函数参数过多检测；
//    sexpr = "ltrim()";//函数参数过少检测;
//    sexpr = "round(6)";//函数参数过少检测;
//    sexpr = " 10 < 20 and f >g or b < a)"; //检测出)
//    sexpr = " (10";//检测(
//    sexpr = " a - b * (c - (d + e) > f.x "; //syntax error: missing ')'.
//    sexpr = "round(1) + 2"; //错误
//    sexpr = "ltrim(2";  //syntax error: syntax error: missing ')'.
//    sexpr = " 1 + round( field.name, fun(6, 8), 9)  + 10 < 20 ";  //x
    if (argc == 2) {
        sexpr = argv[1];
    }

    TokenizerT *tokenizer = TKCreate(sexpr);
    ParserT *parser = newParser(tokenizer);

    Expression *expr = parseExpressionRD(parser);
    if (parser->parserStateType ==PARSER_WRONG){
        printf(parser->parserMessage);
    }else {
        printExpression(expr);
    }
    TKDestroy(tokenizer);

    return 0;
}