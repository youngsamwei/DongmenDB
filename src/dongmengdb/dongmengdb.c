//
// Created by Sam on 2018/1/25.
//

#include "dongmengdb.h"

int dongmengdbOpen(const char *file, dongmengdb **db){};
int dongmengdbPrepare(dongmengdb *db, const char *sql, dongmengdb_stmt **stmt){};
int dongmengdbStep(dongmengdb_stmt *stmt){};
int dongmengdbFinalize(dongmengdb_stmt *stmt){};

int dongmengdbColumnCount(dongmengdb_stmt *stmt){};
int dongmengdbColumnType(dongmengdb_stmt *stmt, int col){};
const char *dongmengdbColumnName(dongmengdb_stmt* stmt, int col){};
int dongmengdbColumnInt(dongmengdb_stmt *stmt, int col){};
const char *dongmengdbColumnText(dongmengdb_stmt *stmt, int col){};

int dongmengdbClose(dongmengdb *db){};