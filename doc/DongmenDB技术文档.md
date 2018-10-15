
# DongmenDB技术文档

## 1 概述
DongmenDB是一个用于教学的关系数据库管理系统。
DongmenDB实现关系数据库管理系统的基本功能：
1）基本SQL解析与执行：包括select，insert，update，delete，create table等语句；
2）安全性管理；
3）完整性管理；
4）索引管理。

从技术实现角度，DongmenDB包含以下模块：
1）shell：与用户交互的cmd界面。
2）dongmensql：定义了sql语句以及相关的数据结构；
3）parser：词法解析和语法解析；
4）dongmendb：定义了系统级别的模块；
5）physicalplan：实现物理计划；
6）utils：定义了常用的基础数据结构：列表arraylist，哈希hashmap，堆栈stack等。

## 2 shell

## 3 语言处理层dongmendql
### 3.1 create
### 3.2 insert
### 3.3 select
### 3.4 delete
### 3.5 column
### 3.6 relational algebra
### 3.7 关系代数查询优化

## 4 parser
### 4.1 词法分析器tokenizer
### 4.2 语法分析器parser
### 4.3 语句解析statement
### 4.4 表达式解析expression

## 5 系统管理dongmendb
dongmendb的数据库以文件夹为单位，文件夹名称作为数据库名称，每个表保存在独立的文件中，以扩展名tbl作为结尾。
系统表也保存在独立的文件中，也以扩展名tbl作为结尾。

### 5.1 dongmendb
实现数据库的管理：数据库打开，关闭等。

### 5.2 数据存取层recordfile
实现数据存取层的逻辑数据文件的管理：逻辑文件的打开，关闭，以记录为单位进行读写操作等；

### 5.3 数据存储层filemanager
实现数据存储层的物理数据文件的管理：物理文件的打开，关闭，以块为单位的外存读写操作等。

### 5.4 完整性管理integrity manager
实现数据库的完整性管理：约束的定义，检查与处理。
5.4.1 约束的系统表结构
系统表integrity(name,tablename,typename, expression),
name:约束名称；
tablename：表名称；
fieldnames:字段名称列表；
reftablename:外键中被引用的表名称；其他约束为空
reffieldnames：外键中被引用的字段名称；其他约束为空
typename：约束类型名称；（primary key， foreign key，check，null，default）
expression：约束定义；(check时为布尔表达式，null时为null或notnull，default为表达式，主外键时为空)

5.4.2 约束的检查
在执行insert，update，delete时检查约束是否被满足。

5.4.3 约束的处理
如果不满足约束，则拒绝执行该语句。

5.4.4 约束的管理
在create table中定义约束；
使用alter table 增加，删除，修改约束；

### 5.5 安全性管理security manager
实现数据库的安全性管理：权限的定义，检查与处理。
5.5.1 权限的系统表结构
系统表security(rightname，tablename，username）
rightname：权限名称；
tablename：表名称；
username：用户名称；

用户表user（username，password，status）
username：用户名；
password：密码；
status：状态；

5.5.2 权限的检查
在执行select，insert，update，delete等语句之前检查该用户是否拥有目标表上的相应权限。

5.5.3 权限的处理
如果不满足权限要求，则拒绝执行该语句。

5.5.4 用户与权限的管理
使用create user；drop user管理用户；
使用grant ，revoke语句管理权限。

### 5.6 元数据管理metadata manager
实现系统表的管理：表信息，字段信息等。

### 5.7 日志管理log manager
实现日志管理：日志读写。

### 5.8 缓冲区管理buffer manager
实现数据库的缓冲区管理：以块为单位管理缓冲区。

### 5.9 事务管理transaction manager

### 5.10 索引管理index manager
5.10.1 索引的存储
索引表index(tablename, fieldname, asc, order)
tablename:表名称；
fieldname：字段名称；
asc：是否升序；
order：若多字段索引，说明字段顺序；

5.10.2 索引的维护
在insert，update，delete时，更新索引表中的数据；

5.10.3 索引相关操作的实现
实现table scan，index join等物理操作；

5.10.4 索引的定义
使用create index，drop index定义索引。


### 5.11 封锁管理lock manager

## 6 实现物理计划physicalplan
### 6.1 定义物理计划
### 6.2 表扫描table scan
### 6.3 嵌套循环 nest loop
### 6.4 投影 project
### 6.5 选择 select
### 6.6 排序合并连接
### 6.7 索引连接
### 6.8 哈希连接

## 7 辅助数据结构utils
### 7.1 arraylist
### 7.2 hashmap
### 7.3 opstack