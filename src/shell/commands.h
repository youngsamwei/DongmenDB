/*
 * commands.h
 *
 *  Created on: Jul 31, 2014
 *      Author: borja
 */

#ifndef COMMANDS_H_
#define COMMANDS_H_


struct handler_entry;

typedef int (*handler_function)(dongmendb_shell_handle_sql_t *ctx, struct handler_entry *e, const char **tokens, int ntokens);

struct handler_entry
{
    char *name;
    char *help;
    int name_len;
    handler_function func;
};

#define HANDLER_ENTRY(NAME,HELP) { #NAME, HELP, sizeof #NAME - 1, dongmendb_shell_handle_cmd_ ## NAME}
#define NULL_ENTRY               { NULL, NULL, 0, NULL }

int dongmendb_shell_handle_cmd_open(dongmendb_shell_handle_sql_t *ctx, struct handler_entry *e, const char **tokens, int ntokens);
int dongmendb_shell_handle_cmd_help(dongmendb_shell_handle_sql_t *ctx, struct handler_entry *e, const char **tokens, int ntokens);
int dongmendb_shell_handle_cmd_parse(dongmendb_shell_handle_sql_t *ctx, struct handler_entry *e, const char **tokens, int ntokens);
int dongmendb_shell_handle_cmd_opt(dongmendb_shell_handle_sql_t *ctx, struct handler_entry *e, const char **tokens, int ntokens);
int dongmendb_shell_handle_cmd_mode(dongmendb_shell_handle_sql_t *ctx, struct handler_entry *e, const char **tokens, int ntokens);
int dongmendb_shell_handle_cmd_headers(dongmendb_shell_handle_sql_t *ctx, struct handler_entry *e, const char **tokens, int ntokens);
int dongmendb_shell_handle_cmd_explain(dongmendb_shell_handle_sql_t *ctx, struct handler_entry *e, const char **tokens, int ntokens);
int dongmendb_shell_handle_cmd_exit(dongmendb_shell_handle_sql_t *ctx, struct handler_entry *e, const char **tokens, int ntokens);
int dongmendb_shell_handle_cmd_desc(dongmendb_shell_handle_sql_t *ctx, struct handler_entry *e, const char **tokens, int ntokens);

int dongmendb_shell_handle_cmd(dongmendb_shell_handle_sql_t *ctx, const char *cmd);
int dongmendb_shell_handle_sql(dongmendb_shell_handle_sql_t *ctx, const char *sql);

int dongmendb_shell_handle_create_table(dongmendb_shell_handle_sql_t *ctx, const char *sqlcreate);
int dongmendb_shell_handle_insert_table(dongmendb_shell_handle_sql_t *ctx, const char *sqlinsert);
int dongmendb_shell_handle_select_table(dongmendb_shell_handle_sql_t *ctx, const char *sqlselect);


#endif /* COMMANDS_H_ */
