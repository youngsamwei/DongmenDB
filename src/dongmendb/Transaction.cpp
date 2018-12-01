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
    this->bufferList->buffers = new map<string, memory_buffer*>();

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

int Transaction::transaction_pin(disk_block *block) {
    return buffer_list_pin(this->bufferList, block);
};

int Transaction::transaction_unpin(disk_block *block) {
    buffer_list_unpin(this->bufferList, block);
};

int Transaction::transaction_getint(disk_block *block, int offset) {
    //concurrency_manager_slock(this->concurrencyManager, block);
    memory_buffer *buffer = buffer_list_get_buffer(this->bufferList, block);
    return memory_buffer_getint(buffer, offset);
};

int Transaction::transaction_setint(disk_block *block, int offset, int value) {
    //concurrency_manager_xlock(this->concurrencyManager, block);
    memory_buffer *buffer =
            buffer_list_get_buffer(this->bufferList, block);
    int lsn = 0;//recovery_manager_setint(this->recoveryManager, buffer, offset, value);
    return memory_buffer_setint(buffer, offset, value, this->txNum, lsn);
};

int Transaction::transaction_getstring(disk_block *block, int offset, char *value) {
    //concurrency_manager_slock(this->concurrencyManager, block);
    memory_buffer *buffer =
            buffer_list_get_buffer(this->bufferList, block);
    return memory_buffer_getstring(buffer, offset, value);
};

int Transaction::transaction_setstring(disk_block *block, int offset, const char *value) {
    //concurrency_manager_xlock(this->concurrencyManager, block);
    memory_buffer *buffer =
            buffer_list_get_buffer(this->bufferList, block);
    int lsn = 0;//recovery_manager_setstring(this->recoveryManager, buffer, offset, value);
    return memory_buffer_setstring(buffer, offset, value, this->txNum, lsn);
};

int Transaction::transaction_size(char *fileName) {
    return file_manager_size(this->db->fileManager, fileName);
}

int Transaction::transaction_append(char *fileName, table_info *tableInfo) {
    //concurrency_manager_xlock
    void_ptr *pblock = (void_ptr *) malloc(sizeof(void_ptr *));
    buffer_list_pin_new(this->bufferList, fileName, pblock, tableInfo);

    disk_block *block = (disk_block *) *pblock;
    transaction_unpin( block);
    return 1;
}

int Transaction::transaction_next_txnum() {
    return next_tx_num++;
};

int buffer_list_pin(buffer_list *bufferList, disk_block *block) {
    char *blockName = disk_block_get_num_string(block);
    void_ptr *pbuf = (void_ptr *) malloc(sizeof(void_ptr *));
    bufferList->bufferManager->buffer_manager_pin(block, pbuf);
    memory_buffer *buffer = (memory_buffer *) *pbuf;
    buffer->block = block;

    bufferList->buffers->insert(pair<string, memory_buffer*>(blockName, buffer));
    bufferList->pins.push_back(block);
    return 1;
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
    bufferList->bufferManager->buffer_manager_unpin(buffer);
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

        bufferList->bufferManager-> buffer_manager_unpin( buffer);
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
    int r = bufferList->bufferManager-> buffer_manager_pinnew( fileName, pbuffer, tableInfo);
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
