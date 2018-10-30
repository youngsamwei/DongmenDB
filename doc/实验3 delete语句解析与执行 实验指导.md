# 实验 3 delete 语句解析与执行

## 3.1 delete 语句的解析

解析 sql 语句需要了解一些分词器和解析器


简要流程：

1. 首先匹配 `delete`, 使用`matchToken(parser,TOKEN_RESERVED_WORD,"delete")`匹配
2. 匹配 `from` 使用 matchToken(parser,TOKEN_RESERVED_WORD,"from")
3. 获取表名 使用 if 判断是否是 TOKEN_WORD 类型，如果是，获得表名；给字符串指针开空间的时候可以使用`new_id_name()`函数
4. 匹配 `where`，matchToken(parser,TOKEN_RESERVED_WORD,"where")，注意不是所有`delete`语句都伴随一个`where`表达式
6. 使用`parseExpressionRD(parse)`获取值（或者表达式），并包裹在`SRA_t`中（使用`SRA_SELECT`构造），
作为`sql_stmt_delete->where`的值。（语句若不伴随`where`表达式则需要以一个`SRA_TABLE`代替，具体实现可参考 exp_01_stmt_parser/exp_01_03_select.c）
7. 创建`sql_stmt_delete`指针，开空间，对各字段进行赋值，返回`sql_stmt_delete`结构体

常用函数：

- `arraylist_create()` //创建一个 arraylist
- `int arraylist_add(arraylist *list, void *element)` //向 arraylist 中添加元素
- `Expression *parseExpressionRD(ParserT *parser)` //递归下降法解析表达式 详情看 src/parser/expression.c 中
- `SRA_t *SRATable(TableReference_t *ref);` // `SRA_t`构造函数
- `SRA_t *SRASelect(SRA_t *sra, Expression *cond);` // `SRA_t`构造函数

## 3.2 delete 语句执行

### 3.2.1 物理操作

首先创建一个表扫描计划，然后是用 physical_scan 类型中的 next 函数迭代数据库数据条目，对满足条件的条目进行修改。

physical_scan 结构体中有很多函数指针，针对不同的物理计划，这些函数指针指向了不同功能的函数：

```c
//include/physicalplan/physicalscan.h中
typedef struct physical_scan_ {
    scan_type scanType;
    union {
        physical_scan_table *physicalScanTable;
        physical_scan_join_nest_loop *physicalScanJoinNestLoop;
        physical_scan_select *physicalScanSelect;
        physical_scan_project *physicalScanProject;
    };
    physical_scan_before_first beforeFirst;
    physical_scan_next next;
    physical_scan_close close;
    physical_scan_get_val_by_index getValByIndex;
    physical_scan_get_int_by_index getIntByIndex;
    physical_scan_get_string_by_index getStringByIndex;
    physical_scan_get_val getVal;
    physical_scan_get_int getInt;
    physical_scan_get_string getString;
    physical_scan_has_field hasField;
    physical_scan_get_field getField;
    physical_scan_get_fields_name getFieldsName;
    physical_scan_set_int setInt;
    physical_scan_set_string setString;
    physical_scan_delete deleterec;
    physical_scan_insert insert;
    physical_scan_get_rid getRid;
    physical_scan_moveto_rid movetoRid;
} physical_scan;
```

### 3.2.2 实现

函数的具体实现在`src/physicalplan/physical_scan_*.c`中分别实现（ps:有的不需要实现，所以直接赋值为 NULL。因为具体的实现不同，所以函数参数也不同）

主要思路：

1. 使用 where 条件的 SRA_t 构造执行计划；
   （参考 src\shell\shell.c 中的 dongmendb_shell_handle_select_table 函数中的写法）
2. 遍历计划（循环）
   （参考 src\shell\shell.c 中的 dongmendb_shell_handle_select_table 函数中的写法）
3. 在遍历计划过程的循环中，删除记录
   调用scan->deleterec(scan);删除记录
4. 在循环中更新删除的记录条数，作为最终的返回值。


---

