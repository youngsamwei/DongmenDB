#include <string.h>
#include <utils/utils.h>
#include "shell/shell.h"

#include <stdio.h>
#include <stdlib.h>
#include <dongmendb/dongmendb.h>
#include <dongmensql/dongmensql.h>
#include <dongmendb/recordfile.h>
#include <utils/utils.h>
#include <parser/statement.h>
#include <physicalplan/physicalplan.h>

/*
 * 解析sql语句获得描述语句的结构，然后执行语句
 *
 * */
#ifdef __cplusplus
extern "C" {
#endif

#define COL_SEPARATOR "|"

void dongmendb_shell_init_ctx(dongmendb_shell_handle_sql_t *ctx)
{
    ctx->db = NULL;
    ctx->dbfile = NULL;

    ctx->header = false;
    ctx->mode = MODE_LIST;
}

struct handler_entry handlers[] =
        {
                HANDLER_ENTRY (open, ".open FILENAME     Close existing database (if any) and open FILENAME"),
                HANDLER_ENTRY (parse, ".parse \"SQL\"       Show parse tree for statement SQL"),
                HANDLER_ENTRY (opt,
                               ".optimizer \"SQL\"       Show parse tree and optimized parse tree for statement SQL"),
                HANDLER_ENTRY (headers, ".headers on|off    Switch display of headers on or off in dongmensql results"),
                HANDLER_ENTRY (mode, ".mode MODE         Switch display mode. MODE is one of:\n"
                        "                     column  Left-aligned columns\n"
                        "                     list    Values delimited by | (default)"),
                HANDLER_ENTRY (explain, ".explain on|off    Turn output mode suitable for EXPLAIN on or off."),
                HANDLER_ENTRY (help, ".help              Show this message"),
                HANDLER_ENTRY (exit, ".exit              exit shell"),
                HANDLER_ENTRY (desc, ".desc TABLENAME    desc table"),

                NULL_ENTRY
        };


void usage_error(struct handler_entry *e, const char *msg) {
    fprintf(stderr, "ERROR: %s\n", msg);
    fprintf(stderr, "%s\n", e->help);
}


int dongmendb_shell_handle_cmd(dongmendb_shell_handle_sql_t *ctx, const char *cmd) {
    int rc = 0;
    int h, ntokens;
    char *cmddup = strdup(cmd), **tokens;

    ntokens = dongmendb_tokenize(cmddup, &tokens);
    if (cmd[0] == '.') {

        for (h = 0; handlers[h].name != NULL; h++) {
            if (!strncmp(tokens[0] + 1, handlers[h].name, handlers[h].name_len)) {
                rc = handlers[h].func(ctx, &handlers[h], (const char **) tokens, ntokens);
                break;
            }
        }

        if (handlers[h].name == NULL) {
            fprintf(stderr, "ERROR: Unrecognized command: %s\n", tokens[0]);
            free(tokens);
            free(cmddup);
            return 1;
        } else {
            free(tokens);
            free(cmddup);
            return rc;
        }
    } else {
        /* Anything that is not a command is a SQL statement,
         * which requires a DB file. */
        if (!ctx->db) {
            fprintf(stderr, "ERROR: No database is open.\n");
            return 1;
        } else {
            if (stricmp(tokens[0], "select") == 0) {
                dongmendb_shell_handle_select_table(ctx, cmd);
                //rc = dongmendb_shell_handle_sql(ctx, cmd);
            } else if (stricmp(tokens[0], "create") == 0 && stricmp(tokens[1], "table") == 0) {
                dongmendb_shell_handle_create_table(ctx, cmd);
            } else if (stricmp(tokens[0], "insert") == 0 && stricmp(tokens[1], "into") == 0) {
                dongmendb_shell_handle_insert_table(ctx, cmd);
            }else if (stricmp(tokens[0], "update") == 0){
                dongmendb_shell_handle_update_data(ctx, cmd);
            }else if (stricmp(tokens[0], "delete") == 0){
                dongmendb_shell_handle_delete_data(ctx, cmd);
            } else {
                fprintf(stderr, "ERROR: not support %s.\n", tokens[0]);
            }

            return rc;
        }
    }

    return 0;
}

int dongmendb_shell_handle_sql(dongmendb_shell_handle_sql_t *ctx, const char *sql) {
    int rc;
    dongmendb_stmt *stmt;

    rc = dongmendb_prepare(ctx->db, sql, &stmt);

    if (rc == DONGMENDB_OK) {
        int numcol = dongmendb_column_count(stmt);

        if (ctx->header) {
            for (int i = 0; i < numcol; i++) {
                if (ctx->mode == MODE_LIST) {
                    printf(i == 0 ? "" : COL_SEPARATOR);
                    printf("%s", dongmendb_column_name(stmt, i));
                } else if (ctx->mode == MODE_COLUMN) {
                    printf(i == 0 ? "" : " ");
                    printf("%-10.10s", dongmendb_column_name(stmt, i));
                }
            }
            printf("\n");

            if (ctx->mode == MODE_COLUMN) {
                for (int i = 0; i < numcol; i++) {
                    printf(i == 0 ? "" : " ");
                    printf("----------");
                }
                printf("\n");
            }
        }

        while ((rc = dongmendb_step(stmt)) == DONGMENDB_ROW) {
            for (int i = 0; i < numcol; i++) {
                int coltype;

                if (ctx->mode == MODE_LIST)
                    printf(i == 0 ? "" : COL_SEPARATOR);
                else if (ctx->mode == MODE_COLUMN)
                    printf(i == 0 ? "" : " ");


                coltype = dongmendb_column_type(stmt, i);

                if (coltype == SQL_NOTVALID) {
                    printf("ERROR: Column %i return an invalid type.\n", coltype);
                    break;
                } else if (coltype == SQL_INTEGER_1BYTE || coltype == SQL_INTEGER_2BYTE ||
                           coltype == SQL_INTEGER_4BYTE) {
                    if (ctx->mode == MODE_LIST)
                        printf("%i", dongmendb_column_int(stmt, i));
                    else if (ctx->mode == MODE_COLUMN)
                        printf("%10i", dongmendb_column_int(stmt, i));
                } else if (coltype == SQL_NULL) {
                    /* Print nothing */
                    if (ctx->mode == MODE_COLUMN)
                        printf("          ");
                } else {
                    int len;
                    if ((coltype - 13) % 2 != 0) {
                        printf("ERROR: Column %i returned an invalid type.\n", i);
                        break;
                    }
                    const char *text = dongmendb_column_text(stmt, i);
                    len = strlen(text);
                    if (len != (coltype - 13) / 2) {
                        printf("ERROR: THe length (%i) of the text in column %i does not match its type (%i).\n", len,
                               i, coltype);
                        break;
                    }

                    if (ctx->mode == MODE_LIST)
                        printf("%s", text);
                    else if (ctx->mode == MODE_COLUMN)
                        printf("%-10.10s", text);
                }
            }
            printf("\n");
        }

        switch (rc) {
            case DONGMENDB_ECONSTRAINT:
                printf("ERROR: SQL statement failed because of a constraint violation.\n");
                break;
            case DONGMENDB_EMISMATCH:
                printf("ERROR: Data type mismatch.\n");
                break;
            case DONGMENDB_EMISUSE:
                printf("ERROR: API used incorrectly.\n");
                break;
            case DONGMENDB_ERROR_IO:
                printf("ERROR: An I/O error has occurred when accessing the file.\n");
                break;
        }

        rc = dongmendb_finalize(stmt);
        if (rc == DONGMENDB_EMISUSE)
            printf("API used incorrectly.\n");
    } else if (rc == DONGMENDB_EINVALIDSQL)
        printf("SQL syntax error.\n");
    else if (rc == DONGMENDB_ENOMEM)
        printf("ERROR: Could not allocate memory.\n");

    return rc;
}

/*处理create table*/
int dongmendb_shell_handle_create_table(dongmendb_shell_handle_sql_t *ctx, const char *sqlcreate) {
    if (!ctx->db) {
        fprintf(stderr, "ERROR: No database is open.\n");
        return 1;
    }

    TokenizerT *tokenizer = TKCreate(sqlcreate);
    ParserT *parser = newParser(tokenizer);
    memset(parser->parserMessage, 0, sizeof(parser->parserMessage));

//    int status = 0;
    sql_stmt_create *sqlStmtCreate = parse_sql_stmt_create(parser);

    /*TODO: 检查是否已经存在要创建的表 */
    int status =  semantic_check_table_exists(ctx->db->metadataManager->tableManager, sqlStmtCreate->tableInfo->tableName, ctx->db->tx);
    if (status != DONGMENDB_OK){
        fprintf(stderr, "table exists.");
        return DONGMENDB_ERROR_IO;
    }

   status = table_manager_create_table(ctx->db->metadataManager->tableManager,
                                            sqlStmtCreate->tableInfo->tableName,
                                            sqlStmtCreate->tableInfo->fieldsName,
                                            sqlStmtCreate->tableInfo->fields,
                                            ctx->db->tx);

    if (status == DONGMENDB_OK) {
        transaction_commit(ctx->db->tx);
        fprintf(stdout, "create  success!");
        return DONGMENDB_OK;
    } else {
        fprintf(stderr, "create  failed!");
        return DONGMENDB_ERROR_IO;
    }
};

/* 处理insert table语句*/
int dongmendb_shell_handle_insert_table(dongmendb_shell_handle_sql_t *ctx, const char *sqlinsert) {
    if (!ctx->db) {
        fprintf(stderr, "ERROR: No database is open.\n");
        return 1;
    }

    TokenizerT *tokenizer = TKCreate(sqlinsert);
    ParserT *parser = newParser(tokenizer);
    memset(parser->parserMessage, 0, sizeof(parser->parserMessage));

    sql_stmt_insert *sqlStmtInsert = parse_sql_stmt_insert(parser);

    /* TODO: 语义检查:检查表和字段是否存在*/
    int status =  semantic_check_table_exists(ctx->db->metadataManager->tableManager, sqlStmtInsert->tableName, ctx->db->tx);
    if (status != DONGMENDB_OK){
        fprintf(stderr, "table does not exist.");
        return DONGMENDB_ERROR_IO;
    }

    /* TODO: 安全性检查 */

    status = plan_execute_insert(ctx->db, sqlStmtInsert->tableName,
                                     sqlStmtInsert->fields,
                                     sqlStmtInsert->values,
                                     ctx->db->tx);

    if (status == DONGMENDB_OK) {
        transaction_commit(ctx->db->tx);
        fprintf(stdout, "insert  success!");
        return DONGMENDB_OK;
    } else {
        fprintf(stderr, "insert  failed!");
        return DONGMENDB_ERROR_IO;
    }
};

/*处理select语句*/
int dongmendb_shell_handle_select_table(dongmendb_shell_handle_sql_t *ctx, const char *sqlselect){
    if (!ctx->db) {
        fprintf(stderr, "ERROR: No database is open.\n");
        return 1;
    }

    TokenizerT *tokenizer = TKCreate(sqlselect);
    ParserT *parser = newParser(tokenizer);
    memset(parser->parserMessage, 0, sizeof(parser->parserMessage));

    /* 解析 select语句，获得SRA_t对象*/
    SRA_t *selectStmt = parse_sql_stmt_select(parser);
    if (selectStmt != NULL) {
        SRA_print(selectStmt);
    } else {
        printf(parser->parserMessage);
    }

    /*TODO: 语义检查：表与字段是否存在*/

    /*TODO: 安全性检查：用户是否有权限访问select中的数据表*/

    /*TODO: 逻辑优化：关系代数优化*/


    if (selectStmt != NULL) {
        /*执行select语句，获得物理扫描计划*/
        physical_scan *plan = plan_execute_select(ctx->db, selectStmt, ctx->db->tx);
        arraylist *exprs = plan->physicalScanProject->expr_list;
        printf("\n%s\n", getExpressionNamesTitle(exprs));
        plan->beforeFirst(plan);
        while (plan->next(plan)){
            for (int i = 0; i <= exprs->size - 1; i++) {
                variant *var = plan->getValByIndex(plan, i);
                if (var->type == DATA_TYPE_CHAR){
                    printf("%s\t", var->strValue);
                }else if(var->type == DATA_TYPE_INT){
                    printf("%i\t", var->intValue);
                }
            }
            printf("\n");
        }
        printf("\nsuccess.");
        plan->close(plan);
    } else {
        printf(parser->parserMessage);
    }
}

/*处理sql：update语句*/
int dongmendb_shell_handle_update_data(dongmendb_shell_handle_sql_t *ctx, const char *sqlupdate){
    /**
     *  1 初始化 TokenizerT和ParserT
     *  2 解析update语句：parse_sql_stmt_update，在src_experiment\exp_01_stmt_parser\exp_01_04_update.c中实现
     *  3 获得物理计划：plan_execute_update
     *  4 执行物理计划
     */

    if (!ctx->db) {
        fprintf(stderr, "ERROR: No database is open.\n");
        return 1;
    }

    /* token解析 */
    TokenizerT *tokenizer = TKCreate(sqlupdate);
    /* parser解析 */
    ParserT *parser = newParser(tokenizer);
    memset(parser->parserMessage, 0, sizeof(parser->parserMessage));

    /*TODO: parse_sql_stmt_update， update语句解析*/
    sql_stmt_update *sqlStmtUpdate = parse_sql_stmt_update(parser);
    if (sqlStmtUpdate == NULL) {
        printf(parser->parserMessage);
        return 1;
    }
    
    /* 显示update语句包含的查询计划*/
    sql_stmt_update_print(sqlStmtUpdate);

    /*TODO: 语义检查：表与字段是否存在*/
    int status = semantic_check_table_exists(ctx->db->metadataManager->tableManager, sqlStmtUpdate->tableName, ctx->db->tx);

    if (status != DONGMENDB_OK){
        fprintf(stdout, "table does not exist!");
        return status;
    }

    /*TODO: 安全性检查：用户是否有权限访问select中的数据表*/

    /*TODO: plan_execute_update， update语句执行*/
    int count = plan_execute_update(ctx->db, sqlStmtUpdate,
                                     ctx->db->tx);

    if (count >= 0) {
        transaction_commit(ctx->db->tx);
        fprintf(stdout, "update  success! %d line updated.", count);
        return DONGMENDB_OK;
    } else {
        fprintf(stderr, "update  failed!");
        return DONGMENDB_ERROR_IO;
    }
}

/*处理sql：delete语句*/
int dongmendb_shell_handle_delete_data(dongmendb_shell_handle_sql_t *ctx, const char *sqldelete) {
    /**
     *  1 初始化 TokenizerT和ParserT
     *  2 解析update语句：parse_sql_stmt_delete，在src_experiment\exp_01_stmt_parser\exp_01_05_delete.c中实现
     *  3 语义检查
     *  4 安全性检查
     *  5 获得物理计划：plan_execute_delete
     *  6 执行物理计划
     */
    if (!ctx->db) {
        fprintf(stderr, "ERROR: No database is open.\n");
        return 1;
    }

    TokenizerT *tokenizer = TKCreate(sqldelete);
    ParserT *parser = newParser(tokenizer);
    memset(parser->parserMessage, 0, sizeof(parser->parserMessage));

    sql_stmt_delete *sqlStmtDelete = parse_sql_stmt_delete(parser);
    if (sqlStmtDelete == NULL) {
        printf(parser->parserMessage);
        return 1;
    }

    /*TODO: 语义检查：表与字段是否存在*/
    int status = semantic_check_table_exists(ctx->db->metadataManager->tableManager, sqlStmtDelete->tableName,
                                             ctx->db->tx);
    if (status != DONGMENDB_OK) {
        fprintf(stdout, "table does not exist!");
        return status;
    }

    /*TODO: 安全性检查：用户是否有权限访问select中的数据表*/

    int count = plan_execute_delete(ctx->db, sqlStmtDelete,
                                    ctx->db->tx);

    if (count >= 0) {
        transaction_commit(ctx->db->tx);
        fprintf(stdout, "delete  success! %d line deleted.", count);
        return DONGMENDB_OK;
    } else {
        fprintf(stderr, "delete  failed!");
        return DONGMENDB_ERROR_IO;
    }
}

int dongmendb_shell_handle_cmd_open(dongmendb_shell_handle_sql_t *ctx, struct handler_entry *e, const char **tokens,
                                    int ntokens) {
    int rc;
    dongmendb *newdb = (dongmendb *) calloc(sizeof(dongmendb), 1);

    if (ntokens != 2) {
        usage_error(e, "Invalid arguments");
        return 1;
    }
    char *token = strdup(tokens[1]);
    rc = dongmendb_open(token, newdb);

    if (rc != DONGMENDB_OK) {
        fprintf(stderr, "ERROR: Could not open file %s or file is not well formed.\n", tokens[1]);
        return rc;
    }

    if (ctx->db) {
        dongmendb_close(ctx->db);
        free(ctx->dbfile);
    }

    ctx->db = newdb;
    ctx->dbfile = strdup(tokens[1]);

    return DONGMENDB_OK;
}

int dongmendb_shell_handle_cmd_parse(dongmendb_shell_handle_sql_t *ctx, struct handler_entry *e, const char **tokens,
                                     int ntokens) {
    dongmensql_statement_t *sqlStmt;
    int rc;

    if (ntokens != 2) {
        usage_error(e, "Invalid arguments");
        return 1;
    }

    rc = dongmensql_parser(tokens[1], &sqlStmt);

    if (rc != DONGMENDB_OK) {
        return rc;
    }

    dongmensql_stmt_print(sqlStmt);
    printf("\n");

    return DONGMENDB_OK;
}

/* Implemented in optimizer.c */
int dongmendb_stmt_optimize(dongmendb *db,
                            dongmensql_statement_t *sqlStmt,
                            dongmensql_statement_t **sqlStmtOpt);

int dongmendb_shell_handle_cmd_opt(dongmendb_shell_handle_sql_t *ctx, struct handler_entry *e, const char **tokens,
                                   int ntokens) {
    dongmensql_statement_t *sqlStmt, *sqlStmtOpt;
    int rc;

    if (ntokens != 2) {
        usage_error(e, "Invalid arguments");
        return 1;
    }

    if (!ctx->db) {
        fprintf(stderr, "ERROR: No database is open.\n");
        return 1;
    }

    rc = dongmensql_parser(tokens[1], &sqlStmt);

    if (rc != DONGMENDB_OK) {
        return rc;
    }

    dongmensql_stmt_print(sqlStmt);
    printf("\n\n");

    rc = dongmendb_stmt_optimize(ctx->db, sqlStmt, &sqlStmtOpt);

    if (rc != DONGMENDB_OK) {
        return rc;
    }

    dongmensql_stmt_print(sqlStmtOpt);
    printf("\n");

    return DONGMENDB_OK;
}

int
dongmendb_shell_handle_cmd_headers(dongmendb_shell_handle_sql_t *ctx, struct handler_entry *e, const char **tokens,
                                   int ntokens) {
    if (ntokens != 2) {
        usage_error(e, "Invalid arguments");
        return 1;
    }

    if (strcmp(tokens[1], "on") == 0)
        ctx->header = true;
    else if (strcmp(tokens[1], "off") == 0)
        ctx->header = false;
    else {
        usage_error(e, "Invalid argument");
        return 1;
    }

    return DONGMENDB_OK;
}

int dongmendb_shell_handle_cmd_mode(dongmendb_shell_handle_sql_t *ctx, struct handler_entry *e, const char **tokens,
                                    int ntokens) {
    if (ntokens != 2) {
        usage_error(e, "Invalid arguments");
        return 1;
    }

    if (strcmp(tokens[1], "list") == 0)
        ctx->mode = MODE_LIST;
    else if (strcmp(tokens[1], "column") == 0)
        ctx->mode = MODE_COLUMN;
    else {
        usage_error(e, "Invalid argument");
        return 1;
    }

    return DONGMENDB_OK;
}

int
dongmendb_shell_handle_cmd_explain(dongmendb_shell_handle_sql_t *ctx, struct handler_entry *e, const char **tokens,
                                   int ntokens) {
    if (ntokens != 2) {
        usage_error(e, "Invalid arguments");
        return 1;
    }

    if (strcmp(tokens[1], "on") == 0) {
        ctx->header = true;
        ctx->mode = MODE_COLUMN;
    } else if (strcmp(tokens[1], "off") == 0) {
        ctx->header = false;
        ctx->mode = MODE_LIST;
    } else {
        usage_error(e, "Invalid argument");
        return 1;
    }

    return DONGMENDB_OK;
}

int dongmendb_shell_handle_cmd_help(dongmendb_shell_handle_sql_t *ctx, struct handler_entry *e, const char **tokens,
                                    int ntokens) {
    for (int h = 0; handlers[h].name != NULL; h++) {
        fprintf(stderr, "%s\n", handlers[h].help);
    }

    return DONGMENDB_OK;
}

int dongmendb_shell_handle_cmd_exit(dongmendb_shell_handle_sql_t *ctx, struct handler_entry *e, const char **tokens,
                                    int ntokens) {
    if (ctx->db) {
        dongmendb_close(ctx->db);
        free(ctx->dbfile);
    }

    exit(0);
};

int dongmendb_shell_handle_cmd_desc(dongmendb_shell_handle_sql_t *ctx, struct handler_entry *e, const char **tokens,
                                    int ntokens) {
    if (!ctx->db) {
        fprintf(stderr, "ERROR: No database is open.\n");
        return DONGMENDB_ECANTOPEN;
    }
    if (ctx->db) {
        char *token = new_id_name();
        strcpy(token, tokens[1]);
        table_info *tableInfo = table_manager_get_tableinfo(ctx->db->metadataManager->tableManager, token, ctx->db->tx);
        table_info_free(tableInfo);
        free(token);
    }
    return DONGMENDB_OK;
};

#ifdef __cplusplus
}
#endif
