//
// Created by Sam on 2018/1/23.
//

#include <stdio.h>
#include <statement.h>
#include <parser.h>
#include <parseSelectStmt.h>
#include <mem.h>
#include <physicalplan.h>


int main(int argc, char **argv) {

    char *select = "select sno,ltrim(sname), round(ssex + 1,1) from student, sc, course where student.sno = sc.sno and sc.cno = course.cno and sage > 20";
    select = "select sno,sname, ssex from student, sc, course where student.sno = sc.sno and sc.cno = course.cno and sage > 20";
    if (argc == 2) {
        select = argv[1];
    }

    TokenizerT *tokenizer = TKCreate(select);
    ParserT *parser = newParser(tokenizer);
    char selectStr[256];
    memset(selectStr,0,sizeof(selectStr));
    SRA_t *selectStmt = parse_sql_stmt_select(parser);
    if (selectStmt != NULL) {
        SRA_print(selectStmt);
    } else {
        printf(parser->parserMessage);
    }
    if (selectStmt != NULL) {
        plan_execute_select(db, selectStmt, tx);
    } else {
        printf(parser->parserMessage);
    }
}