#
# 注意：不要修改任何 CMakeLists.txt 文件中的任何内容，否则可能会导致编译不正确。
# 如果不小心修改了，请使用对应目录下的原始CMakeLists.txt 文件覆盖。
#


set(TEST_LIBRARIES
        gtest
        gtest_main
        ${EXTRA_LIBS}
        )

add_subdirectory(exp_01_stmt_parser)
#
#add_subdirectory(exp_02_semantic)
#
#add_subdirectory(exp_03_integrity)
#
add_subdirectory(exp_04_security)
#
add_subdirectory(exp_05_algebra_opt)
#
#add_subdirectory(exp_06_index)
#
add_subdirectory(exp_07_physical_operate)
#
#add_subdirectory(exp_08_physical_opt)
#
#add_subdirectory(exp_09_lock)
#
#add_subdirectory(exp_10_buffer_manager)
