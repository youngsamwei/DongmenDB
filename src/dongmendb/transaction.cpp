//
// Created by Sam on 2018/1/26.
//

#include <malloc.h>
#include <algorithm>
#include "dongmendb/transaction.h"

transaction *transaction_create(dongmendb *db) {
    transaction *tx = (transaction *) malloc(sizeof(transaction));
    /*使用malloc申请空间，不能初始化map对象*/
    tx->bufferList = (buffer_list *) malloc(sizeof(buffer_list));
    tx->bufferList->bufferManager = db->bufferManager;

    /*TODO:需要释放*/
    tx->bufferList->buffers = new map<string, memory_buffer*>();

    tx->db = db;
    tx->txNum = transaction_next_txnum(tx);
    tx->recoveryManager = NULL;
    tx->concurrencyManager = NULL;

    return tx;
};

int transaction_commit(transaction *tx) {
    buffer_manager_flushall(tx->bufferList->bufferManager, tx->txNum);
    //recovery_manager_commit(tx->recoveryManager);
    //concurrency_manager_release(tx->concurrencyManager);
    buffer_list_unpin_all(tx->bufferList);

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
    memory_buffer *buffer = buffer_list_get_buffer(transaction->bufferList, block);
    return memory_buffer_getint(buffer, offset);
};

int transaction_setint(transaction *transaction, disk_block *block, int offset, int value) {
    //concurrency_manager_xlock(transaction->concurrencyManager, block);
    memory_buffer *buffer =
            buffer_list_get_buffer(transaction->bufferList, block);
    int lsn = 0;//recovery_manager_setint(transaction->recoveryManager, buffer, offset, value);
    return memory_buffer_setint(buffer, offset, value, transaction->txNum, lsn);
};

int transaction_getstring(transaction *transaction, disk_block *block, int offset, char *value) {
    //concurrency_manager_slock(transaction->concurrencyManager, block);
    memory_buffer *buffer =
            buffer_list_get_buffer(transaction->bufferList, block);
    return memory_buffer_getstring(buffer, offset, value);
};

int transaction_setstring(transaction *transaction, disk_block *block, int offset, const char *value) {
    //concurrency_manager_xlock(transaction->concurrencyManager, block);
    memory_buffer *buffer =
            buffer_list_get_buffer(transaction->bufferList, block);
    int lsn = 0;//recovery_manager_setstring(transaction->recoveryManager, buffer, offset, value);
    return memory_buffer_setstring(buffer, offset, value, transaction->txNum, lsn);
};

int transaction_size(transaction *tx, char *fileName) {
    return file_manager_size(tx->db->fileManager, fileName);
}

int transaction_append(transaction *tx, char *fileName, table_info *tableInfo) {
    //concurrency_manager_xlock
    void_ptr *pblock = (void_ptr *) malloc(sizeof(void_ptr *));
    buffer_list_pin_new(tx->bufferList, fileName, pblock, tableInfo);

    disk_block *block = (disk_block *) *pblock;
    transaction_unpin(tx, block);
    return 1;
}

int buffer_list_pin(buffer_list *bufferList, disk_block *block) {
    char *blockName = disk_block_get_num_string(block);
    void_ptr *pbuf = (void_ptr *) malloc(sizeof(void_ptr *));
    buffer_manager_pin(bufferList->bufferManager, block, pbuf);
    memory_buffer *buffer = (memory_buffer *) *pbuf;
    buffer->block = block;

    bufferList->buffers->insert(pair<string, memory_buffer*>(blockName, buffer));
    bufferList->pins.push_back(block);
    return 1;
};

int transaction_next_txnum(transaction *tx) {
    return next_tx_num++;
};

int buffer_list_unpin(buffer_list *bufferList, disk_block *block) {
    char *blockName = disk_block_get_num_string(block);
    memory_buffer *buffer;
    map<string, memory_buffer*>::iterator it = bufferList->buffers->find(blockName);
    if (it != bufferList->buffers->end()){
        buffer = it->second;
        bufferList->buffers->erase(it);
    }

    //buffer->block = NULL;
    buffer_manager_unpin(bufferList->bufferManager, buffer);
//    bufferList->pins->erase(block, bufferList->pins->begin(), bufferList->pins->end());

    vector<disk_block *>::iterator s=find(bufferList->pins.begin(),bufferList->pins.end(),block);
    if( s !=bufferList->pins.end()){
        bufferList->pins.erase(s);
    }

    return 1;
};

int buffer_list_unpin_all(buffer_list *bufferList) {
    int size = bufferList->pins.size() - 1;
    for (int i = 0; i <= size; i++) {
        disk_block *diskBlock = bufferList->pins.at(i);

        char *blockName = disk_block_get_num_string(diskBlock);
        memory_buffer *buffer = bufferList->buffers->find(blockName)->second;

        buffer_manager_unpin(bufferList->bufferManager, buffer);
    }
    bufferList->buffers->clear();
    bufferList->pins.clear();
};

memory_buffer *buffer_list_get_buffer(buffer_list *bufferList, disk_block *block) {
    char *blockName = disk_block_get_num_string(block);
    memory_buffer *buffer1 = bufferList->buffers->find(blockName)->second;
    return buffer1;
};

int buffer_list_pin_new(buffer_list *bufferList, char *fileName, void_ptr *pblock, table_info *tableInfo) {
    void_ptr *pbuffer = (void_ptr *) malloc(sizeof(void_ptr *));
    int r = buffer_manager_pinnew(bufferList->bufferManager, fileName, pbuffer, tableInfo);
    memory_buffer *buffer;
    if (r) {
        buffer = (memory_buffer *) *pbuffer;
    }

    disk_block *diskBlock = buffer->block;
    *pblock = buffer->block;

    char *blockName = disk_block_get_num_string(diskBlock);

    bufferList->buffers->insert(pair<string, memory_buffer*>(blockName, buffer));
    bufferList->pins.push_back(diskBlock);
    return 1;
};
