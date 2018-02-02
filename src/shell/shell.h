#ifndef SHELL_H_
#define SHELL_H_

#include <dongmengdb.h>
#include <stdbool.h>

typedef enum shell_mode
{
    MODE_LIST          = 0,
    MODE_COLUMN        = 1,
} shell_mode_t;

typedef struct dongmengdb_shell_handle_sql
{
    char *dbfile;
    dongmengdb *db;

    bool header;
    shell_mode_t mode;

} dongmengdb_shell_handle_sql_t;

void dongmengdb_shell_init_ctx(dongmengdb_shell_handle_sql_t *ctx);
int dongmengdb_shell_opendb(dongmengdb_shell_handle_sql_t *ctx, char *file);



#endif
