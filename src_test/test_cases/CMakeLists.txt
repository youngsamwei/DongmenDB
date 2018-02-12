include(CheckFunctionExists)
include(CheckCXXSourceCompiles)
include(CheckLibraryExists)
include(CPack)


# 启用测试


add_executable(test_cases test_cases.c)

enable_testing()


file(STRINGS ../select/test_cases.sql test_cases)
set(_index 0)
#如果每行包含多个参数且以分号隔开，自动识别为list，则自动拆分成多个参数传给add_test
foreach(_case ${test_cases})
    add_test(NAME test_${_index} COMMAND test_cases ${_case})
    set_tests_properties (test_${_index}
            PROPERTIES PASS_REGULAR_EXPRESSION "demodb")
    math(EXPR _index "${_index}+1")
endforeach(_case)



# 产生mingw的make文件
# cmake -G   "MinGW Makefiles" .
# mingw32-make
# mingw32-make test