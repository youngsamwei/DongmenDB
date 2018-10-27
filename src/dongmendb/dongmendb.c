//
// Created by Sam on 2018/1/25.
//

#include "dongmendb/dongmendb.h"

#ifdef __cplusplus
extern "C" {
#endif


int dongmendb_open(const char *dbName, dongmendb *db) {

    db->fileManager = (file_manager *)malloc(sizeof(file_manager));
    db->dbName = strdup(dbName);

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
    return DONGMENDB_OK;
};


int dongmendb_prepare(dongmendb *db, const char *sql, dongmendb_stmt **stmt) {};

int dongmendb_step(dongmendb_stmt *stmt) {};

int dongmendb_finalize(dongmendb_stmt *stmt) {};

int dongmendb_column_count(dongmendb_stmt *stmt) {};

int dongmendb_column_type(dongmendb_stmt *stmt, int col) {};

const char *dongmendb_column_name(dongmendb_stmt *stmt, int col) {};

int dongmendb_column_int(dongmendb_stmt *stmt, int col) {};

const char *dongmendb_column_text(dongmendb_stmt *stmt, int col) {};

int dongmendb_close(dongmendb *db) {
    file_manager_closeallfile(db->fileManager);
};


#ifdef __cplusplus
}
#endif
