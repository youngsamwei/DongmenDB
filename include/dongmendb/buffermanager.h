//
// Created by Sam on 2018/1/26.
//

#ifndef DONGMENDB_BUFFERMANAGER_H
#define DONGMENDB_BUFFERMANAGER_H

#include "dongmendb.h"
#ifdef __cplusplus
extern "C" {
#endif

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

typedef struct memory_buffer_ {
    memory_page *contents;
    disk_block *block;
    int pins;
    int modifiedBy; //-1,负值表示未被修改
    int logSequenceNumber; // -1 ，负值表示没有对应的日志记录
} memory_buffer;

typedef struct buffer_manager_ {
    memory_buffer *bufferPool[BUFFER_MAX_SIZE];
    int numAvailable;
} buffer_manager;

int buffer_manager_create(buffer_manager *bufferManager, int bufferSize, file_manager *fileManager);
/**
 * 将缓冲区pin到一个指定的block上。没有考虑异常情况。
 * @param bufferManager
 * @param block
 * @param buffer
 * @return
 */
int buffer_manager_pin(buffer_manager *bufferManager, disk_block *block, void_ptr *buffer);

int buffer_manager_pinnew(buffer_manager *bufferManager, char *fileName, void_ptr *buffer, table_info *tableInfo);

int buffer_manager_unpin(buffer_manager *bufferManager, memory_buffer *buffer);

int buffer_manager_flushall(buffer_manager *bufferManager, int txnum);
int buffer_manager_find_existing(buffer_manager *bufferManager, disk_block *block, void_ptr *buffer);
int buffer_manager_find_choose_unpinned_buffer(buffer_manager *bufferManager, void_ptr *buffer);
int buffer_manager_available(buffer_manager *bufferManager);

int memory_buffer_create(memory_buffer *buffer, file_manager *fileManager);
int memory_buffer_getint(memory_buffer *buffer, int offset);
int memory_buffer_getstring(memory_buffer *buffer, int offset, char *val);
int memory_buffer_setint(memory_buffer *buffer, int offset, int val, int txnum, int lsn);
int memory_buffer_setstring(memory_buffer *buffer, int offset, char *val, int txnum, int lsn);

int memory_buffer_flush(memory_buffer *buffer);
int memory_buffer_pin(memory_buffer *buffer);
int memory_buffer_unpin(memory_buffer *buffer);
int memory_buffer_is_pinned(memory_buffer *buffer);
int memory_buffer_is_modifiedby(memory_buffer *buffer, int txnum);

int memory_buffer_assignto(memory_buffer *buffer, disk_block *block);
int memory_buffer_assignto_new(memory_buffer *buffer, char *fileName, table_info *tableInfo);
#ifdef __cplusplus
}
#endif

#endif //DONGMENDB_BUFFERMANAGER_H
