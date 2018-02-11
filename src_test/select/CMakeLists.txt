# 启用测试

ADD_EXECUTABLE(test_select test_select.c)

enable_testing()
# 测试程序是否成功运行
# add_test (test_run1 my_test.elf)
add_test (test_run2 test_select 1 2)


# 测试 1 + 3 , 是否输出打印'is 4'
add_test (test_1_plus_3 test_select 1 3)
set_tests_properties (test_1_plus_3
        PROPERTIES PASS_REGULAR_EXPRESSION "is 4")

# 产生mingw的make文件
# cmake -G   "MinGW Makefiles" .
# mingw32-make
# mingw32-make test
