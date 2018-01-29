//
// Created by Sam on 2018/1/25.
//

#include "dongmengdb.h"


int dongmengdb_open(const char *file, dongmengdb **db){};
int dongmengdb_prepare(dongmengdb *db, const char *sql, dongmengdb_stmt **stmt){};
int dongmengdb_step(dongmengdb_stmt *stmt){};
int dongmengdb_finalize(dongmengdb_stmt *stmt){};

int dongmengdb_column_count(dongmengdb_stmt *stmt){};
int dongmengdb_column_type(dongmengdb_stmt *stmt, int col){};
const char *dongmengdb_column_name(dongmengdb_stmt* stmt, int col){};
int dongmengdb_column_int(dongmengdb_stmt *stmt, int col){};
const char *dongmengdb_column_text(dongmengdb_stmt *stmt, int col){};

int dongmengdb_close(dongmengdb *db){};