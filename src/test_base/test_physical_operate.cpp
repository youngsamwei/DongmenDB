//
// Created by maokw on 2020-03-04.
//

#include <test/test_physical_operate.h>
#include <parser/Tokenizer.h>
#include <parser/parser.h>
#include <parser/StatementParser.h>


void TestPhysicalOperate::createDB(const char *dbname) {

    test_db_ctx = (dongmendb_shell_handle_sql_t *) calloc(1, sizeof(dongmendb_shell_handle_sql_t));
    dongmendb_shell_init_ctx(test_db_ctx);

    auto *newdb = new DongmenDB();

    int rc = newdb->dongmendb_open(dbname);

    if (rc != DONGMENDB_OK) {
        fprintf(stderr, "ERROR: Could not open file %s or file is not well formed.\n", dbname);
    }
    test_db_ctx->db = newdb;
    test_db_ctx->dbfile = strdup(dbname);
}

void TestPhysicalOperate::createTable() {
    int length = sizeof(_create_table_list) / sizeof(_create_table_list[0]);

    for (int i = 0; i < length; i++) {
        dongmendb_shell_handle_create_table(test_db_ctx, _create_table_list[i]);
    }
}

/*为db增加数据*/
void TestPhysicalOperate::insertData() {
    int length = sizeof(_insert_list) / sizeof(_insert_list[0]);

    for (int i = 0; i < length; i++) {
        dongmendb_shell_handle_insert_table(test_db_ctx, _insert_list[i]);
    }
}


/*删除 db 指向的数据库*/
void TestPhysicalOperate::dropDB() {

    test_db_ctx->db->dongmendb_close();

    removeDir(test_db_ctx->dbfile);

}

int TestPhysicalOperate::select(const char *sqlselect) {

    auto *tokenizer = new Tokenizer(sqlselect);
    auto *parser = new Parser(tokenizer);
    memset(parser->parserMessage, 0, sizeof(parser->parserMessage));

    auto *sp = new SelectParser(tokenizer);
    SRA_t *selectStmt = sp->parse_sql_stmt_select();
    ExecutionPlan plan;

    Scan *scan = plan.generateSelect(test_db_ctx->db, selectStmt, test_db_ctx->db->tx);
    auto *project = (Project *) scan;

    project->beforeFirst();

    int count = 0;
    while (project->next()) {
        count++;
    }
    project->close();

    return count;
}


int TestPhysicalOperate::delete_(const char *strdelete) {

    auto *tokenizer = new Tokenizer(strdelete);
    auto *parser = new Parser(tokenizer);
    memset(parser->parserMessage, 0, sizeof(parser->parserMessage));

    auto *dp = new DeleteParser(tokenizer);
    sql_stmt_delete *sqlStmtDelete = dp->parse_sql_stmt_delete();

    /*返回修改的记录条数*/
    int count = 0;
    ExecutionPlan plan;
    count = plan.executeDelete(test_db_ctx->db, sqlStmtDelete, test_db_ctx->db->tx);

    return count;
}

int TestPhysicalOperate::update(const char *strupdate) {

    auto *tokenizer = new Tokenizer(strupdate);
    auto *parser = new Parser(tokenizer);
    memset(parser->parserMessage, 0, sizeof(parser->parserMessage));

    auto *up = new UpdateParser(tokenizer);
    sql_stmt_update *sqlStmtUpdate = up->parse_sql_stmt_update();

    if (sqlStmtUpdate == nullptr) {
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