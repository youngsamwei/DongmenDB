#include <string.h>
#include "shell.h"

void dongmengdb_shell_init_ctx(dongmengdb_shell_handle_sql_t *ctx)
{
    ctx->db = NULL;
    ctx->dbfile = NULL;

    ctx->header = false;
    ctx->mode = MODE_LIST;
}

int dongmengdb_shell_opendb(dongmengdb_shell_handle_sql_t *ctx, char *file)
{
    int rc;

    rc = dongmengdb_open(file, ctx->db);

    if (rc != DONGMENGDB_OK)
        return 1;

    ctx->dbfile = strdup(file);

    return 0;
}




