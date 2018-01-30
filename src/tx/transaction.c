//
// Created by Sam on 2018/1/26.
//

#include <malloc.h>
#include "transaction.h"

int transaction_create(transaction *tx, dongmengdb *db) {

    tx->bufferList = (buffer_list *) malloc(sizeof(buffer_list));
    tx->bufferList->bufferManager = db->bufferManager;
    tx->bufferList->pins = array_list_create(DEFAULT_ARRAY_LIST_SIZE);
    tx->bufferList->buffers = hashmap_create();
    tx->db = db;
};

int transaction_commit(transaction *transaction) {

}

int transaction_rollback(transaction *transaction) {

}

int transaction_recover(transaction *transaction) {

}

int transaction_pin(transaction *transaction, disk_block *block) {
    return buffer_list_pin(transaction->bufferList, block);
};

int transaction_unpin(transaction *transaction, disk_block *block) {
    buffer_list_unpin(transaction->bufferList, block);
};

int transaction_getint(transaction *transaction, disk_block *block, int offset) {
    //concurrency_manager_slock(transaction->concurrencyManager, block);
    memory_buffer *buffer;
    buffer_list_get_buffer(transaction->bufferList, block, buffer);
    return memory_buffer_getint(buffer, offset);
};

int transaction_setint(transaction *transaction, disk_block *block, int offset, int value) {
    //concurrency_manager_xlock(transaction->concurrencyManager, block);
    memory_buffer *buffer;
    buffer_list_get_buffer(transaction->bufferList, block, buffer);
    int lsn = 0;//recovery_manager_setint(transaction->recoveryManager, buffer, offset, value);
    return memory_buffer_setint(buffer, offset, value, transaction->txNum, lsn);
};

int transaction_getstring(transaction *transaction, disk_block *block, int offset, char *value) {
    //concurrency_manager_slock(transaction->concurrencyManager, block);
    memory_buffer *buffer;
    buffer_list_get_buffer(transaction->bufferList, block, buffer);
    return memory_buffer_getstring(buffer, offset, value);
};

int transaction_setstring(transaction *transaction, disk_block *block, int offset, char *value) {
    //concurrency_manager_xlock(transaction->concurrencyManager, block);
    memory_buffer *buffer;
    buffer_list_get_buffer(transaction->bufferList, block, buffer);
    int lsn = 0;//recovery_manager_setstring(transaction->recoveryManager, buffer, offset, value);
    return memory_buffer_setstring(buffer, offset, value, transaction->txNum, lsn);
};

int transaction_size(transaction *tx, char *fileName) {
    return file_manager_size(tx->db->fileManager, fileName);
}

int transaction_append(transaction *tx, char *fileName, table_info *tableInfo) {
    //concurrency_manager_xlock
    disk_block *block;
    buffer_list_pin_new(tx->bufferList, fileName, block, tableInfo);
    transaction_unpin(tx, block);
    return 1;
}

int transaction_next_txnum(transaction *tx) {
    return 1;
};

int buffer_list_pin(buffer_list *bufferList, disk_block *block) {
    char *blockName = disk_block_get_num_string(block);
    void_ptr *buffer;
    buffer_manager_pin(bufferList->bufferManager, block, buffer);
    hashmap_put(bufferList->buffers, blockName, buffer);
    array_list_add(&bufferList->pins, block);
    return 1;
};

int buffer_list_unpin(buffer_list *bufferList, disk_block *block) {
    char *blockName = disk_block_get_num_string(block);
    void_ptr *buffer;
    hashmap_get(bufferList->buffers, blockName, buffer);
    memory_buffer *buffer1 = (memory_buffer *) buffer;
    buffer_manager_unpin(bufferList->bufferManager, buffer1);
    array_list_remove(&bufferList->pins, block);
    return 1;
};

int buffer_list_get_buffer(buffer_list *bufferList, disk_block *block, memory_buffer *buffer) {
    char *blockName = disk_block_get_num_string(block);
    void_ptr *buffer1;
    hashmap_get(bufferList->buffers, blockName, buffer1);
    buffer = (memory_buffer *) buffer1;
    return 1;
};

int buffer_list_pin_new(buffer_list *bufferList, char *fileName, disk_block *block, table_info *tableInfo) {
    void_ptr *pbuffer;
    int r = buffer_manager_pinnew(bufferList->bufferManager, fileName, pbuffer, tableInfo);
    memory_buffer *buffer;
    if (r){
        buffer  = *pbuffer;
    }

    block = buffer->block;

    char *blockName = disk_block_get_num_string(block);
    hashmap_put(bufferList->buffers, blockName, buffer);
    array_list_add(&bufferList->pins, block);
    return 1;
};
