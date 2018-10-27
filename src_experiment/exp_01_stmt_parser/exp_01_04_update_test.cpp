//
// Created by Sam on 2018/2/11.
//

#include <stdio.h>
#include <stdlib.h>
#include <gtest/gtest.h>
#include <parser/statement.h>

#include "physicalplan/physicalplan.h"
#include "test/test_stmt_parser.h"

/* 2018-10-27 测试类改进：每次测试都重新创建数据库，重新初始化数据。
 * 测试类继承自 TestStmtParser

这个测试用例设计的还是不完善，只能做简单的测试，尚不能准确的测试是否解析的正确.*/


class Exp_01_04_UpdateTest : public TestStmtParser {


protected:

    virtual void SetUp() {
        _m_list[0] = "update student set sname = 'Tom Cruise' where sno = '2012010101' ";
        _m_list[1] = "update student set sname = 'zhang simith' where sname = 'li simith' ";
        _m_list[2] = "update student set sname = 'li simith', ssex= 'male' where sname = 'zhang simith'; ";
        _m_list[3] = "update student set sname = 'zhang simith', ssex= 'male', sage = sage + 1 where sname = 'li simith'";
        _m_list[4] = "update student set sname = 'li simith' where sname = 'zhang simith'";
        _m_list[5] = "update student set sage = sage + 1 where sname = 'li simith'";
        _m_list[6] = "update student set sage = sage + 1 " ;
        _m_list[7] = "update student set sage = 20 where sno = '2012010101'";
        _m_list[8] = "update student set sname = 'To mCruise' where sage > 30; ";
        _m_list[9] = "update student set sage = sage + 1 where sage > 30 and ssex = 'male'; ";

        _expect_list[0] = 1;
        _expect_list[1] = 1;
        _expect_list[2] = 1;
        _expect_list[3] = 1;
        _expect_list[4] = 1;
        _expect_list[5] = 1;
        _expect_list[6] = 9;
        _expect_list[7] = 1;
        _expect_list[8] = 0;
        _expect_list[9] = 0;
    }
    /* 测试用例*/
    const char *_m_list[11];
    /* 执行测试用例返回的影响记录条数*/
    int _expect_list[11];
    /*准备使用的测试数据库名称 */
    const char *dbname = "test_demodb";
};

TEST_F(Exp_01_04_UpdateTest, Correct){

    /*据指定的数据库名称创建数据库*/
    createDB(dbname);
/*创建表*/
    createTable();
/*增加数据*/
    insertData();

    /*执行测试，不能写成循环，因为无法清楚显示哪个测试用例失败了*/
    EXPECT_EQ(_expect_list[0], update(_m_list[0]));
    EXPECT_EQ(_expect_list[1], update(_m_list[1]));
    EXPECT_EQ(_expect_list[2], update(_m_list[2]));
    EXPECT_EQ(_expect_list[3], update(_m_list[3]));
    EXPECT_EQ(_expect_list[4], update(_m_list[4]));
    EXPECT_EQ(_expect_list[5], update(_m_list[5]));
    EXPECT_EQ(_expect_list[6], update(_m_list[6]));
    EXPECT_EQ(_expect_list[7], update(_m_list[7]));
    EXPECT_EQ(_expect_list[8], update(_m_list[8]));
    EXPECT_EQ(_expect_list[9], update(_m_list[9]));

    /*删除数据库*/
    dropDB();

}
