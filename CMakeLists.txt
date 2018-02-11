cmake_minimum_required(VERSION 3.9)
project(DongmenDB C)
set(CMAKE_C_STANDARD 11)

add_subdirectory(src bin)

#测试文件夹，测试时使用终端Terminal进入 bin_test，运行 ctest 进行单元测试
add_subdirectory(src_test/select bin_test)
