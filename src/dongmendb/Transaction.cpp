//
// Created by Sam on 2018/1/26.
//

#include <malloc.h>
#include <algorithm>
#include "dongmendb/Transaction.h"

Transaction::Transaction(DongmenDB *db) {
    /*使用malloc申请空间，不能初始化map对象*/
    this->bufferList = new BufferList();
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
    this->bufferList->buffer_list_unpin_all();

}

int Transaction::transaction_rollback() {

}

int Transaction::transaction_recover() {

}

int Transaction::transaction_pin(DiskBlock *block) {
    return this->bufferList->buffer_list_pin(block);
};

int Transaction::transaction_unpin(DiskBlock *block) {
    this->bufferList->buffer_list_unpin(block);
};

int Transaction::transaction_getint(DiskBlock *block, int offset) {
    //concurrency_manager_slock(this->concurrencyManager, block);
    MemoryBuffer *buffer = this->bufferList->buffer_list_get_buffer( block);
    return buffer->memory_buffer_getint(offset);
};

int Transaction::transaction_setint(DiskBlock *block, int offset, int value) {
    //concurrency_manager_xlock(this->concurrencyManager, block);
    MemoryBuffer *buffer =
            this->bufferList->buffer_list_get_buffer(block);
    int lsn = 0;//recovery_manager_setint(this->recoveryManager, buffer, offset, value);
    return buffer->memory_buffer_setint( offset, value, this->txNum, lsn);
};

int Transaction::transaction_getstring(DiskBlock *block, int offset, char *value) {
    //concurrency_manager_slock(this->concurrencyManager, block);
    MemoryBuffer *buffer =
            this->bufferList->buffer_list_get_buffer(block);
    return buffer->memory_buffer_getstring( offset, value);
};

int Transaction::transaction_setstring(DiskBlock *block, int offset, const char *value) {
    //concurrency_manager_xlock(this->concurrencyManager, block);
    MemoryBuffer *buffer =
            this->bufferList->buffer_list_get_buffer(block);
    int lsn = 0;//recovery_manager_setstring(this->recoveryManager, buffer, offset, value);
    return buffer->memory_buffer_setstring( offset, value, this->txNum, lsn);
};

int Transaction::transaction_size(char *fileName) {
    return this->db->fileManager->file_manager_size(fileName);
}

int Transaction::transaction_append(char *fileName, TableInfo *tableInfo) {
    //concurrency_manager_xlock
    void_ptr *pblock = (void_ptr *) malloc(sizeof(void_ptr *));
    this->bufferList->buffer_list_pin_new( fileName, pblock, tableInfo);

    DiskBlock *block = (DiskBlock *) *pblock;
    transaction_unpin( block);
    return 1;
}

int Transaction::transaction_next_txnum() {
    return next_tx_num++;
};

int BufferList::buffer_list_pin(DiskBlock *block) {
    char *blockName = block->disk_block_get_num_string();
    void_ptr *pbuf = (void_ptr *) malloc(sizeof(void_ptr *));
    this->bufferManager->buffer_manager_pin(block, pbuf);
    MemoryBuffer *buffer = (MemoryBuffer *) *pbuf;
    buffer->block = block;

    this->buffers->insert(pair<string, MemoryBuffer*>(blockName, buffer));
    this->pins.push_back(block);
    return 1;
};

int BufferList::buffer_list_unpin(DiskBlock *block) {
    char *blockName = block->disk_block_get_num_string();
    MemoryBuffer *buffer;
    map<string, MemoryBuffer*>::iterator it = this->buffers->find(blockName);
    if (it != this->buffers->end()){
        buffer = it->second;
        this->buffers->erase(it);
    }

    //buffer->block = NULL;
    this->bufferManager->buffer_manager_unpin(buffer);
//    this->pins->erase(block, this->pins->begin(), this->pins->end());

    vector<DiskBlock *>::iterator s=find(this->pins.begin(),this->pins.end(),block);
    if( s !=this->pins.end()){
        this->pins.erase(s);
    }

    return 1;
};

int BufferList::buffer_list_unpin_all() {
    int size = this->pins.size() - 1;
    for (int i = 0; i <= size; i++) {
        DiskBlock *diskBlock = this->pins.at(i);

        char *blockName = diskBlock->disk_block_get_num_string();
        MemoryBuffer *buffer = this->buffers->find(blockName)->second;

        this->bufferManager-> buffer_manager_unpin( buffer);
    }
    this->buffers->clear();
    this->pins.clear();
};

MemoryBuffer *BufferList::buffer_list_get_buffer(DiskBlock *block) {
    char *blockName = block->disk_block_get_num_string();
    MemoryBuffer *buffer1 = this->buffers->find(blockName)->second;
    return buffer1;
};

int BufferList::buffer_list_pin_new(char *fileName, void_ptr *pblock, TableInfo *tableInfo) {
    void_ptr *pbuffer = (void_ptr *) malloc(sizeof(void_ptr *));
    int r = this->bufferManager-> buffer_manager_pinnew( fileName, pbuffer, tableInfo);
    MemoryBuffer *buffer;
    if (r) {
        buffer = (MemoryBuffer *) *pbuffer;
    }

    DiskBlock *diskBlock = buffer->block;
    *pblock = buffer->block;

    char *blockName = diskBlock->disk_block_get_num_string();

    this->buffers->insert(pair<string, MemoryBuffer*>(blockName, buffer));
    this->pins.push_back(diskBlock);
    return 1;
};
