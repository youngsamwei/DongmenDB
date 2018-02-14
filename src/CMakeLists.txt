
aux_source_directory(dongmendb DIR_SRCS_DONGMENDB)
ADD_LIBRARY(dongmendb ${DIR_SRCS_DONGMENDB})

aux_source_directory(parser DIR_SRCS_PARSER)
ADD_LIBRARY(parser ${DIR_SRCS_PARSER})

aux_source_directory(physicalplan DIR_SRCS_PHYSICALPLAN)
ADD_LIBRARY(physicalplan ${DIR_SRCS_PHYSICALPLAN})

aux_source_directory(dongmensql DIR_SRCS_DONGMENSQL)
ADD_LIBRARY(dongmensql ${DIR_SRCS_DONGMENSQL})

aux_source_directory(utils DIR_SRCS_UTILS)
ADD_LIBRARY(utils ${DIR_SRCS_UTILS})

aux_source_directory(shell DIR_SRCS_SHELL)
ADD_LIBRARY(shell ${DIR_SRCS_SHELL})

