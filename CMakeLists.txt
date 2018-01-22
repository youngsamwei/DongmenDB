cmake_minimum_required(VERSION 3.9)
project(DongmenDB C)
set(CMAKE_C_STANDARD 11)

INCLUDE_DIRECTORIES(
        src/parser
        src/query
        src/buffer
        src/file
        src/index
        src/log
        src/materialize
        src/metadata
        src/opt
        src/planner
        src/record
        src/shell
        src/tx
        src/utils
)

aux_source_directory( src/parser DIR_SRCS)
aux_source_directory( src/query DIR_SRCS)
aux_source_directory( src/buffer DIR_SRCS)
aux_source_directory( src/file DIR_SRCS)
aux_source_directory( src/index DIR_SRCS)
aux_source_directory( src/log DIR_SRCS)
aux_source_directory( src/materialize DIR_SRCS)
aux_source_directory( src/metadata DIR_SRCS)
aux_source_directory( src/opt DIR_SRCS)
aux_source_directory( src/planner DIR_SRCS)
aux_source_directory( src/record DIR_SRCS)
aux_source_directory( src/shell DIR_SRCS)
aux_source_directory( src/tx DIR_SRCS)
aux_source_directory( src/utils DIR_SRCS)


add_executable(testParserExpressionRD ${DIR_SRCS} src_test/parser/testParserExpressionRD.c)

#add_executable(testTokenizer ${DIR_SRCS} src_test/parser/testTokenizer.c)