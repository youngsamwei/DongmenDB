#
# 注意：不要修改任何 CMakeLists.txt 文件中的任何内容，否则可能会导致编译不正确。
# 如果不小心修改了，请使用对应目录下的原始CMakeLists.txt 文件覆盖。
#


CMAKE_MINIMUM_REQUIRED(VERSION 3.9)
PROJECT(DongmenDB)
SET(CMAKE_C_STANDARD 11)
SET(CMAKE_CXX_STANDARD 11)

#设置执行文件输出目录
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
#设置库输出路径
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

#头文件搜索目录
INCLUDE_DIRECTORIES(include)
#链接库搜索路径
LINK_DIRECTORIES(lib)

# 将用于google test的头文件gtest.h添加到include路径中
INCLUDE_DIRECTORIES(googletest/include)

# 编译google test ADD_SUBDIRECTORY
ADD_SUBDIRECTORY(googletest)

#增加项目的源代码文件夹
ADD_SUBDIRECTORY(src)

#设置被依赖的库
SET(EXTRA_LIBS dongmendb dongmensql parser physicalplan shell utils)

#包含实验题目和测试的文件夹
ADD_SUBDIRECTORY(src_experiment)


#测试代码
ADD_SUBDIRECTORY(src_test)

# main.c 的代码列表
AUX_SOURCE_DIRECTORY(. MAIN_SRC_LIST)
ADD_EXECUTABLE(dmdb ${MAIN_SRC_LIST} )
# 设置dmdb的依赖
TARGET_LINK_LIBRARIES(dmdb ${EXTRA_LIBS})


