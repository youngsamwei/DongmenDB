//
// Created by Sam on 2018/1/26.
//


#include <dongmendb/BufferManager.h>

 BufferManager::BufferManager(int bufferSize, FileManager *fileManager) {

    this->numAvailable = bufferSize;
    for (int i = 0; i <= bufferSize - 1 ; i++){
        this->bufferPool[i] = new MemoryBuffer(fileManager);
    }
};

int BufferManager::buffer_manager_pin(DiskBlock *block, void_ptr *buffer) {
    /*先查找block是否已经在缓存中*/
    buffer_manager_find_existing(block, buffer);
    MemoryBuffer *buf = (MemoryBuffer *)*buffer;
    if (*buffer == NULL) {
        buffer_manager_find_choose_unpinned_buffer(buffer);
        if (*buffer == NULL) {
            return -1;
        }
        buf = (MemoryBuffer *)*buffer;
        buf->memory_buffer_assignto( block);
    }
    if (!buf->memory_buffer_is_pinned()) {
        this->numAvailable--;
    }
    buf->memory_buffer_pin();
}

int BufferManager::buffer_manager_pinnew(char *fileName, void_ptr *buffer, table_info *tableInfo) {
    buffer_manager_find_choose_unpinned_buffer(buffer);
    if (*buffer == NULL) {
        return -1;
    }
    MemoryBuffer *buf = (MemoryBuffer *)*buffer;
    buf->memory_buffer_assignto_new(fileName, tableInfo);
    this->numAvailable--;
    buf-> memory_buffer_pin();
}

int BufferManager::buffer_manager_unpin(MemoryBuffer *buffer) {
    buffer-> memory_buffer_unpin();
    if (!buffer->memory_buffer_is_pinned()) {
        this->numAvailable++;
    }
}

int BufferManager::buffer_manager_flushall(int txnum) {
    for (int i = 0; i <= BUFFER_MAX_SIZE - 1; i++) {
        if (this->bufferPool[i]->memory_buffer_is_modifiedby(txnum)) {
            this->bufferPool[i]->memory_buffer_flush();
        }
    }
}

/*这里的查找算法效率低，可以修改为效率高的算法.*/
int BufferManager::buffer_manager_find_existing(DiskBlock *block, void_ptr *buffer) {
    for (int i = 0; i <= BUFFER_MAX_SIZE - 1; i++) {
        MemoryBuffer *buf = this->bufferPool[i];
        DiskBlock *b = buf->block;
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

 MemoryBuffer::MemoryBuffer(FileManager *fileManager) {
    this->contents = new MemoryPage(fileManager);
    this->pins = 0;
    this->modifiedBy = -1;
    this->logSequenceNumber = -1;
    this->block=NULL;
};

int MemoryBuffer::memory_buffer_getint(int offset) {
    return this->contents->memory_page_getint(offset);
};

int MemoryBuffer::memory_buffer_getstring(int offset, char *val) {
    this->contents->memory_page_getstring(offset, val);
};

int MemoryBuffer::memory_buffer_setint(int offset, int val, int txnum, int lsn) {
    this->modifiedBy = txnum;
    if (lsn >= 0) {
        this->logSequenceNumber = lsn;
    }
    this->contents-> memory_page_setint(offset, val);
};

int MemoryBuffer::memory_buffer_setstring(int offset, const char *val, int txnum, int lsn) {
    this->modifiedBy = txnum;
    if (lsn >= 0) {
        this->logSequenceNumber = lsn;
    }
    this->contents->memory_page_setstring(offset, val);
};

int MemoryBuffer::memory_buffer_flush() {
    if (this->modifiedBy >= 0) {
        //log_manager_flush(this->logSequenceNumber);
        this->contents-> memory_page_write(this->block);
        this->modifiedBy = -1;
    }

};

int MemoryBuffer::memory_buffer_pin() {
    this->pins++;
};

int MemoryBuffer::memory_buffer_unpin() {
    this->pins--;
};

int MemoryBuffer::memory_buffer_is_pinned() {
    return this->pins > 0;
};

int MemoryBuffer::memory_buffer_is_modifiedby(int txnum) {
    return txnum == this->modifiedBy;
}

int MemoryBuffer::memory_buffer_assignto(DiskBlock *block) {
    memory_buffer_flush();
    this->block = block;
    this->contents->memory_page_read(block);
    this->pins = 0;
};

int MemoryBuffer::memory_buffer_assignto_new(char *fileName, table_info *tableInfo) {
    memory_buffer_flush();
    this->contents->memory_page_record_formatter(tableInfo);
    this->contents->memory_page_append(this,fileName, tableInfo);
    this->pins = 0;
};

