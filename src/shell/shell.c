#include <string.h>
#include "shell.h"

void dongmengdbShellInitCtx(dongmengdbShellHandleSql_t *ctx)
{
    ctx->db = NULL;
    ctx->dbfile = NULL;

    ctx->header = false;
    ctx->mode = MODE_LIST;
}

int dongmengdbShellOpenDB(dongmengdbShellHandleSql_t *ctx, char *file)
{
    int rc;

    rc = dongmengdbOpen(file, &ctx->db);

    if (rc != DONGMENGDB_OK)
        return 1;

    ctx->dbfile = strdup(file);

    return 0;
}




