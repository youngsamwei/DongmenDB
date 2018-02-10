#ifndef SHELL_H_
#define SHELL_H_

#include <dongmendb.h>
#include <stdbool.h>

typedef enum shell_mode
{
    MODE_LIST          = 0,
    MODE_COLUMN        = 1,
} shell_mode_t;

typedef struct dongmendb_shell_handle_sql
{
    char *dbfile;
    dongmendb *db;

    bool header;
    shell_mode_t mode;

} dongmendb_shell_handle_sql_t;

void dongmendb_shell_init_ctx(dongmendb_shell_handle_sql_t *ctx);
int dongmendb_shell_opendb(dongmendb_shell_handle_sql_t *ctx, char *file);



#endif
