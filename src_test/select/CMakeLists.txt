include(CheckFunctionExists)
include(CheckCXXSourceCompiles)
include(CheckLibraryExists)
include(CPack)

INCLUDE_DIRECTORIES(
        ../../include/dongmendb
        ../../include/dongmensql
        ../../src/parser
        ../../src/physicalplan
        ../../src/queryplan
        ../../src/shell
        ../../src/utils
)

aux_source_directory(../../src/dongmendb DIR_TEST_SRCS)
aux_source_directory(../../src/parser DIR_TEST_SRCS)
aux_source_directory(../../src/physicalplan DIR_TEST_SRCS)
aux_source_directory(../../src/queryplan DIR_TEST_SRCS)
aux_source_directory(../../src/utils DIR_TEST_SRCS)

aux_source_directory(. DIR_TEST_SRCS)

# 启用测试

add_executable(test_select ${DIR_TEST_SRCS})

enable_testing()

#用宏定义测试用例
macro (test_select arg0 arg1 arg2 result)
    add_test (NAME test_${arg1}_${arg0} COMMAND test_select ${arg1} ${arg2} )
    set_tests_properties (test_${arg1}_${arg0}
            PROPERTIES PASS_REGULAR_EXPRESSION ${result})

endmacro (test_select)

# 使用该宏进行一系列的数据测试
test_select (1 "demodb" "select sno from student" "record count is 8")
test_select (2 "demodb" "select sno from student where sage < 25" "record count is 5")
test_select (3 "demodb" "select sno from student where sage = 25" "record count is 1")
test_select (4 "demodb" "select sno from student where sno =\"2012010106\"" "record count is 1")
test_select (5 "demodb" "select sno from student where sage >= 25 and sage <= 26" "record count is 2")


# 产生mingw的make文件
# cmake -G   "MinGW Makefiles" .
# mingw32-make
# mingw32-make test