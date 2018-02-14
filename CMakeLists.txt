cmake_minimum_required(VERSION 3.9)
project(DongmenDB)
set(CMAKE_C_STANDARD 11)


#设置执行文件输出目录
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
#设置库输出路径
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

# 编译google test，会在当前目录生成libtest.a动态库
add_subdirectory(googletest)
# 将用于google test的头文件gtest.h添加到include路径中
include_directories(googletest/include)
# 将libtest.a添加到链接路径中（同时需要添加googletest运行需要的pthread）
link_directories(googletest)

#头文件搜索目录
INCLUDE_DIRECTORIES(include)
#链接库搜索路径
LINK_DIRECTORIES(lib)

AUX_SOURCE_DIRECTORY(. SRC_LIST)
add_subdirectory(src)

#测试文件夹，测试时使用终端Terminal进入 bin_test，运行 ctest 进行单元测试
add_subdirectory(src_test)
SET(EXTRA_LIBS dongmendb dongmensql parser physicalplan shell utils)

ADD_EXECUTABLE(dmdb ${SRC_LIST})
TARGET_LINK_LIBRARIES(dmdb ${EXTRA_LIBS})