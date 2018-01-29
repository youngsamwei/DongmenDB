/*****************************************************************************
 *
 *																 dongmengdb
 *
 * 简单的sql命令行界面
 *
\*****************************************************************************/
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <dongmengdb.h>
#include <stdio.h>
#include "shell.h"
#include "commands.h"

#define  MAX_CMD 512

int main(int argc, char *argv[]) {
    int opt;
    int rc;
    int verbosity = 0;
    char *command = "select sno from student;";
    dongmengdb_shell_handle_sql_t shell_ctx;

    dongmengdb_shell_init_ctx(&shell_ctx);

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
                printf("Usage: dongmengdb [-c COMMAND] [DATABASE]\n");
                exit(0);
            default:
                printf("ERROR: Unknown option -%c\n", opt);
                exit(-1);
        }


    char cmdstring[MAX_CMD];
    int n;
    while (1) {
        printf("*dongmengdb*>");

        if ((n = read(0, cmdstring, MAX_CMD)) < 0) {
            printf("read error");
        }

        int wordcount = 0;
        while(cmdstring[wordcount] != '\n'){
            wordcount++;
        }
        cmdstring[wordcount] = '\0';
        /*is a buildin command*/
        /*direct return*/

        if (cmdstring) {
            dongmengdb_shell_handle_cmd(&shell_ctx, cmdstring);
        }

    }

//    optind= 0;
//    char *file = "demo.db";
//    if (optind < argc)
//    {
//        rc = dongmengdb_shell_opendb(&shell_ctx, file /*argv[optind]*/);
//        if(rc)
//        {
//            fprintf(stderr, "ERROR: Could not open file %s or file is not well formed.\n", argv[optind]);
//            exit(1);
//        }
//    }

    /* If a command was specified as an argument, we just run that.
     * Otherwise, we start the shell. */



    return 0;
}
