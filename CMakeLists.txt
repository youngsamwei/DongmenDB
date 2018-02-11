cmake_minimum_required(VERSION 3.9)
project(DongmenDB C)
set(CMAKE_C_STANDARD 11)
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ./cmake-build-debug/)


#add_executable(testParseSelect ${DIR_SRCS} src_test/parser/testParseSelect.c)
#add_executable(testParserInsert ${DIR_SRCS} src_test/parser/testParserInsert.c)
#add_executable(testParserCreate ${DIR_SRCS} src_test/parser/testParserCreate.c)
#add_executable(testTokenizer ${DIR_SRCS} src_test/parser/testTokenizer.c)

add_subdirectory(src bin)

add_subdirectory(src_test/select bin_test)
