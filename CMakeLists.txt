cmake_minimum_required(VERSION 3.9)
project(DongmenDB C)
set(CMAKE_C_STANDARD 11)

#头文件搜索目录
INCLUDE_DIRECTORIES(include)
#链接库搜索路径
LINK_DIRECTORIES(lib)

add_subdirectory(src output/bin)

#测试文件夹，测试时使用终端Terminal进入 bin_test，运行 ctest 进行单元测试
add_subdirectory(src_test/test_01_stmt_parser/test_01_03_select_cases output/bin_test_01_03_select_cases)
