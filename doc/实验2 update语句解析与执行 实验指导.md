# 实验 2 update 语句解析与执行

## 2.1 update 语句的解析

解析 sql 语句需要了解一些分词器和解析器

### 2.1.1 分词器（tokenizer)

分词器（tokenizer）的主要作用就是将 sql 语句拆分为 token（词），这样我们就可以逐词进行解析。分词器不仅会将 sql 语句拆分成词，还会判断词的属性（ `TokenType` ）:
词的全部属性（ `TokenType` ）定义在 `/include/parser/tokenizer.h` 中

```cpp
typedef enum TokenType_ {
    TOKEN_OPEN_PAREN = 0,    //左括号
    TOKEN_CLOSE_PAREN,       //右括号
    TOKEN_POWER,             //
    TOKEN_PLUS,              //加号
    TOKEN_MINUS,             //减号
    TOKEN_DIVIDE,            //除
    TOKEN_MULTIPLY,          //乘
    TOKEN_LT,                //less-than operator 小于
    TOKEN_GT,                //大于
    TOKEN_EQ,                //等于
    TOKEN_NOT_EQUAL,         //不等于
    TOKEN_LE,                //小于等于 less-than-or-equal-to operator"
    TOKEN_GE,                //大于等于
    TOKEN_IN,                //in运算
    TOKEN_LIKE,              //like运算
    TOKEN_AND,
    TOKEN_OR,
    TOKEN_NOT,               //匹配not
    TOKEN_ASSIGNMENT,        //赋值
    TOKEN_FUN,               //函数
    TOKEN_COMMA,             /*以上是操作符，在表达式解析时使用*/ //逗号

    TOKEN_INVALID, //"invalid token"
    TOKEN_RESERVED_WORD,  //sql中的保留字，比如create,set,where 等等
    TOKEN_WORD,           //非保留字的词
    TOKEN_UNENDED_SRING,  //没有结尾的字符串
    TOKEN_STRING,         //字符串
    TOKEN_MOD,            //模
    TOKEN_INCOMPLETE_CHAR,//不完整的字符
    TOKEN_CHAR,           //字符
    TOKEN_INVALID_CHAR,   //非法字符
    TOKEN_SEMICOLON,      //分号
    TOKEN_EXP_FLOAT,
    TOKEN_FLOAT,
    TOKEN_OCTAL,               //"octal integer"
    TOKEN_HEX,                 //"hexadecimal integer"
    TOKEN_DECIMAL,            //"decimal integer"
    TOKEN_ZERO,               //"zero integer"
    TOKEN_NULL                 //匹配null
} TokenType;
```

```cpp
//sql中的保留字
const char *reservedWords[] = {
            "select", "from", "where", "order", "by", "group", "create", "table", "index",
            "and", "not", "or", "null",            "like", "in", "grant", "integer", "int",
            "char", "values", "insert", "into", "update", "delete", "set", "on",
            "user", "view", "rule", "default", "check", "between", "trigger", "primary", "key", "foreign"
    };
```

---

### 2.1.2 解析器（parser）

解析器作用主要是获取 token(词)，然后对 token(词)进行解析，主要的函数在 `src/parser/parser.cpp` 中实现：

