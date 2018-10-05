/*****************************************************************************
 *
 *																 dongmendb
 *
 * 简单的sql命令行界面
 *
\*****************************************************************************/
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <dongmendb/dongmendb.h>
#include <stdio.h>
#include "shell/shell.h"

#define  MAX_CMD 512

int main(int argc, char *argv[]) {
    int opt;
    int rc;
    int verbosity = 0;
    char *command = "select sno from student;";
    dongmendb_shell_handle_sql_t shell_ctx;

    dongmendb_shell_init_ctx(&shell_ctx);

    /* Process command-line arguments */
    while ((opt = getopt(argc, argv, "c:vh")) != -1)
        switch (opt) {
            case 'c':
                command = strdup(optarg);
                break;
            case 'v':
                verbosity++;
                break;
            case 'h':
                printf("Usage: dongmendb [-c COMMAND] [DATABASE]\n");
                exit(0);
            default:
                printf("ERROR: Unknown option -%c\n", opt);
                exit(-1);
        }


    char cmdstring[MAX_CMD];
    int n;

    setbuf(stdout,0);
    setbuf(stderr,0);
    while (1) {
        printf("\ndongmendb>");

        if ((n = read(0, cmdstring, MAX_CMD)) < 0) {
            printf("read error");
        }

        int wordcount = 0;
        while(cmdstring[wordcount] != '\n'){
            wordcount++;
        }
        cmdstring[wordcount] = '\0';

        if (cmdstring) {
            dongmendb_shell_handle_cmd(&shell_ctx, cmdstring);
        }

    }

    return 0;
}
