
set(LIBRARIES
        gtest
        gtest_main
        dongmendb dongmensql parser physicalplan shell utils
        )

add_executable(test_01_03_select test_01_03_select.cpp)
target_link_libraries(test_01_03_select ${LIBRARIES})

set(SOURCE_FILES test_01_03_select_01.cpp)
add_executable(test_01_03_select_01 ${SOURCE_FILES})
target_link_libraries(test_01_03_select_01 ${LIBRARIES})
