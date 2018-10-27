#
# 注意：不要修改任何 CMakeLists.txt 文件中的任何内容，否则可能会导致编译不正确。
# 如果不小心修改了，请使用对应目录下的原始CMakeLists.txt 文件覆盖。
#


aux_source_directory(utils DIR_SRCS_UTILS)
ADD_LIBRARY(utils ${DIR_SRCS_UTILS})

aux_source_directory(dongmendb DIR_SRCS_DONGMENDB)
#增加安全性管理的实验内容
set(DIR_SRCS_DONGMENDB ${DIR_SRCS_DONGMENDB} ../src_experiment/exp_04_security/exp_04_02_rights_manager.c )
#增加完整性检查的实验内容
set(DIR_SRCS_DONGMENDB ${DIR_SRCS_DONGMENDB} ../src_experiment/exp_03_integrity/exp_03_01_integrity_check.c )

#增加语义检查的实验内容
set(DIR_SRCS_DONGMENDB ${DIR_SRCS_DONGMENDB} ../src_experiment/exp_02_semantic/exp_02_01_table_exists.c )
set(DIR_SRCS_DONGMENDB ${DIR_SRCS_DONGMENDB} ../src_experiment/exp_02_semantic/exp_02_02_field_exists.c )

ADD_LIBRARY(dongmendb ${DIR_SRCS_DONGMENDB})
TARGET_LINK_LIBRARIES(dongmendb utils)

aux_source_directory(parser DIR_SRCS_PARSER)
#增加安全性管理的实验内容
set(DIR_SRCS_PARSER ${DIR_SRCS_PARSER} ../src_experiment/exp_04_security/exp_04_01_security_parser.c )
#增加create table语句解析的实验内容
set(DIR_SRCS_PARSER ${DIR_SRCS_PARSER} ../src_experiment/exp_01_stmt_parser/exp_01_01_create_table.c )
#增加insert语句解析的实验内容
set(DIR_SRCS_PARSER ${DIR_SRCS_PARSER} ../src_experiment/exp_01_stmt_parser/exp_01_02_insert.c )
#增加select语句解析的实验内容
set(DIR_SRCS_PARSER ${DIR_SRCS_PARSER} ../src_experiment/exp_01_stmt_parser/exp_01_03_select.c )
#增加update语句解析的实验内容
set(DIR_SRCS_PARSER ${DIR_SRCS_PARSER} ../src_experiment/exp_01_stmt_parser/exp_01_04_update.c )
#增加delete语句解析的实验内容
set(DIR_SRCS_PARSER ${DIR_SRCS_PARSER} ../src_experiment/exp_01_stmt_parser/exp_01_05_delete.c )

#增加update和delete的执行的实验内容
set(DIR_SRCS_PARSER ${DIR_SRCS_PARSER} ../src_experiment/exp_07_physical_operate/exp_07_05_update.c )
set(DIR_SRCS_PARSER ${DIR_SRCS_PARSER} ../src_experiment/exp_07_physical_operate/exp_07_06_delete.c )

ADD_LIBRARY(parser ${DIR_SRCS_PARSER})

aux_source_directory(physicalplan DIR_SRCS_PHYSICALPLAN)
ADD_LIBRARY(physicalplan ${DIR_SRCS_PHYSICALPLAN})

aux_source_directory(dongmensql DIR_SRCS_DONGMENSQL)
ADD_LIBRARY(dongmensql ${DIR_SRCS_DONGMENSQL})

aux_source_directory(shell DIR_SRCS_SHELL)
ADD_LIBRARY(shell ${DIR_SRCS_SHELL})

aux_source_directory(test_base DIR_SRCS_TEST_BASE)
ADD_LIBRARY(test_base ${DIR_SRCS_TEST_BASE})


TARGET_LINK_LIBRARIES(shell dongmendb parser dongmensql utils physicalplan test_base)
