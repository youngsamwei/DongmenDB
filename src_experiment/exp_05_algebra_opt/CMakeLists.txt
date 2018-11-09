#
# 注意：不要修改任何 CMakeLists.txt 文件中的任何内容，否则可能会导致编译不正确。
# 如果不小心修改了，请使用对应目录下的原始CMakeLists.txt 文件覆盖。
#


add_executable(Exp_05_02_ConditionPushdownTest exp_05_02_condition_push_down_test.cpp  ../../src/shell/shell.c)
target_link_libraries(Exp_05_02_ConditionPushdownTest ${TEST_LIBRARIES})
