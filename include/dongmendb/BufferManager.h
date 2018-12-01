//
// Created by Sam on 2018/1/26.
//

#ifndef DONGMENDB_BUFFERMANAGER_H
#define DONGMENDB_BUFFERMANAGER_H

#include "dongmendb.h"

/**
 * 缓存管理
 *
 */
/**
 * 缓冲区的最大等待时间，10s
 */
#define BUFFER_MAX_WAIT_TIME 10000
/**
 * 缓冲区大小，1000块
 */
#define BUFFER_MAX_SIZE 5


typedef void *void_ptr;

typedef struct table_info_ table_info;
typedef struct memory_page_ memory_page;
typedef struct disk_block_ disk_block;
typedef struct file_manager_ file_manager;

class MemoryBuffer {
public:
    memory_page *contents;
    disk_block *block;
    int pins;
    int modifiedBy; //-1,负值表示未被修改
    int logSequenceNumber; // -1 ，负值表示没有对应的日志记录

    MemoryBuffer(file_manager *fileManager);
    int memory_buffer_getint(int offset);
    int memory_buffer_getstring(int offset, char *val);
    int memory_buffer_setint(int offset, int val, int txnum, int lsn);
    int memory_buffer_setstring(int offset, const char *val, int txnum, int lsn);

    int memory_buffer_flush();
    int memory_buffer_pin();
    int memory_buffer_unpin();
    int memory_buffer_is_pinned();
    int memory_buffer_is_modifiedby(int txnum);

    int memory_buffer_assignto(disk_block *block);
    int memory_buffer_assignto_new(char *fileName, table_info *tableInfo);


} ;

class BufferManager {
public:
    MemoryBuffer *bufferPool[BUFFER_MAX_SIZE];
    int numAvailable;

    BufferManager(int bufferSize, file_manager *fileManager);
/**
 * 将缓冲区pin到一个指定的block上。没有考虑异常情况。
 * @param bufferManager
 * @param block
 * @param buffer
 * @return
 */
    int buffer_manager_pin(disk_block *block, void_ptr *buffer);

    int buffer_manager_pinnew(char *fileName, void_ptr *buffer, table_info *tableInfo);

    int buffer_manager_unpin(MemoryBuffer *buffer);

    int buffer_manager_flushall(int txnum);
    int buffer_manager_find_existing(disk_block *block, void_ptr *buffer);
    int buffer_manager_find_choose_unpinned_buffer(void_ptr *buffer);
    int buffer_manager_available(BufferManager *bufferManager);
} ;


#endif //DONGMENDB_BUFFERMANAGER_H
