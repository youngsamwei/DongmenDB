//
// Created by Sam on 2018/2/11.
//

#include <stdio.h>
#include <stdlib.h>
#include "dongmendb.h"
#include "tokenizer.h"
#include "parser.h"
#include "sra.h"
#include "parseSelectStmt.h"
#include "physicalplan.h"


int select(dongmendb *db, char *sqlselect) {
    char *sql = (char *) calloc(strlen(sqlselect), 1);
    strcpy(sql, sqlselect);
    TokenizerT *tokenizer = TKCreate(sql);
    ParserT *parser = newParser(tokenizer);
    memset(parser->parserMessage, 0, sizeof(parser->parserMessage));

    SRA_t *selectStmt = parse_sql_stmt_select(parser);
    physical_scan *plan = plan_execute_select(db, selectStmt, db->tx);
    plan->beforeFirst(plan);
    int count = 0;
    while (plan->next(plan)) {
        count++;
    }
    plan->close(plan);
    dongmendb_close(db);
    return count;
}

int main(int argc, const char *argv[]) {
    dongmendb *newdb = (dongmendb *) calloc(sizeof(dongmendb), 1);
    char *dbname = strdup(argv[1]);
    char *strselect = strdup(argv[2]);
    int rc = dongmendb_open(dbname, newdb);
    int count = select(newdb, strselect);
    printf("record count is %d\n", count);
    printf("exit 0!\n");
    return 0;
}
