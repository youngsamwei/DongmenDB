.open demodb
create table student(sno char(10), sname char(20), sage integer, ssex char(10));
create table course(cno char(10), cname char(20), credit integer);
create table sc(sno char(10), cno char(10), grade integer);

.open demodb


insert into student(sno, sname, sage, ssex) values("2012010101","tom simith",20,"male");
insert into student(sno, sname, sage, ssex) values("2012010102","jack simith",21,"male");
insert into student(sno, sname, sage, ssex) values("2012010103","li simith",22,"male");
insert into student(sno, sname, sage, ssex) values("2012010104","john simith",23,"male");
insert into student(sno, sname, sage, ssex) values("2012010105","dan simith",24,"female");
insert into student(sno, sname, sage, ssex) values("2012010106","mary simith",25,"female");
insert into student(sno, sname, sage, ssex) values("2012010107","jene simith",26,"female");
insert into student(sno, sname, sage, ssex) values("2012010108","di simith",27,"female");
insert into student(sno, sname, sage, ssex) values("2012010108","sam simith",27,"male");

insert into course(cno, cname, credit) values("c001", "data structure", 3);
insert into course(cno, cname, credit) values("c002", "c", 4);
insert into course(cno, cname, credit) values("c003", "database", 3);
insert into course(cno, cname, credit) values("c004", "math", 6);

insert into sc(sno, cno, grade) values("2012010101","c001",80);
insert into sc(sno, cno, grade) values("2012010102","c001",45);
insert into sc(sno, cno, grade) values("2012010103","c001",75);
insert into sc(sno, cno, grade) values("2012010104","c001",77);
insert into sc(sno, cno, grade) values("2012010105","c001",67);
insert into sc(sno, cno, grade) values("2012010106","c001",87);
insert into sc(sno, cno, grade) values("2012010107","c001",90);
insert into sc(sno, cno, grade) values("2012010101","c002",96);
insert into sc(sno, cno, grade) values("2012010102","c002",74);
insert into sc(sno, cno, grade) values("2012010103","c002",86);
insert into sc(sno, cno, grade) values("2012010104","c003",95);
insert into sc(sno, cno, grade) values("2012010105","c003",89);
insert into sc(sno, cno, grade) values("2012010106","c003",48);
insert into sc(sno, cno, grade) values("2012010107","c003",80);


.exit


.open demodb
select student.sno,sname from student, sc, course where student.sno = sc.sno and sc.cno = course.cno and grade > 80
select student.sno,sname, cname, grade from student, sc, course where student.sno = sc.sno and sc.cno = course.cno and grade > 80


.open demodb
select sage + 1, sno from student

.open demodb
select student.sno, sc.cno,sname,grade, cname from student, sc, course where student.sno = sc.sno and sc.cno = course.cno

.open demodb
select sno,cno,grade, cname from sc, course where sc.cno = course.cno and grade > 80
select sno, cno,grade from sc where grade > 80


select student.*, grade from student, sc where student.sno = sc.sno
select student.*, sc.* from student, sc where student.sno = sc.sno

select student.* from student, sc, course where student.sno = sc.sno and sc.cno = course.cno
select student.*, course.* from student, sc, course where student.sno = sc.sno and sc.cno = course.cno
select sc.*, course.* from student, sc, course where student.sno = sc.sno and sc.cno = course.cno
select student.*, sc.*, course.* from student, sc, course where student.sno = sc.sno and sc.cno = course.cno

/* 对系统表的查询 */
select * from tablecat
select * from fieldcat


update student set sname = "tom" where sno = "001"