#ifndef __DELETE_H_
#define __DELETE_H_

#include "common.h"
#ifdef __cplusplus
extern "C" {
#endif

typedef struct Delete_s {
    char *table_name;
    Expression *where;
} Delete_t;

Delete_t *Delete_make(const char *table_name, Expression *where);

void deleteDelete(Delete_t *del);

void Delete_print(Delete_t *del);

void Delete_free(Delete_t *del);

#ifdef __cplusplus
}
#endif

#endif
