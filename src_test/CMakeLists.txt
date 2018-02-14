
set(LIBRARIES
        gtest
        gtest_main
        dongmendb dongmensql parser physicalplan shell utils
        )

set(SOURCE_FILES test_01_03_select.cpp)
add_executable(test_01_03_select ${SOURCE_FILES})
target_link_libraries(test_01_03_select ${LIBRARIES})
