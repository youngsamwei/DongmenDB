//
// Created by Sam on 2018/1/25.
//

#include "dongmengdb.h"


int dongmengdb_open(char *dbName, dongmengdb *db) {

    db->fileManager = (file_manager *)malloc(sizeof(file_manager));

    /*初始化文件管理*/
    file_manager_new(db->fileManager, "", dbName);
    /*初始日志*/
    //log_manager_new(db->fileManager, file);
    db->bufferManager = (buffer_manager *) malloc(sizeof(buffer_manager));
    buffer_manager_create(db->bufferManager, BUFFER_MAX_SIZE, db->fileManager);

    /*初始化事务*/
    transaction *tx =    transaction_create( db);
    db->tx = tx;

    /*初始化元数据管理*/
    db->metadataManager = (metadata_manager *)malloc(sizeof(metadata_manager));
    metadata_manager_create(db->metadataManager, dbName, tx, db->fileManager->isNew);

    transaction_commit(tx);
    return DONGMENGDB_OK;
};


int dongmengdb_prepare(dongmengdb *db, const char *sql, dongmengdb_stmt **stmt) {};

int dongmengdb_step(dongmengdb_stmt *stmt) {};

int dongmengdb_finalize(dongmengdb_stmt *stmt) {};

int dongmengdb_column_count(dongmengdb_stmt *stmt) {};

int dongmengdb_column_type(dongmengdb_stmt *stmt, int col) {};

const char *dongmengdb_column_name(dongmengdb_stmt *stmt, int col) {};

int dongmengdb_column_int(dongmengdb_stmt *stmt, int col) {};

const char *dongmengdb_column_text(dongmengdb_stmt *stmt, int col) {};

int dongmengdb_close(dongmengdb *db) {};