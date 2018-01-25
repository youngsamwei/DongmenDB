/*
 * commands.h
 *
 *  Created on: Jul 31, 2014
 *      Author: borja
 */

#ifndef COMMANDS_H_
#define COMMANDS_H_

int dongmengdbShellHandleCmd(dongmengdbShellHandleSql_t *ctx, const char *cmd);
int dongmengdbShellHandleSql(dongmengdbShellHandleSql_t *ctx, const char *sql);

struct handler_entry;

typedef int (*handler_function)(dongmengdbShellHandleSql_t *ctx, struct handler_entry *e, const char **tokens, int ntokens);

struct handler_entry
{
    char *name;
    char *help;
    int name_len;
    handler_function func;
};

#define HANDLER_ENTRY(NAME,HELP) { #NAME, HELP, sizeof #NAME - 1, dongmengdbShellHandleCmd_ ## NAME}
#define NULL_ENTRY               { NULL, NULL, 0, NULL }

int dongmengdbShellHandleCmd_open(dongmengdbShellHandleSql_t *ctx, struct handler_entry *e, const char **tokens, int ntokens);
int dongmengdbShellHandleCmd_help(dongmengdbShellHandleSql_t *ctx, struct handler_entry *e, const char **tokens, int ntokens);
int dongmengdbShellHandleCmd_parse(dongmengdbShellHandleSql_t *ctx, struct handler_entry *e, const char **tokens, int ntokens);
int dongmengdbShellHandleCmd_opt(dongmengdbShellHandleSql_t *ctx, struct handler_entry *e, const char **tokens, int ntokens);
int dongmengdbShellHandleCmd_mode(dongmengdbShellHandleSql_t *ctx, struct handler_entry *e, const char **tokens, int ntokens);
int dongmengdbShellHandleCmd_headers(dongmengdbShellHandleSql_t *ctx, struct handler_entry *e, const char **tokens, int ntokens);
int dongmengdbShellHandleCmd_explain(dongmengdbShellHandleSql_t *ctx, struct handler_entry *e, const char **tokens, int ntokens);
int dongmengdbShellHandleCmd_exit(dongmengdbShellHandleSql_t *ctx, struct handler_entry *e, const char **tokens, int ntokens);

#endif /* COMMANDS_H_ */
