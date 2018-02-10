cmake_minimum_required(VERSION 3.9)
project(DongmenDB C)
set(CMAKE_C_STANDARD 11)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ./cmake-build-debug/)

INCLUDE_DIRECTORIES(
        include
        src/parser
        src/physicalplan
        src/queryplan
        src/shell
        src/utils
)


aux_source_directory( src/dongmengdb DIR_SRCS)
aux_source_directory( src/parser DIR_SRCS)
aux_source_directory(src/physicalplan DIR_SRCS)
aux_source_directory(src/queryplan DIR_SRCS)
aux_source_directory( src/utils DIR_SRCS)

aux_source_directory( src/shell DIR_SRCS)

add_executable(dongmengdb ${DIR_SRCS} )

#add_executable(testParseSelect ${DIR_SRCS} src_test/parser/testParseSelect.c)
#add_executable(testParserInsert ${DIR_SRCS} src_test/parser/testParserInsert.c)
#add_executable(testParserCreate ${DIR_SRCS} src_test/parser/testParserCreate.c)
#add_executable(testTokenizer ${DIR_SRCS} src_test/parser/testTokenizer.c)