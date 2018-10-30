//
// Created by sam on 2018/10/20.
//

#ifndef DONGMENDB_TEST_STMT_PARSER_H
#define DONGMENDB_TEST_STMT_PARSER_H


#include <shell/shell.h>

#include <dongmendb/dongmendb.h>
#include <gtest/gtest.h>
#include <utils/utils.h>
#include <parser/tokenizer.h>
#include <parser/parser.h>
#include <dongmensql/sra.h>
#include "physicalplan/physicalplan.h"


/* 2018-10-27 测试sql语句的基础类。
 * 每个测试都会重新创建数据库，初始化数据，执行测试，删除数据库。
 *
 * */

class TestStmtParser : public testing::Test {

public:
    dongmendb_shell_handle_sql_t *test_db_ctx;

    /*创建数据库*/
    void createDB(const char *dbname);
    /*创建数据表*/
    void createTable();
    /*为数据表增加数据*/
    void insertData();
    /*增加数据*/
    void dropDB();
    /*执行select语句*/
    int select(const char *sqlselect);
    /*执行delete语句*/
    int delete_(const char *strdelete);
    /*执行update语句*/
    int update(const char *strupdate);

    const char *_create_table_list[3] = {
            "create table student(sno char(10), sname char(20), sage integer, ssex char(10));",
            "create table course(cno char(10), cname char(20), credit integer);",
            "create table sc(sno char(10), cno char(10), grade integer);"
    };
    const char *_insert_list[27]= {
            "insert into student(sno, sname, sage, ssex) values('2012010101','tom simith',20,'male');",
            "insert into student(sno, sname, sage, ssex) values('2012010102','jack simith',21,'male');",
            "insert into student(sno, sname, sage, ssex) values('2012010103','li simith',22,'male');",
            "insert into student(sno, sname, sage, ssex) values('2012010104','john simith',23,'male');",
            "insert into student(sno, sname, sage, ssex) values('2012010105','dan simith',24,'female');",
            "insert into student(sno, sname, sage, ssex) values('2012010106','mary simith',25,'female');",
            "insert into student(sno, sname, sage, ssex) values('2012010107','jene simith',26,'female');",
            "insert into student(sno, sname, sage, ssex) values('2012010108','di simith',27,'female');",
            "insert into student(sno, sname, sage, ssex) values('2012010108','sam simith',27,'male');",

            "insert into course(cno, cname, credit) values('c001', 'data structure', 3);",
            "insert into course(cno, cname, credit) values('c002', 'c', 4);",
            "insert into course(cno, cname, credit) values('c003', 'database', 3);",
            "insert into course(cno, cname, credit) values('c004', 'math', 6);",

            "insert into sc(sno, cno, grade) values('2012010101','c001',80);",
            "insert into sc(sno, cno, grade) values('2012010102','c001',45);",
            "insert into sc(sno, cno, grade) values('2012010103','c001',75);",
            "insert into sc(sno, cno, grade) values('2012010104','c001',77);",
            "insert into sc(sno, cno, grade) values('2012010105','c001',67);",
            "insert into sc(sno, cno, grade) values('2012010106','c001',87);",
            "insert into sc(sno, cno, grade) values('2012010107','c001',90);",
            "insert into sc(sno, cno, grade) values('2012010101','c002',96);",
            "insert into sc(sno, cno, grade) values('2012010102','c002',74);",
            "insert into sc(sno, cno, grade) values('2012010103','c002',86);",
            "insert into sc(sno, cno, grade) values('2012010104','c003',95);",
            "insert into sc(sno, cno, grade) values('2012010105','c003',89);",
            "insert into sc(sno, cno, grade) values('2012010106','c003',48);",
            "insert into sc(sno, cno, grade) values('2012010107','c003',80);"
    };

};
#endif //DONGMENDB_TEST_STMT_PARSER_H
