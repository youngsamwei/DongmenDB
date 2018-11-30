#ifndef DONGMENSQL_H_
#define DONGMENSQL_H_

#include <stdint.h>

#include "create.h"
#include "sra.h"

#define SQL_NOTVALID (-1)
#define SQL_NULL (0)
#define SQL_INTEGER_1BYTE (1)
#define SQL_INTEGER_2BYTE (2)
#define SQL_INTEGER_4BYTE (4)
#define SQL_TEXT (13)

#define STMT_CREATE (0)
#define STMT_SELECT (1)
#define STMT_INSERT (2)
#define STMT_DELETE (3)

typedef struct Create_s Create_t;
typedef struct SRA_s SRA_t;
typedef struct Insert_s Insert_t;
typedef struct Delete_s Delete_t;


typedef struct dongmensql_statement {

    bool explain;
    char *text;
    uint8_t type;

    union {
        Create_t *create;
        SRA_t *select;
        Insert_t *insert;
        Delete_t *_delete;
    } stmt;
} dongmensql_statement_t;

int dongmensql_parser(const char *sql, dongmensql_statement_t **stmt);

int dongmensql_stmt_print(dongmensql_statement_t *stmt);



#endif /* SQL_TYPES_H_ */
