#ifndef __COMMON_H_
#define __COMMON_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdbool.h>
#ifdef __cplusplus
extern "C" {
#endif

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
    DATA_TYPE_BOOLEAN
};

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
#ifdef __cplusplus
}
#endif

#endif
