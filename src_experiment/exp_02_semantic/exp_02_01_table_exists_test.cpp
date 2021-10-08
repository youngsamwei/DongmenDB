//
// Created by maokw on 2020-03-18.
//

#include <gtest/gtest.h>
#include <test/test_stmt_parser.h>
#include <test/test_physical_operate.h>

class TableExistsSemanticTest : public TestPhysicalOperate {
protected:
    void SetUp() override {
        createDB(dbname);
    }

    void TearDown() override {
        dropDB();
    }

    const char *dbname = "test_demodb";
};

TEST_F(TableExistsSemanticTest, TableExists) {
    dongmendb_shell_handle_create_table(test_db_ctx,
                                        "create table student(sno char(10), sname char(20), sage integer, ssex char(10));");
    int code = test_db_ctx->db->tableManager->semantic_check_table_exists("student", test_db_ctx->db->tx);
    EXPECT_TRUE(code == DONGMENDB_OK);
}

TEST_F(TableExistsSemanticTest, TableNotExists) {
    int code = test_db_ctx->db->tableManager->semantic_check_table_exists("student", test_db_ctx->db->tx);
    EXPECT_FALSE(code == DONGMENDB_OK);
}

