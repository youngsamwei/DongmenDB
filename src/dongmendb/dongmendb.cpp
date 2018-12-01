//
// Created by Sam on 2018/1/25.
//

#include <iostream>
#include "dongmendb/dongmendb.h"



int dongmendb_open(const char *dbName, dongmendb *db) {

    db->fileManager = new FileManager("", dbName);
    db->dbName = strdup(dbName);


    /*初始日志*/
    //log_manager_new(db->fileManager, file);
    db->bufferManager = new BufferManager(BUFFER_MAX_SIZE, db->fileManager);

    /*初始化事务*/
    Transaction *tx =    new Transaction( db);
    db->tx = tx;

    /*初始化元数据管理*/
    db->tableManager =  new TableManager(db->fileManager->isNew, tx);

    tx->transaction_commit();

    return DONGMENDB_OK;
};


extern int dongmendb_prepare(dongmendb *db, const char *sql, dongmendb_stmt **stmt) {};

int dongmendb_step(dongmendb_stmt *stmt) {};

int dongmendb_finalize(dongmendb_stmt *stmt) {};

int dongmendb_column_count(dongmendb_stmt *stmt) {};

int dongmendb_column_type(dongmendb_stmt *stmt, int col) {};

const char *dongmendb_column_name(dongmendb_stmt *stmt, int col) {};

int dongmendb_column_int(dongmendb_stmt *stmt, int col) {};

const char *dongmendb_column_text(dongmendb_stmt *stmt, int col) {};

int dongmendb_close(dongmendb *db) {
    db->fileManager->file_manager_closeallfile();
};



