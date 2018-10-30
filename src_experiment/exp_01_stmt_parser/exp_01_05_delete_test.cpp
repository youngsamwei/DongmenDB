//
// Created by Sam on 2018/2/11.
//

#include <stdlib.h>
#include <gtest/gtest.h>
#include <test/test_stmt_parser.h>


class Exp_01_05_DeleteTest : public TestStmtParser {
protected:

    virtual void SetUp() {
        _m_list[0] = "delete from student where sname = 'tom simith'";
        _m_list[1] = "select * from student where sname = 'tom simith'";
        _m_list[2] = "delete from student where sno = '2012010102'";
        _m_list[3] = "select *  from student where sno = '2012010102'";
        _m_list[4] = "delete from student  where sname = 'john simith' and ssex='male'";
        _m_list[5] = "select *  from student  where sname = 'john simith' and ssex='male'";
        _m_list[6] = "delete from sc where sno = '2012010103' ";
        _m_list[7] = "select *  from sc where sno = '2012010103' ";
        _m_list[8] = "delete from sc  where grade >= 80 and cno = 'c003'";
        _m_list[9] = "select *  from sc  where grade >= 80 and cno = 'c003'";
    }
    const char *_m_list[11];
    const char *dbname = "test_demodb";
};

TEST_F(Exp_01_05_DeleteTest, Correct){
    /*据指定的数据库名称创建数据库*/
    createDB(dbname);
/*创建表*/
    createTable();
/*增加数据*/
    insertData();

    /* 执行删除的测试用例，返回结果1*/
    EXPECT_EQ(1, delete_( _m_list[0]));
    /*执行验证删除的测试用例，返回结果0，看是否真的删除*/
    EXPECT_EQ(0, select( _m_list[1]));
    EXPECT_EQ(1, delete_( _m_list[2]));
    EXPECT_EQ(0, select( _m_list[3]));
    EXPECT_EQ(1, delete_( _m_list[4]));
    EXPECT_EQ(0, select( _m_list[5]));
    EXPECT_EQ(2, delete_( _m_list[6]));
    EXPECT_EQ(0, select( _m_list[7]));
    EXPECT_EQ(3, delete_( _m_list[8]));
    EXPECT_EQ(0, select( _m_list[9]));

    /*删除数据库*/
    dropDB();

}
