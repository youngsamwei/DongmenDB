
INCLUDE_DIRECTORIES(
        ../include/dongmendb
        ../include/dongmensql
        parser
        physicalplan
        queryplan
        shell
        utils
)

aux_source_directory(dongmendb DIR_SRCS)
aux_source_directory(parser DIR_SRCS)
aux_source_directory(physicalplan DIR_SRCS)
aux_source_directory(queryplan DIR_SRCS)
aux_source_directory(utils DIR_SRCS)

aux_source_directory(shell DIR_SRCS)


add_executable(dongmendb ${DIR_SRCS} )
