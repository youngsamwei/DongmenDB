#include <string.h>
#include "shell.h"

void dongmengdb_shell_init_ctx(dongmengdb_shell_handle_sql_t *ctx)
{
    ctx->db = NULL;
    ctx->dbfile = NULL;

    ctx->header = false;
    ctx->mode = MODE_LIST;
}




