# 实验2 update语句解析与执行

## 2.1 update 语句的解析

解析sql语句需要了解一些分词器和解析器
### 2.1.1 分词器（tokenizer)
分词器（tokenizer）的主要作用就是将sql语句拆分为token（词），这样我们就可以逐词进行解析。分词器不仅会将sql语句拆分成词，还会判断词的属性（`TokenType`）: 
词的全部属性（`TokenType`）定义在 `/include/parser/tokenizer.h` 中  
```
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

```
//sql中的保留字
const char *reservedWords[] = {
            "select", "from", "where", "order", "by", "group", "create", "table", "index", "and", "not", "or", "null",
            "like", "in", "grant", "integer", "int", "char", "values", "insert", "into", "update", "delete", "set", "on",
            "user", "view", "rule", "default", "check", "between", "trigger", "primary", "key", "foreign"
    };
```

---------------------------------------
### 2.1.2 解析器（parser）
解析器作用主要是获取token(词)，然后对token(词)进行解析，主要的函数在`src/parser/parser.c`中实现：  


`int matchToken(ParserT *parser, TokenType type, char *text)`//**常用函数**
对解析器中token(词）的`TokenType`和`text`进行检查，如果匹配成功，就**直接获取下一个词放入解析器中**，否则输出错误信息，返回NUL，不对解析器中的词修改    

`TokenT *parseNextToken(ParserT *parser)`   
首先判断当前解析器中的词是否为NULL,如果是，就获取下一个token;否则就返回当前词   

`TokenT *parseEatToken(ParserT *parser)`  
在解析完当前词之后，将当前解析器中的token(词)置为NULL   

`TokenT *parseEatAndNextToken(ParserT *parser)`  
直接获取下一个词，不对当前解析器中的词进行判断    

`void *parseError(ParserT *parser, char *message)`  
将解析状态修改为解析失败(`PARSER_WRONG`),返回NULL   



### 2.1.3 解析 update 语句(parse_sql_stmt_update)
`以update student set sage=20[,fieldname=expr] where sage=22`为例，其中update set where 全都是保留字类型
```
typedef struct sql_stmt_update_ {
    char *tableName;         //保存上面update语句的student
    arraylist *fields; //set fields 被更新的字段列表。保存上面update语句的sage
    arraylist *fieldsExpr;  //set fields expression 新值(表达式)列表 。保存上面update语句的20
    SRA_t *where;  //保存上面update语句的 sage = 22
} sql_stmt_update;
```

简要流程：
1. 首先匹配 `update`, 使用`matchToken(parser,TOKEN_RESERVED_WORD,"update")`匹配
2. 获取表名 使用if判断是否是TOKEN_WORD类型，如果是，获得表名；给字符串指针开空间的时候可以使用`new_id_name()`函数
3. 匹配 `set` 使用matchToken(parser,TOKEN_RESERVED_WORD,"set")
4. 使用循环获得字段名（TOKEN_WORD类型），放入一个fields(arraylist类型)中；循环获得值（或者表达式）`parseExpressionRD(parse)`函数,fieldsExpr(arraylist类型)中 
5. 匹配 `where` ,matchToken(parser,TOKEN_RESERVED_WORD,"where")
6. 获得字段名(TOKEN_WORD类型)，使用`parseExpressionRD(parse)`获取字段值（或者表达式）
7. 创建`sql_stmt_update`指针，开空间，对各字段进行赋值，返回`sql_stmt_update`结构体 


常用函数：
`arraylist_create()` //创建一个arraylist
`int arraylist_add(arraylist *list, void *element)` //向 arraylist中添加元素
`Expression *parseExpressionRD(ParserT *parser) ` //递归下降法解析表达式 详情看 src/parser/expression.c中




## 2.2 update语句执行

### 2.2.1 物理操作
首先创建一个表扫描计划，然后是用physical_scan 类型中的next函数迭代数据库数据条目，对满足条件的条目进行修改。   


physical_scan 结构体中有很多函数指针，针对不同的物理计划，这些函数指针指向了不同功能的函数： 
```
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
### 2.2.2 实现
函数的具体实现在`src/physicalplan/physical_scan_*.c`中分别实现（ps:有的不需要实现，所以直接赋值为NULL。因为具体的实现不同，所以函数参数也不同）  

主要思路：
1. 使用 where条件的SRA_t构造执行计划；
（参考src\shell\shell.c中的dongmendb_shell_handle_select_table函数中的写法）
2. 遍历计划（循环）
（参考src\shell\shell.c中的dongmendb_shell_handle_select_table函数中的写法）
3. 在遍历计划过程的循环中，计算表达式的值，计算需要修改字段的偏移量，使用setInt或setString修改；
(参考src\physicalplan\physicalplan.c中的plan_execute_insert函数的实现)


针对Integer和String类型的字段修改数据主要是用两个函数`setInt`,`setString`,具体用哪个，未完待续
