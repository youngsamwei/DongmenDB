cmake_minimum_required(VERSION 3.9)
project(DongmenDB C)

INCLUDE_DIRECTORIES(src/utils)

set(CMAKE_C_STANDARD 11)

add_executable(DongmenDB src/utils/tokenizer.c src/utils/tokenizer.h)