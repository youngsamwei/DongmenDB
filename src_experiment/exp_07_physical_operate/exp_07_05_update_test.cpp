//
// Created by maokw on 2020-03-04.
//

#include <gtest/gtest.h>
#include <test/test_stmt_parser.h>
#include <test/test_physical_operate.h>

class UpdateOperateTest : public TestPhysicalOperate {
protected:
    void SetUp() override {
            createDB(dbname);
            createTable();
            insertData();
    }

    void TearDown() override {
            dropDB();
    }

    const char *dbname = "test_demodb";
};


TEST_F(UpdateOperateTest, Test1) {
    EXPECT_EQ(1, update("update student set sname = 'Tom Cruise' where sno = '2012010101'"));
}

TEST_F(UpdateOperateTest, Test2) {

    EXPECT_EQ(1, update("update student set sname = 'zhang simith' where sname = 'li simith'"));
}

TEST_F(UpdateOperateTest, Test3) {
    EXPECT_EQ(1, update("update student set sname = 'zhang simith', ssex= 'male' where sname = 'li simith'"));
}

TEST_F(UpdateOperateTest, Test4) {
    EXPECT_EQ(1,
              update("update student set sname = 'zhang simith', ssex= 'male', sage = sage + 1 where sname = 'li simith'"));
}

TEST_F(UpdateOperateTest, Test5) {
    EXPECT_EQ(0, update("update student set sname = 'li simith' where sname = 'zhang simith'"));
}

TEST_F(UpdateOperateTest, Test6) {
    EXPECT_EQ(1, update("update student set sage = sage + 1 where sname = 'li simith'"));
}

TEST_F(UpdateOperateTest, Test7) {
    EXPECT_EQ(9, update("update student set sage = sage + 1"));
}

TEST_F(UpdateOperateTest, Test8) {
    EXPECT_EQ(1, update("update student set sage = 20 where sno = '2012010101'"));
}

TEST_F(UpdateOperateTest, Test9) {
    EXPECT_EQ(0, update("update student set sname = 'To mCruise' where sage > 30"));
}

TEST_F(UpdateOperateTest, Test10) {
    EXPECT_EQ(9, update("update student set sage = sage + 1 where sage > 10 and ssex = 'male'"));
}
