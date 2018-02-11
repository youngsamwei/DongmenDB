
# 启用测试

add_executable(test_select test_select.c)

enable_testing()

macro (do_test arg1 arg2 result)
    add_test (test_${arg1}_${arg2} test_select ${arg1} ${arg2})
    set_tests_properties (test_${arg1}_${arg2}
            PROPERTIES PASS_REGULAR_EXPRESSION ${result})
endmacro (do_test)

# 使用该宏进行一系列的数据测试
do_test (5 2 "is 25")
do_test (10 5 "is 100000")
do_test (2 10 "is 1024")

# 测试程序是否成功运行
# add_test (test_run1 my_test.elf)
add_test (NAME test_run2 COMMAND test_select 1 2 ) #WORKING_DIRECTORY cmake-build-debug

# 测试 1 + 3 , 是否输出打印'is 4'
add_test (NAME test_1_plus_3 COMMAND test_select 1 3 ) #WORKING_DIRECTORY cmake-build-debug
set_tests_properties (test_1_plus_3  PROPERTIES PASS_REGULAR_EXPRESSION "is 4")

# 产生mingw的make文件
# cmake -G   "MinGW Makefiles" .
# mingw32-make
# mingw32-make test