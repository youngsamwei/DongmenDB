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
macro (test_select index dbname result sql)
    add_test (NAME test_${dbname}_${index} COMMAND test_select ${dbname} "1" ${sql} )
    set_tests_properties (test_${dbname}_${index}
            PROPERTIES PASS_REGULAR_EXPRESSION ${result})
endmacro (test_select)



file(STRINGS test_cases.sql test_cases)
set(_count 0)
#如果每行包含多个参数且以分号隔开，自动识别为list，则自动拆分成多个参数传给add_test
#怎么从_case中取出部分参数呢? 使用list(GET...)
#arg0;_dbname;_resutl;_sql;
foreach(_case ${test_cases})
    list(GET _case 0 arg0)
    list(GET _case 1 _dbname)
    list(GET _case 2 _result)
    list(GET _case 3 _sql)
    add_test(NAME test_${_dbname}_${arg0} COMMAND test_select ${_dbname} ${_sql})
    #设置TIMEOUT为3秒,以应对死循环和段错误等异常,但返回信息成为timeout，异常根源无法看到.
    set_tests_properties (test_${_dbname}_${arg0}
            PROPERTIES TIMEOUT 3 PASS_REGULAR_EXPRESSION ${_result})
    math(EXPR _count "${_count} + 1")
endforeach(_case)



# 产生mingw的make文件
# cmake -G   "MinGW Makefiles" .
# mingw32-make
# mingw32-make test