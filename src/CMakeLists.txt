#
# 注意：不要修改任何 CMakeLists.txt 文件中的任何内容，否则可能会导致编译不正确。
# 如果不小心修改了，请使用对应目录下的原始CMakeLists.txt 文件覆盖。
#


aux_source_directory(utils DIR_SRCS_UTILS)
ADD_LIBRARY(utils ${DIR_SRCS_UTILS})

aux_source_directory(dongmendb DIR_SRCS_DONGMENDB)
aux_source_directory(exp_04_security/exp_04_02_rights_manager.c DIR_SRCS_DONGMENDB)
ADD_LIBRARY(dongmendb ${DIR_SRCS_DONGMENDB})
TARGET_LINK_LIBRARIES(dongmendb utils)

aux_source_directory(parser DIR_SRCS_PARSER)
aux_source_directory(exp_04_security/exp_04_01_security_parser.c DIR_SRCS_PARSER)
ADD_LIBRARY(parser ${DIR_SRCS_PARSER})

aux_source_directory(physicalplan DIR_SRCS_PHYSICALPLAN)
ADD_LIBRARY(physicalplan ${DIR_SRCS_PHYSICALPLAN})

aux_source_directory(dongmensql DIR_SRCS_DONGMENSQL)
ADD_LIBRARY(dongmensql ${DIR_SRCS_DONGMENSQL})

aux_source_directory(shell DIR_SRCS_SHELL)
ADD_LIBRARY(shell ${DIR_SRCS_SHELL})
TARGET_LINK_LIBRARIES(shell dongmendb parser dongmensql utils physicalplan)