`int matchToken(Parser *parser, TokenType type, char *text)`//**常用函数**
对解析器中 token(词）的`TokenType`和`text`进行检查，如果匹配成功，就**直接获取下一个词放入解析器中**，否则输出错误信息，返回 NULL，不对解析器中的词修改

`Token *parseNextToken(Parser *parser)`
首先判断当前解析器中的词是否为 NULL,如果是，就获取下一个 token;否则就返回当前词

`Token *parseEatToken(Parser *parser)`
在解析完当前词之后，将当前解析器中的 token(词)置为 NULL

`Token *parseEatAndNextToken(Parser *parser)`
直接获取下一个词，不对当前解析器中的词进行判断

`void *parseError(Parser *parser, char *message)`
将解析状态修改为解析失败( `PARSER_WRONG` ),返回 NULL

### 2.1.3 解析 update 语句(parse_sql_stmt_update)

`以update student set sage=20[,fieldname=expr] where sage=22`为例，其中 `update` `set` `where` 均为保留字类型

```cpp
class sql_stmt_update  : public SQLStatement {
public:
    char *tableName;    //保存上面update语句的student
    vector<char*> fields; //set fields 被更新的字段列表,保存上面update语句的sage
    vector<Expression*> fieldsExpr;  //set fields expression 新值(或表达式)列表,保存上面update语句的20
    SRA_t *where;   //保存上面update语句的 sage=22
} ;
```

其中， `where` 是一个 `SRA_t` （即 `SRA_s` ） 结构体的指针，该结构体定义在 `include/relationalalgebra/sra.h` 中，这个文件存放 SRA（sugar relation algebra）的定义和操作，这是一种关系代数的表示方法。

```cpp
struct SRA_s {
    enum SRAType t;  // Type of SRA_s
    union {
        SRA_Table_t table;
        SRA_Select_t select;
        // others...
    };
};

// Constructors
SRA_t *SRATable(TableReference_t *ref);
SRA_t *SRASelect(SRA_t *sra, Expression *cond);
// others...
```

`where` 语句中，会用到两种 `SRA_t` ： `SRA_TABLE` 和 `SRA_SELECT` ，其构造函数已在上面列出。

简要流程：

1. 首先匹配 `update` ：使用 `matchToken(TOKEN_RESERVED_WORD, "update")` 匹配
2. 获取表名 `tableName` ：使用 if 判断是否是 `TOKEN_WORD` 类型，如果是，获得表名，并将表名包裹在 `SRA_t` 中（ `SRA_TABLE` 类型）作为 `sql_stmt_update->where` 的值；给字符串指针开空间的时候可以使用 `new_id_name()` 函数
3. 匹配 `set` ：使用 `matchToken(TOKEN_RESERVED_WORD,"set")`
4. 循环获取更新的表达式：获取字段名（ `TOKEN_WORD` 类型），存入 `fields` 中；识别并跳过 `=` （ `TOKEN_EQ` 类型）；使用 `parseExpressionRD()` 函数获得新值（或表达式）,存入 `fieldsExpr` 中即可获得一个完整的表达式；请注意设置循环停止条件
5. 匹配 `where` ： `matchToken(TOKEN_RESERVED_WORD,"where")` ，注意不是所有 `update` 语句都伴随一个 `where` 表达式
6. 使用 `parseExpressionRD()` 函数获取值（或者表达式），并包裹在 `SRA_t` 中（ `SRA_SELECT` 类型），作为 `sql_stmt_update->where` 的值。（语句若不伴随 `where` 则仅需构造 `SRA_TABLE` ，具体实现可参考 `exp_01_stmt_parser/exp_01_03_select.cpp` ）
7. 创建 `sql_stmt_update` 类型的指针，分配内存空间并对各字段进行赋值，返回该指针

常用函数：

- `push_back(elem);` //向 vector 尾部加入数据
- `Expression *Parser::parseExpressionRD();` //递归下降法解析表达式 详情看 `src/parser/parser.cpp` 中
- `SRA_t *SRATable(TableReference_t *ref);` // `SRA_TABLE` 构造函数
- `SRA_t *SRASelect(SRA_t *sra, Expression *cond);` // `SRA_SELECT` 构造函数
- `Expression *expression_print(Expression *expr, char *desc);`  //输出 `Expression` 对象的数据（调试用）
- `void SRA_print(SRA_t *sra);`  //输出 `SRA_t` 中的数据（调试用）

## 2.2 update 语句执行

### 2.2.1 物理操作

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

### 2.2.2 实现

函数的具体实现在 `src/physicalplan/` 下 `Scan` 的多个子类中

主要思路：

1. 使用 `SRA_t` 类型的 `where` 构造执行计划( `Scan` 对象)；
   （使用 `generateScan(DongmenDB *db, SRA_t *sra, Transaction *tx)` 函数构造, 在 `src\physicalplan\ExecutionPlan.cpp` 中实现）
2. 遍历计划
3. 在遍历计划过程的循环中，计算表达式的值，计算需要修改字段的偏移量
   （使用 `Expression * evaluateExpression(Expression *expr, Scan* scan, variant *var)` 函数，在 `src\physicalplan\Scan.cpp` 中实现）
4. 使用 `setInt(string tableName, string fieldName, int value)` 或 `setString(string tableName, string fieldName, string value)` 修改；
5. 在循环中更新修改的记录条数，作为最终的返回值。

针对 `Integer` 和 `String` 类型的字段修改数据主要是用两个函数 `setInt()` 、 `setString()` ，你可以

- 通过 `scan->getField(string tableName, string fieldName)->type` 得到字段类型。
- 执行 `evaluateExpression(Expression *expr, Scan* scan, variant *var)` 后，通过 `var->type` 得到表达式的类型。

---

文档主要作者： 冯晓坤 毛锟 石乐山 等。 如果愿意也像他们一样为大家制作技术文档，请联系谭婷加入 QQ 群。
