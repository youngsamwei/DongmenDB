include(CheckFunctionExists)
include(CheckCXXSourceCompiles)
include(CheckLibraryExists)
include(CPack)

aux_source_directory(. DIR_TEST_SRCS)

add_executable(test_select ${DIR_TEST_SRCS})
TARGET_LINK_LIBRARIES(test_select dongmendb parser dongmensql utils physicalplan)

# 启用测试

enable_testing()

file(STRINGS test_cases.sql test_cases)
set(_count 0)
#如果每行包含多个参数且以分号隔开，自动识别为list，则自动拆分成多个参数传给add_test
#怎么从_case中取出部分参数呢? 使用list(GET...)
#arg0;_dbname;_resutl;_sql;
foreach(_case ${test_cases})
    list(GET _case 0 arg0)
    #跳过注释行,有错误呢
    if(arg0 MATCHES "#.*")
         continue()
    endif(arg0 MATCHES "#.*")
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