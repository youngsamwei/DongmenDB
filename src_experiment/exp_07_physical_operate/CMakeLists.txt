#
# 注意：不要修改任何 CMakeLists.txt 文件中的任何内容，否则可能会导致编译不正确。
# 如果不小心修改了，请使用对应目录下的原始CMakeLists.txt 文件覆盖。
#


add_executable(exp_07_05_update_test exp_07_05_update_test.cpp ../../src/shell/shell.cpp)
target_link_libraries(exp_07_05_update_test ${TEST_LIBRARIES})

add_executable(exp_07_06_delete_test exp_07_06_delete_test.cpp ../../src/shell/shell.cpp)
target_link_libraries(exp_07_06_delete_test ${TEST_LIBRARIES})
