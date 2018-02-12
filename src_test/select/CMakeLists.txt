include(CheckFunctionExists)
include(CheckCXXSourceCompiles)
include(CheckLibraryExists)
include(CPack)

INCLUDE_DIRECTORIES(
        ../../include/dongmendb
        ../../include/dongmensql
        ../../src/parser
        ../../src/physicalplan
        ../../src/queryplan
        ../../src/shell
        ../../src/utils
)

aux_source_directory(../../src/dongmendb DIR_TEST_SRCS)
aux_source_directory(../../src/parser DIR_TEST_SRCS)
aux_source_directory(../../src/physicalplan DIR_TEST_SRCS)
aux_source_directory(../../src/queryplan DIR_TEST_SRCS)
aux_source_directory(../../src/utils DIR_TEST_SRCS)

aux_source_directory(. DIR_TEST_SRCS)

# 启用测试

add_executable(test_select ${DIR_TEST_SRCS})

enable_testing()

#用宏定义测试用例
macro (test_select index dbname result sql)
    add_test (NAME test_${dbname}_${index} COMMAND test_select ${dbname} ${sql} )
    set_tests_properties (test_${dbname}_${index}
            PROPERTIES PASS_REGULAR_EXPRESSION ${result})

endmacro (test_select)

# 使用该宏进行一系列的数据测试
test_select (1 "demodb" "record count is 8" "select sno from student")
test_select (2 "demodb" "record count is 5" "select sno from student where sage < 25")
test_select (3 "demodb" "record count is 1" "select sno from student where sage = 25" )
test_select (4 "demodb" "record count is 1" "select sno from student where sno =\"2012010106\"")
test_select (5 "demodb" "record count is 2" "select sno from student where sage >= 25 and sage <= 26" )

test_select (6 "demodb" "record count is 13" "select student.sno, sc.cno,sname,grade, cname from student, sc, course where student.sno = sc.sno and sc.cno = course.cno")
test_select (7 "demodb" "record count is 6" "select sno,cno,grade, cname from sc, course where sc.cno = course.cno and grade > 80")
test_select (8 "demodb" "record count is 13" "select student.*, grade from student, sc where student.sno = sc.sno")
test_select (9 "demodb" "record count is 13" "select student.*, sc.* from student, sc where student.sno = sc.sno")
test_select (10 "demodb" "record count is 13" "select student.* from student, sc, course where student.sno = sc.sno and sc.cno = course.cno")
test_select (11 "demodb" "record count is 13" "select student.*, sc.*, course.* from student, sc, course where student.sno = sc.sno and sc.cno = course.cno")



# 产生mingw的make文件
# cmake -G   "MinGW Makefiles" .
# mingw32-make
# mingw32-make test