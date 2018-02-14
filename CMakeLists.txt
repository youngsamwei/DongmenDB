cmake_minimum_required(VERSION 3.9)
project(DongmenDB C)
set(CMAKE_C_STANDARD 11)

#设置编译器
SET(CMAKE_CXX_COMPILER "gcc")
#设置构建类型，及相应的编译选项
SET(CMAKE_BUILD_TYPE "Debug")
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

#设置执行文件输出目录
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
#设置库输出路径
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

#头文件搜索目录
INCLUDE_DIRECTORIES(include)
#链接库搜索路径
LINK_DIRECTORIES(lib)

AUX_SOURCE_DIRECTORY(. SRC_LIST)
add_subdirectory(src)

#测试文件夹，测试时使用终端Terminal进入 bin_test，运行 ctest 进行单元测试
add_subdirectory(src_test/test_01_stmt_parser/test_01_03_select_cases)
SET(EXTRA_LIBS dongmendb dongmensql parser physicalplan shell utils)

ADD_EXECUTABLE(dmdb ${SRC_LIST})
TARGET_LINK_LIBRARIES(dmdb ${EXTRA_LIBS})