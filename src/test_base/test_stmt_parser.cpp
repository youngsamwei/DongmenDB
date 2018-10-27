//
// Created by sam on 2018/10/20.
//

#include <test/test_stmt_parser.h>

void TestStmtParser :: createDB(const char *dbname){

    test_db_ctx = (dongmendb_shell_handle_sql_t *)calloc(sizeof(dongmendb_shell_handle_sql_t),1);
    dongmendb_shell_init_ctx(test_db_ctx);

    dongmendb *newdb = (dongmendb *) calloc(sizeof(dongmendb), 1);

    int rc = dongmendb_open(dbname, newdb);

    if (rc != DONGMENDB_OK) {
        fprintf(stderr, "ERROR: Could not open file %s or file is not well formed.\n", dbname);
    }
    test_db_ctx->db = newdb;
    test_db_ctx->dbfile = strdup(dbname);

}

void TestStmtParser ::createTable(){
    int length=sizeof(_create_table_list)/sizeof(0);

    for (int i =0 ; i< length; i++){
        dongmendb_shell_handle_create_table(test_db_ctx, _create_table_list[i]);
    }

};

/*为db增加数据*/
void TestStmtParser ::insertData() {
    int length=sizeof(_insert_list)/sizeof(0);

    for (int i =0 ; i< length; i++){
        dongmendb_shell_handle_insert_table(test_db_ctx, _insert_list[i]);
    }
}


/*删除 db 指向的数据库*/
void TestStmtParser ::dropDB() {

    dongmendb_close(test_db_ctx->db);

    removeDir(test_db_ctx->dbfile);

}

int TestStmtParser ::select(const char *sqlselect) {

    TokenizerT *tokenizer = TKCreate(sqlselect);
    ParserT *parser = newParser(tokenizer);
    memset(parser->parserMessage, 0, sizeof(parser->parserMessage));

    SRA_t *selectStmt = parse_sql_stmt_select(parser);
    physical_scan *plan = plan_execute_select(test_db_ctx->db, selectStmt, test_db_ctx->db->tx);
    plan->beforeFirst(plan);
    int count = 0;
    while (plan->next(plan)) {
        count++;
    }
    plan->close(plan);

    return count;
}


int TestStmtParser ::delete_( const char *strdelete) {

    TokenizerT *tokenizer = TKCreate(strdelete);
    ParserT *parser = newParser(tokenizer);
    memset(parser->parserMessage, 0, sizeof(parser->parserMessage));

    sql_stmt_delete *sqlStmtDelete  = parse_sql_stmt_delete(parser);

    /*返回修改的记录条数*/
    int count  = 0;
    count = plan_execute_delete(test_db_ctx->db, sqlStmtDelete, test_db_ctx->db->tx);

    return count;
}

int TestStmtParser ::update(const char *strupdate) {

    TokenizerT *tokenizer = TKCreate(strupdate);
    ParserT *parser = newParser(tokenizer);
    memset(parser->parserMessage, 0, sizeof(parser->parserMessage));
    sql_stmt_update *sqlStmtUpdate  = parse_sql_stmt_update(parser);

    if(sqlStmtUpdate == NULL){
        printf(parser->parserMessage);
        return -1;
    }

    sql_stmt_update_print(sqlStmtUpdate);

    /*返回修改的记录条数*/
    int count  = 0;
    count = plan_execute_update(test_db_ctx->db, sqlStmtUpdate, test_db_ctx->db->tx);

    return count;
}
