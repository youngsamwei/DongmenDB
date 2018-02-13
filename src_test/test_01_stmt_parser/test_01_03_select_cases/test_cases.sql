# 测试select语句.以#开头的行为注释行。
# 编号；数据库名称；要求的返回结果；测试语句
1;demodb;record count is 8;select sno from student;
2;demodb;record count is 5;select sno from student where sage < 25;
3;demodb;record count is 1;select sno from student where sage = 25;
4;demodb;record count is 1;select sno from student where sno ="2012010106";
5;demodb;record count is 2;select sno from student where sage >= 25 and sage <= 26;
6;demodb;record count is 13;select student.sno, sc.cno,sname,grade, cname from student, sc, course where student.sno = sc.sno and sc.cno = course.cno;
7;demodb;record count is 6;select sno,cno,grade, cname from sc, course where sc.cno = course.cno and grade > 80;
8;demodb;record count is 13;select student.*, grade from student, sc where student.sno = sc.sno;
9;demodb;record count is 13;select student.*, sc.* from student, sc where student.sno = sc.sno;
10;demodb;record count is 13;select student.* from student, sc, course where student.sno = sc.sno and sc.cno = course.cno;
11;demodb;record count is 13;select student.*, sc.*, course.* from student, sc, course where student.sno = sc.sno and sc.cno = course.cno;