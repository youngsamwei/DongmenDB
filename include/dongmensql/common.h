#ifndef __COMMON_H_
#define __COMMON_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdbool.h>
#include <ostream>

typedef enum sql_statement_type_ {
    SQL_CREATE_DATABASE,
    SQL_CREATE_TABLE,
    SQL_CREATE_VIEW,
    SQL_CREATE_INDEX,
    SQL_CREATE_USER,
    SQL_CREATE_ROLE,
    SQL_DROP,
    SQL_SELECT,
    SQL_INSERT,
    SQL_UPDATE,
    SQL_DELETE,
    SQL_GRANT,
    SQL_REVOKE,
    SQL_DENY
} sql_statement_type;

enum query_type {
    SELECT_Q, CREATE_T_Q, CREATE_I_Q, INSERT_Q, DELETE_Q
};

typedef struct Query_s {
    enum query_type t;
    union {
        struct SRA_s *sra;
        struct Table_s *table;
        struct Index_s *index;
        struct Insert_s *insert;
        struct Delete_s *del;
    };
} Query_t;

enum data_type {
    DATA_TYPE_INT,
    DATA_TYPE_DOUBLE,
    DATA_TYPE_CHAR,
    DATA_TYPE_TEXT,
    DATA_TYPE_BOOLEAN,
    DATA_TYPE_NULL
};

/**
 * 定义物理计划的数据结构和接口函数.
 */
typedef struct variant_ {
    enum data_type type;
    union {
        int intValue;
        char *strValue;
        int booleanValue;
    };
} variant;

typedef struct StrList_t {
    char *str;
    struct StrList_t *next;
} StrList_t;

char *typeToString(enum data_type type, char *buf);
StrList_t *StrList_makeWithNext(const char *str, StrList_t *next);
StrList_t *StrList_make(char *str);
StrList_t *StrList_append(StrList_t *list1, StrList_t *list2);
void StrList_print(StrList_t *list);
void StrList_free(StrList_t *list);
void upInd(void);
void downInd(void);
void indent_print(const char *format, ...);

void Query_free(Query_t *query);

std::ostream &operator<<(std::ostream &os, StrList_t *col_list);

#endif
