//
// Created by Sam on 2018/1/26.
//

#include "buffermanager.h"

int buffer_manager_pin(buffer_manager *bufferManager, disk_block *block, memory_buffer *buffer) {
    buffer_manager_find_existing(bufferManager, block, buffer);
    if (buffer == NULL) {
        buffer_manager_find_choose_unpinned_buffer(bufferManager, buffer);
        if (buffer == NULL) {
            return 1;
        }
        buffer_assignto(buffer, block);
    }
    if (buffer_is_pinned(buffer)) {
        bufferManager->numAvailable--;
    }
    buffer_pin(buffer);
}

int buffer_manager_pinnew(buffer_manager *bufferManager, char *fileName, memory_buffer *buffer, table_info *tableInfo) {
    buffer_manager_find_choose_unpinned_buffer(bufferManager, buffer);
    if (buffer == NULL) {
        return -1;
    }
    buffer_assignto_new(buffer, fileName, tableInfo);
    bufferManager->numAvailable--;
    buffer_pin(buffer);
}

int buffer_manager_unpin(buffer_manager *bufferManager, memory_buffer *buffer) {
    buffer_unpin(buffer);
    if (!buffer_is_pinned(buffer)){
        bufferManager->numAvailable++;
    }
}

int buffer_manager_flushall(buffer_manager *bufferManager, int txnum) {
    for (int i = 0; i < BUFFER_MAX_SIZE - 1; i++) {
        if (buffer_is_modifiedby(&(bufferManager->bufferPool[i]), txnum)) {
            buffer_flush(&(bufferManager->bufferPool[i]));
        }
    }
}

int buffer_manager_find_existing(buffer_manager *bufferManager, disk_block *block, memory_buffer *buffer){
    for (int i = 0; i < BUFFER_MAX_SIZE - 1; i++){
        buffer = &(bufferManager->bufferPool[i]);
        disk_block *b = buffer->block;
        if (b!=NULL && b==block){
            return 0;
        }
    }
    return 1;
}

int buffer_manager_find_choose_unpinned_buffer(buffer_manager *bufferManager, memory_buffer *buffer){
    for (int i = 0; i < BUFFER_MAX_SIZE - 1; i++){
        buffer = &(bufferManager->bufferPool[i]);
        if (!buffer_is_pinned(buffer)){
            return 0;
        }
    }
    return 1;
}

int buffer_manager_available(buffer_manager *bufferManager) {
    return bufferManager->numAvailable;
}


int buffer_getint( memory_buffer *buffer, int offset){
    return memory_page_getint(buffer->contents, offset);
};
int buffer_getstring( memory_buffer *buffer, int offset, char *val){
    memory_page_getstring(buffer->contents, offset, val);
};
int buffer_setint( memory_buffer *buffer, int offset, int val, int txnum, int lsn){
    buffer->modifiedBy = txnum;
    if (lsn >=0){
        buffer->logSequenceNumber = lsn;
    }
    memory_page_setint(buffer->contents, offset, val);
};
int buffer_setstring( memory_buffer *buffer, int offset, char *val, int txnum, int lsn){
    buffer->modifiedBy = txnum;
    if (lsn >=0){
        buffer->logSequenceNumber = lsn;
    }
    memory_page_setstring(buffer->contents, offset, val);
};

int buffer_flush( memory_buffer *buffer){
    if (buffer->modifiedBy >=0){
        //log_manager_flush(buffer->logSequenceNumber);
        memory_page_write(buffer->contents, buffer->block);
        buffer->modifiedBy = -1;
    }
};
int buffer_pin( memory_buffer *buffer){
    buffer->pins++;
};
int buffer_unpin( memory_buffer *buffer){
    buffer->pins--;
};
int buffer_is_pinned( memory_buffer *buffer){
    return buffer->pins > 0;
};
int buffer_is_modifiedby( memory_buffer *buffer, int txnum){
    return txnum == buffer->modifiedBy;
}

int buffer_assignto( memory_buffer *buffer, disk_block *block){
    buffer_flush(buffer);
    buffer->block = block;
    memory_page_read(buffer->contents, block);
    buffer->pins = 0;
};

int buffer_assignto_new( memory_buffer *buffer, char *fileName, table_info *tableInfo){
    buffer_flush(buffer);
    //memory_page_record_formatter(buffer->contents, tableInfo);
    //memory_page_append(buffer->contents, fileName, buffer->block);
    buffer->pins = 0;
};

