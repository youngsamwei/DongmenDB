set(TEST_LIBRARIES
        gtest
        gtest_main
        ${EXTRA_LIBS}
        )

add_subdirectory(test_parser)
add_subdirectory(test_execution)