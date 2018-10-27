#
# 注意：不要修改任何 CMakeLists.txt 文件中的任何内容，否则可能会导致编译不正确。
# 如果不小心修改了，请使用对应目录下的原始CMakeLists.txt 文件覆盖。
#



add_executable(exp_01_01_create_table_test exp_01_01_create_table_test.cpp)
target_link_libraries(exp_01_01_create_table_test ${TEST_LIBRARIES})

add_executable(exp_01_03_select_test exp_01_03_select_test.cpp)
target_link_libraries(exp_01_03_select_test ${TEST_LIBRARIES})


add_executable(exp_01_04_update_test exp_01_04_update_test.cpp  ../../src/shell/shell.c)
target_link_libraries(exp_01_04_update_test ${TEST_LIBRARIES})


add_executable(exp_01_05_delete_test exp_01_05_delete_test.cpp)
target_link_libraries(exp_01_05_delete_test ${TEST_LIBRARIES})
