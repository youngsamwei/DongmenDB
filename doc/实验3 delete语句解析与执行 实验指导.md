# 实验 3 delete 语句解析与执行

## 3.1 delete 语句的解析

解析 sql 语句需要了解一些分词器和解析器


简要流程：

1. 首先匹配 `delete` ：使用 `matchToken(TOKEN_RESERVED_WORD, "delete")` 匹配
2. 匹配 `from` ：使用 `matchToken(TOKEN_RESERVED_WORD,"from")` 匹配
3. 获取表名：使用 if 判断是否是 `TOKEN_WORD` 类型，如果是，获得表名；给字符串指针开空间的时候可以使用 `new_id_name()` 函数
4. 匹配 `where` ： `matchToken(TOKEN_RESERVED_WORD,"where")` ，注意不是所有 `delete` 语句都伴随一个 `where` 表达式
6. 使用 `parseExpressionRD()` 获取值（或者表达式），并包裹在 `SRA_t` 中（ `SRA_SELECT` 类型），作为 `sql_stmt_delete->where` 的值。（语句若不伴随 `where` 则仅需构造 `SRA_TABLE` ，具体实现可参考 `exp_01_stmt_parser/exp_01_03_select.cpp`）
7. 创建 `sql_stmt_delete` 类型的指针，分配内存空间并对各字段进行赋值，返回该指针

常用函数：

- `push_back(elem);` //向 vector 尾部加入数据
- `Expression *Parser::parseExpressionRD();` //递归下降法解析表达式 详情看 `src/parser/parser.cpp` 中
- `SRA_t *SRATable(TableReference_t *ref);` // `SRA_TABLE` 构造函数
- `SRA_t *SRASelect(SRA_t *sra, Expression *cond);` // `SRA_SELECT` 构造函数
- `void SRA_print(SRA_t *sra);`  //输出 `SRA_t` 中的数据（调试用）

## 3.2 delete 语句执行

### 3.2.1 物理操作

首先创建一个表扫描计划，然后用 `Scan` 类中的 `next()` 函数迭代数据库数据条目，对满足条件的条目进行修改。

`Scan` 类中有针对不同的物理计划的虚拟函数：

```cpp
class Scan {
public:
    DongmenDB *m_db;
    Transaction *m_tx;

    virtual int beforeFirst()=0;
    virtual int next()=0;
    virtual int close()=0;
    virtual variant *getValueByIndex(int index)=0;
    virtual int getIntByIndex(int index)=0;
    virtual string getStringByIndex(int index)=0;
    virtual int getInt(string tableName, string fieldName) = 0;
    virtual variant* getValue(string fieldName)=0;
    virtual string getString(string tableName, string fieldName)=0;
    virtual int hasField(string tableName,string fieldName)=0;
    virtual FieldInfo * getField(string tableName, string fieldName)=0;
    virtual  vector<char*> getFieldsName(string tableName)=0;
    virtual int setInt(string tableName, string fieldName, int value)=0;
    virtual int setString(string tableName, string fieldName, string value)=0;
    virtual int deleteRecord()=0;
    virtual int insertRecord()=0;
    virtual int getRID(RecordID *recordID)=0;
    virtual int moveTo(RecordID *recordID)=0;
    Expression * evaluateExpression(Expression *expr, Scan* scan, variant *var);

};
```

### 3.2.2 实现

函数的具体实现在 `src/physicalplan/` 下 `Scan` 的多个子类中

主要思路：

1. 使用 `SRA_t` 类型的 `where` 构造执行计划( `Scan` 对象)；
2. 遍历计划
3. 在遍历计划过程的循环中，调用 `scan->deleteRecord();` 删除记录
4. 在循环中更新删除的记录条数，作为最终的返回值。


---

文档主要作者： 冯晓坤 毛锟 石乐山 等。 如果愿意也像他们一样为大家制作技术文档，请联系谭婷加入 QQ 群。