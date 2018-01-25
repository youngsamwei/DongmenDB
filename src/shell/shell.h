#ifndef SHELL_H_
#define SHELL_H_

#include <dongmengdb.h>
#include <stdbool.h>

typedef enum shellMode
{
    MODE_LIST          = 0,
    MODE_COLUMN        = 1,
} shellMode_t;

typedef struct dongmengdbShellHandleSql
{
    char *dbfile;
    dongmengdb *db;

    bool header;
    shellMode_t mode;

} dongmengdbShellHandleSql_t;

void dongmengdbShellInitCtx(dongmengdbShellHandleSql_t *ctx);
int dongmengdbShellOpenDB(dongmengdbShellHandleSql_t *ctx, char *file);

#endif
