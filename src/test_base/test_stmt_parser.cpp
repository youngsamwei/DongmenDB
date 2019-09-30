//
// Created by sam on 2018/10/20.
//

#include <test/test_stmt_parser.h>
#include <dongmensql/sqlstatement.h>
#include <parser/StatementParser.h>


void TestStmtParser::createDB(const char *dbname) {

    test_db_ctx = (dongmendb_shell_handle_sql_t *) calloc(1, sizeof(dongmendb_shell_handle_sql_t));
    dongmendb_shell_init_ctx(test_db_ctx);

    DongmenDB *newdb = new DongmenDB();

    int rc = newdb->dongmendb_open(dbname);

    if (rc != DONGMENDB_OK) {
        fprintf(stderr, "ERROR: Could not open file %s or file is not well formed.\n", dbname);
    }
    test_db_ctx->db = newdb;
    test_db_ctx->dbfile = strdup(dbname);

}

void TestStmtParser::createTable() {
    int length = sizeof(_create_table_list) / sizeof(_create_table_list[0]);

    for (int i = 0; i < length; i++) {
        dongmendb_shell_handle_create_table(test_db_ctx, _create_table_list[i]);
    }

};

/*为db增加数据*/
void TestStmtParser::insertData() {
    int length = sizeof(_insert_list) / sizeof(_insert_list[0]);

    for (int i = 0; i < length; i++) {
        dongmendb_shell_handle_insert_table(test_db_ctx, _insert_list[i]);
    }
}


/*删除 db 指向的数据库*/
void TestStmtParser::dropDB() {

    test_db_ctx->db->dongmendb_close();

    removeDir(test_db_ctx->dbfile);

}

int TestStmtParser::select(const char *sqlselect) {

    Tokenizer *tokenizer = new Tokenizer(sqlselect);
    Parser *parser = new Parser(tokenizer);
    memset(parser->parserMessage, 0, sizeof(parser->parserMessage));

    SelectParser *sp = new SelectParser(tokenizer);
    SRA_t *selectStmt = sp->parse_sql_stmt_select();
    ExecutionPlan plan;

    Scan* scan = plan.generateSelect(test_db_ctx->db, selectStmt, test_db_ctx->db->tx);
    Project *project = (Project*) scan;

    project->beforeFirst();

    int count = 0;
    while (project->next()) {
        count++;
    }
    project->close();

    return count;
}


int TestStmtParser::delete_(const char *strdelete) {

    Tokenizer *tokenizer =  new Tokenizer(strdelete);
    Parser *parser = new Parser(tokenizer);
    memset(parser->parserMessage, 0, sizeof(parser->parserMessage));

    DeleteParser *dp = new DeleteParser(tokenizer);
    sql_stmt_delete *sqlStmtDelete = dp->parse_sql_stmt_delete();

    /*返回修改的记录条数*/
    int count = 0;
    ExecutionPlan plan;
    count = plan.executeDelete(test_db_ctx->db, sqlStmtDelete, test_db_ctx->db->tx);

    return count;
}

int TestStmtParser::update(const char *strupdate) {

    Tokenizer *tokenizer = new Tokenizer(strupdate);
    Parser *parser = new Parser(tokenizer);
    memset(parser->parserMessage, 0, sizeof(parser->parserMessage));

    UpdateParser *up = new UpdateParser(tokenizer);
    sql_stmt_update *sqlStmtUpdate = up->parse_sql_stmt_update();

    if (sqlStmtUpdate == NULL) {
        printf(parser->parserMessage);
        return -1;
    }

    sql_stmt_update_print(sqlStmtUpdate);

    /*返回修改的记录条数*/
    int count = 0;
    ExecutionPlan plan;
    count = plan.executeUpdate(test_db_ctx->db, sqlStmtUpdate, test_db_ctx->db->tx);

    return count;
}
