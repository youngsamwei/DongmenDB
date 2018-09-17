//
// Created by Sam on 2018/1/26.
//

#ifndef DONGMENDB_TRANSACTION_H
#define DONGMENDB_TRANSACTION_H

#include "dongmendb.h"
#include <utils/arraylist.h>
#include "utils/hashmap.h"

#ifdef __cplusplus
extern "C" {
#endif

#define  END_OF_FILE -1

typedef struct dongmendb_ dongmendb;
typedef struct recovery_manager_ recovery_manager;
typedef struct concurrency_manager_ concurrency_manager;
typedef struct buffer_list_ buffer_list;
typedef struct buffer_manager_ buffer_manager;

static int next_tx_num = 1;

typedef struct transaction_ {
    dongmendb *db;
    int txNum;
    recovery_manager *recoveryManager;
    concurrency_manager *concurrencyManager;
    buffer_list *bufferList;
} transaction;

typedef struct buffer_list_ {
    hmap_t buffers;
    arraylist *pins;
    buffer_manager *bufferManager;
} buffer_list;

transaction *transaction_create(dongmendb *db);
int transaction_commit(transaction *transaction);
int transaction_rollback(transaction *transaction);
int transaction_recover(transaction *transaction);

int transaction_pin(transaction *transaction, disk_block *block);
int transaction_unpin(transaction *transaction, disk_block *block);

int transaction_getint(transaction *transaction, disk_block *block, int offset);
int transaction_setint(transaction *transaction, disk_block *block, int offset, int value);
int transaction_getstring(transaction *transaction, disk_block *block, int offset, char *value);
int transaction_setstring(transaction *transaction, disk_block *block, int offset, char *value);

int transaction_size(transaction *tx, char *fileName);
int transaction_append(transaction *tx, char *fileName, table_info *tableInfo);

int transaction_next_txnum(transaction *tx);

int buffer_list_pin(buffer_list *bufferList, disk_block *block);
int buffer_list_unpin(buffer_list *bufferList, disk_block *block);
int buffer_list_unpin_all(buffer_list *bufferList);
memory_buffer *buffer_list_get_buffer(buffer_list *bufferList, disk_block *block);
int buffer_list_pin_new(buffer_list *bufferList, char *fileName, void_ptr *block, table_info *tableInfo);

#ifdef __cplusplus
}
#endif

#endif //DONGMENDB_TRANSACTION_H
