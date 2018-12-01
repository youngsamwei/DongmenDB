//
// Created by Sam on 2018/1/26.
//

#include <malloc.h>
#include <algorithm>
#include "dongmendb/Transaction.h"

Transaction::Transaction(dongmendb *db) {
    /*使用malloc申请空间，不能初始化map对象*/
    this->bufferList = (buffer_list *) malloc(sizeof(buffer_list));
    this->bufferList->bufferManager = db->bufferManager;

    /*TODO:需要释放*/
    this->bufferList->buffers = new map<string, MemoryBuffer*>();

    this->db = db;
    this->txNum = transaction_next_txnum();
    this->recoveryManager = NULL;
    this->concurrencyManager = NULL;

};

int Transaction::transaction_commit() {
    this->bufferList->bufferManager->buffer_manager_flushall(this->txNum);
    //recovery_manager_commit(this->recoveryManager);
    //concurrency_manager_release(this->concurrencyManager);
    buffer_list_unpin_all(this->bufferList);

}

int Transaction::transaction_rollback() {

}

int Transaction::transaction_recover() {

}

int Transaction::transaction_pin(DiskBlock *block) {
    return buffer_list_pin(this->bufferList, block);
};

int Transaction::transaction_unpin(DiskBlock *block) {
    buffer_list_unpin(this->bufferList, block);
};

int Transaction::transaction_getint(DiskBlock *block, int offset) {
    //concurrency_manager_slock(this->concurrencyManager, block);
    MemoryBuffer *buffer = buffer_list_get_buffer(this->bufferList, block);
    return buffer->memory_buffer_getint(offset);
};

int Transaction::transaction_setint(DiskBlock *block, int offset, int value) {
    //concurrency_manager_xlock(this->concurrencyManager, block);
    MemoryBuffer *buffer =
            buffer_list_get_buffer(this->bufferList, block);
    int lsn = 0;//recovery_manager_setint(this->recoveryManager, buffer, offset, value);
    return buffer->memory_buffer_setint( offset, value, this->txNum, lsn);
};

int Transaction::transaction_getstring(DiskBlock *block, int offset, char *value) {
    //concurrency_manager_slock(this->concurrencyManager, block);
    MemoryBuffer *buffer =
            buffer_list_get_buffer(this->bufferList, block);
    return buffer->memory_buffer_getstring( offset, value);
};

int Transaction::transaction_setstring(DiskBlock *block, int offset, const char *value) {
    //concurrency_manager_xlock(this->concurrencyManager, block);
    MemoryBuffer *buffer =
            buffer_list_get_buffer(this->bufferList, block);
    int lsn = 0;//recovery_manager_setstring(this->recoveryManager, buffer, offset, value);
    return buffer->memory_buffer_setstring( offset, value, this->txNum, lsn);
};

int Transaction::transaction_size(char *fileName) {
    return this->db->fileManager->file_manager_size(fileName);
}

int Transaction::transaction_append(char *fileName, table_info *tableInfo) {
    //concurrency_manager_xlock
    void_ptr *pblock = (void_ptr *) malloc(sizeof(void_ptr *));
    buffer_list_pin_new(this->bufferList, fileName, pblock, tableInfo);

    DiskBlock *block = (DiskBlock *) *pblock;
    transaction_unpin( block);
    return 1;
}

int Transaction::transaction_next_txnum() {
    return next_tx_num++;
};

int buffer_list_pin(buffer_list *bufferList, DiskBlock *block) {
    char *blockName = block->disk_block_get_num_string();
    void_ptr *pbuf = (void_ptr *) malloc(sizeof(void_ptr *));
    bufferList->bufferManager->buffer_manager_pin(block, pbuf);
    MemoryBuffer *buffer = (MemoryBuffer *) *pbuf;
    buffer->block = block;

    bufferList->buffers->insert(pair<string, MemoryBuffer*>(blockName, buffer));
    bufferList->pins.push_back(block);
    return 1;
};

int buffer_list_unpin(buffer_list *bufferList, DiskBlock *block) {
    char *blockName = block->disk_block_get_num_string();
    MemoryBuffer *buffer;
    map<string, MemoryBuffer*>::iterator it = bufferList->buffers->find(blockName);
    if (it != bufferList->buffers->end()){
        buffer = it->second;
        bufferList->buffers->erase(it);
    }

    //buffer->block = NULL;
    bufferList->bufferManager->buffer_manager_unpin(buffer);
//    bufferList->pins->erase(block, bufferList->pins->begin(), bufferList->pins->end());

    vector<DiskBlock *>::iterator s=find(bufferList->pins.begin(),bufferList->pins.end(),block);
    if( s !=bufferList->pins.end()){
        bufferList->pins.erase(s);
    }

    return 1;
};

int buffer_list_unpin_all(buffer_list *bufferList) {
    int size = bufferList->pins.size() - 1;
    for (int i = 0; i <= size; i++) {
        DiskBlock *diskBlock = bufferList->pins.at(i);

        char *blockName = diskBlock->disk_block_get_num_string();
        MemoryBuffer *buffer = bufferList->buffers->find(blockName)->second;

        bufferList->bufferManager-> buffer_manager_unpin( buffer);
    }
    bufferList->buffers->clear();
    bufferList->pins.clear();
};

MemoryBuffer *buffer_list_get_buffer(buffer_list *bufferList, DiskBlock *block) {
    char *blockName = block->disk_block_get_num_string();
    MemoryBuffer *buffer1 = bufferList->buffers->find(blockName)->second;
    return buffer1;
};

int buffer_list_pin_new(buffer_list *bufferList, char *fileName, void_ptr *pblock, table_info *tableInfo) {
    void_ptr *pbuffer = (void_ptr *) malloc(sizeof(void_ptr *));
    int r = bufferList->bufferManager-> buffer_manager_pinnew( fileName, pbuffer, tableInfo);
    MemoryBuffer *buffer;
    if (r) {
        buffer = (MemoryBuffer *) *pbuffer;
    }

    DiskBlock *diskBlock = buffer->block;
    *pblock = buffer->block;

    char *blockName = diskBlock->disk_block_get_num_string();

    bufferList->buffers->insert(pair<string, MemoryBuffer*>(blockName, buffer));
    bufferList->pins.push_back(diskBlock);
    return 1;
};
