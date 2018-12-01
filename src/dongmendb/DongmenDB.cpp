//
// Created by Sam on 2018/1/25.
//

#include <iostream>
#include "dongmendb/DongmenDB.h"



int DongmenDB::dongmendb_open(const char *dbName) {

    this->fileManager = new FileManager("", dbName);
    this->dbName = strdup(dbName);


    /*初始日志*/
    //log_manager_new(this->fileManager, file);
    this->bufferManager = new BufferManager(BUFFER_MAX_SIZE, this->fileManager);

    /*初始化事务*/
    Transaction *tx =    new Transaction( this);
    this->tx = tx;

    /*初始化元数据管理*/
    this->tableManager =  new TableManager(this->fileManager->isNew, tx);

    tx->transaction_commit();

    return DONGMENDB_OK;
};


int DongmenDB::dongmendb_prepare(const char *sql, dongmendb_stmt **stmt) {};

int DongmenDB::dongmendb_step(dongmendb_stmt *stmt) {};

int DongmenDB::dongmendb_finalize(dongmendb_stmt *stmt) {};

int DongmenDB::dongmendb_column_count(dongmendb_stmt *stmt) {};

int DongmenDB::dongmendb_column_type(dongmendb_stmt *stmt, int col) {};

const char *DongmenDB::dongmendb_column_name(dongmendb_stmt *stmt, int col) {};

int DongmenDB::dongmendb_column_int(dongmendb_stmt *stmt, int col) {};

const char *DongmenDB::dongmendb_column_text(dongmendb_stmt *stmt, int col) {};

int DongmenDB::dongmendb_close() {
    this->fileManager->file_manager_closeallfile();
};



