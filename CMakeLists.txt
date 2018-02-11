cmake_minimum_required(VERSION 3.9)
project(DongmenDB C)
set(CMAKE_C_STANDARD 11)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ./cmake-build-debug/)

INCLUDE_DIRECTORIES(
        include/dongmendb
        include/dongmensql
        src/parser
        src/physicalplan
        src/queryplan
        src/shell
        src/utils
)

aux_source_directory(src/dongmendb DIR_SRCS)
aux_source_directory( src/parser DIR_SRCS)
aux_source_directory(src/physicalplan DIR_SRCS)
aux_source_directory(src/queryplan DIR_SRCS)
aux_source_directory( src/utils DIR_SRCS)

aux_source_directory( src/shell DIR_SRCS)

add_executable(dongmendb ${DIR_SRCS} )

# 启用测试

add_executable(test_select src_test/select/test_select.c)

enable_testing()
# 测试程序是否成功运行
# add_test (test_run1 my_test.elf)
add_test (NAME test_run2 COMMAND test_select 1 2 WORKING_DIRECTORY cmake-build-debug)

# 测试 1 + 3 , 是否输出打印'is 4'
add_test (NAME test_1_plus_3 COMMAND test_select 1 3 WORKING_DIRECTORY cmake-build-debug)
set_tests_properties (test_1_plus_3
        PROPERTIES PASS_REGULAR_EXPRESSION "is 4")

# 产生mingw的make文件
# cmake -G   "MinGW Makefiles" .
# mingw32-make
# mingw32-make test

#add_executable(testParseSelect ${DIR_SRCS} src_test/parser/testParseSelect.c)
#add_executable(testParserInsert ${DIR_SRCS} src_test/parser/testParserInsert.c)
#add_executable(testParserCreate ${DIR_SRCS} src_test/parser/testParserCreate.c)
#add_executable(testTokenizer ${DIR_SRCS} src_test/parser/testTokenizer.c)