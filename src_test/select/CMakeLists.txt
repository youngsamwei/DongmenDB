include(CheckFunctionExists)
include(CheckCXXSourceCompiles)
include(CheckLibraryExists)
include(CPack)

# 启用测试

add_executable(test_select test_select.c)

enable_testing()

#用宏定义测试用例
macro (do_test arg1 arg2 result)
    add_test (NAME test_${arg1}_${arg2} COMMAND test_select ${arg1} ${arg2} )
    set_tests_properties (test_${arg1}_${arg2}
            PROPERTIES PASS_REGULAR_EXPRESSION ${result})
endmacro (do_test)

# 使用该宏进行一系列的数据测试
do_test (5 2 "is 7")
do_test (10 5 "is 15")
do_test (2 10 "is 12")
do_test (1 3 "is 4")

# 产生mingw的make文件
# cmake -G   "MinGW Makefiles" .
# mingw32-make
# mingw32-make test