//
// Created by sam on 2018/11/10.
//

#ifndef DONGMENDB_TEST_STMT_OPTIMIZED_H
#define DONGMENDB_TEST_STMT_OPTIMIZED_H

#include <test/test_stmt_parser.h>
#include <shell/shell.h>

#include <dongmendb/dongmendb.h>
#include <gtest/gtest.h>
#include <utils/utils.h>
#include <parser/tokenizer.h>
#include <parser/parser.h>
#include <dongmensql/sra.h>
#include "physicalplan/physicalplan.h"


class TestStmtOptimized : public TestStmtParser {
public:
    /*关系代数优化之查询条件优化的测试
     * 参数：select语句
     * 返回：若检查通过则返回1，失败则返回0*/
    int opt_condition_pushdown_test(const char *sqlselect);

};
#endif //DONGMENDB_TEST_STMT_OPTIMIZED_H
