//
// Created by maokw on 2020-03-04.
//

#include <cstdio>
#include <gtest/gtest.h>
#include <test/test_stmt_parser.h>
#include <test/test_physical_operate.h>

class Exp_07_05_UpdateTest : public TestPhysicalOperate {
protected:
    void SetUp() override {
        _m_list[0] = "update student set sname = 'Tom Cruise' where sno = '2012010101' ";
        _m_list[1] = "update student set sname = 'zhang simith' where sname = 'li simith' ";
        _m_list[2] = "update student set sname = 'li simith', ssex= 'male' where sname = 'zhang simith'; ";
        _m_list[3] = "update student set sname = 'zhang simith', ssex= 'male', sage = sage + 1 where sname = 'li simith'";
        _m_list[4] = "update student set sname = 'li simith' where sname = 'zhang simith'";
        _m_list[5] = "update student set sage = sage + 1 where sname = 'li simith'";
        _m_list[6] = "update student set sage = sage + 1 ";
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

    const char *_m_list[15]{};
    int _expect_list[15]{};
    const char *dbname = "test_demodb";
};

TEST_F(Exp_07_05_UpdateTest, Correct) {
    try {
        createDB(dbname);
        createTable();
        insertData();

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

        dropDB();
    } catch (const std::exception &e) {
        cout << e.what() << endl;
    }
}