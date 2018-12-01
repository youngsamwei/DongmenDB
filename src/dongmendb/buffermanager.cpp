//
// Created by Sam on 2018/1/26.
//


#include <dongmendb/buffermanager.h>

 BufferManager::BufferManager(int bufferSize, file_manager *fileManager) {

    this->numAvailable = bufferSize;
    for (int i = 0; i <= bufferSize - 1 ; i++){
        this->bufferPool[i] = (memory_buffer *)malloc(sizeof(memory_buffer));
        memory_buffer_create( this->bufferPool[i], fileManager);
    }
};

int BufferManager::buffer_manager_pin(disk_block *block, void_ptr *buffer) {
    /*先查找block是否已经在缓存中*/
    buffer_manager_find_existing(block, buffer);
    memory_buffer *buf = (memory_buffer *)*buffer;
    if (*buffer == NULL) {
        buffer_manager_find_choose_unpinned_buffer(buffer);
        if (*buffer == NULL) {
            return -1;
        }
        buf = (memory_buffer *)*buffer;
        memory_buffer_assignto(buf, block);
    }
    if (!memory_buffer_is_pinned(buf)) {
        this->numAvailable--;
    }
    memory_buffer_pin(buf);
}

int BufferManager::buffer_manager_pinnew(char *fileName, void_ptr *buffer, table_info *tableInfo) {
    buffer_manager_find_choose_unpinned_buffer(buffer);
    if (*buffer == NULL) {
        return -1;
    }
    memory_buffer *buf = (memory_buffer *)*buffer;
    memory_buffer_assignto_new(buf, fileName, tableInfo);
    this->numAvailable--;
    memory_buffer_pin(buf);
}

int BufferManager::buffer_manager_unpin(memory_buffer *buffer) {
    memory_buffer_unpin(buffer);
    if (!memory_buffer_is_pinned(buffer)) {
        this->numAvailable++;
    }
}

int BufferManager::buffer_manager_flushall(int txnum) {
    for (int i = 0; i <= BUFFER_MAX_SIZE - 1; i++) {
        if (memory_buffer_is_modifiedby(this->bufferPool[i], txnum)) {
            memory_buffer_flush(this->bufferPool[i]);
        }
    }
}

/*这里的查找算法效率低，可以修改为效率高的算法.*/
int BufferManager::buffer_manager_find_existing(disk_block *block, void_ptr *buffer) {
    for (int i = 0; i <= BUFFER_MAX_SIZE - 1; i++) {
        memory_buffer *buf = this->bufferPool[i];
        disk_block *b = buf->block;
        if (b != NULL && stricmp(b->fileName, block->fileName)==0 && (b->blkNum == block->blkNum)) {
            *buffer = buf;
            return 0;
        }
    }
    *buffer = NULL;
    return 1;
}

int BufferManager::buffer_manager_find_choose_unpinned_buffer(void_ptr *buffer) {
    for (int i = 0; i <= BUFFER_MAX_SIZE - 1; i++) {
        if (!this->bufferPool[i]->pins > 0) {
            *buffer = (void_ptr)this->bufferPool[i];
            return 1;
        }
    }
    return 0;
}

int BufferManager::buffer_manager_available(BufferManager *bufferManager) {
    return this->numAvailable;
}

int memory_buffer_create(memory_buffer *buffer, file_manager *fileManager) {
    buffer->contents = (memory_page *)malloc(sizeof(memory_page));
    buffer->pins = 0;
    buffer->modifiedBy = -1;
    buffer->logSequenceNumber = -1;
    buffer->block=NULL;
    memory_page_create(buffer->contents, fileManager);
    return 1;
};

int memory_buffer_getint(memory_buffer *buffer, int offset) {
    return memory_page_getint(buffer->contents, offset);
};

int memory_buffer_getstring(memory_buffer *buffer, int offset, char *val) {
    memory_page_getstring(buffer->contents, offset, val);
};

int memory_buffer_setint(memory_buffer *buffer, int offset, int val, int txnum, int lsn) {
    buffer->modifiedBy = txnum;
    if (lsn >= 0) {
        buffer->logSequenceNumber = lsn;
    }
    memory_page_setint(buffer->contents, offset, val);
};

int memory_buffer_setstring(memory_buffer *buffer, int offset, const char *val, int txnum, int lsn) {
    buffer->modifiedBy = txnum;
    if (lsn >= 0) {
        buffer->logSequenceNumber = lsn;
    }
    memory_page_setstring(buffer->contents, offset, val);
};

int memory_buffer_flush(memory_buffer *buffer) {
    if (buffer->modifiedBy >= 0) {
        //log_manager_flush(buffer->logSequenceNumber);
        memory_page_write(buffer->contents, buffer->block);
        buffer->modifiedBy = -1;
    }

};

int memory_buffer_pin(memory_buffer *buffer) {
    buffer->pins++;
};

int memory_buffer_unpin(memory_buffer *buffer) {
    buffer->pins--;
};

int memory_buffer_is_pinned(memory_buffer *buffer) {
    return buffer->pins > 0;
};

int memory_buffer_is_modifiedby(memory_buffer *buffer, int txnum) {
    return txnum == buffer->modifiedBy;
}

int memory_buffer_assignto(memory_buffer *buffer, disk_block *block) {
    memory_buffer_flush(buffer);
    buffer->block = block;
    memory_page_read(buffer->contents, block);
    buffer->pins = 0;
};

int memory_buffer_assignto_new(memory_buffer *buffer, char *fileName, table_info *tableInfo) {
    memory_buffer_flush(buffer);
    memory_page_record_formatter(buffer->contents, tableInfo);
    memory_page_append(buffer, fileName, tableInfo);
    buffer->pins = 0;
};

