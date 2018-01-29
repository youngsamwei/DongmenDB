//
// Created by Sam on 2018/1/26.
//

#ifndef DONGMENDB_FILEMANAGER_H
#define DONGMENDB_FILEMANAGER_H

/**
 * 默认的磁盘块的大小，因为测试目的这里设置的过小，一般可以设置为4k
 * 一条记录的总字节长度不能超过块大小。
 */
#include <stdio.h>
#include "dongmengdb.h"

#define DISK_BOLCK_SIZE 400
/**
 * int类型的字节长度
 */
#define INT_SIZE 4

/**
 * 一个数据库对应一个文件夹，一个数据表对应一个文件。
 * 一个文件包含多个磁盘块。
 */
typedef struct memory_buffer_ memory_buffer;

typedef struct file_manager_{
    /**
     * 数据库目录
     */
    FILE *dbDirectory;
    int isNew;
    FILE *openFiles[];
} file_manager;


/**
 * 描述保存在磁盘上的块
 */
typedef struct disk_block_{
    char *fileName;
    int blkNum;
}disk_block;

/**
 * 与磁盘块对应的大小一致的内存页。
 * 将一个磁盘块的内容保存在一一样大小的内存页中。
 */
typedef struct memory_page_{
    unsigned   char contents[DISK_BOLCK_SIZE];
    file_manager *fileManager;
}memory_page;

int file_manager_new(file_manager *fileManager, char *directory, char *dbName);

int file_manager_read(file_manager *fileManager, memory_buffer *buffer, disk_block *diskBlock);

int file_manager_write(file_manager *fileManager, memory_buffer *buffer, disk_block *diskBlock);

int file_manager_append(file_manager *fileManager,char *fileName, memory_buffer *buffer);

int file_manager_size(file_manager *fileManager, char *fileName);

int file_manager_isnew(file_manager *fileManager);

int file_manager_getfile(file_manager *fileManager, char *fileName);


int disk_block_new(char *fileName, int blockNum);

int memory_page_read(memory_page *memoryPage, disk_block *block);

int memory_page_write(memory_page *memoryPage, disk_block *block);

int memory_page_append(memory_page *memoryPage, char *fileName);

int memory_page_getint(memory_page *memoryPage, int offset);
int memory_page_setint(memory_page *memoryPage, int offset, int val);

int memory_page_getstring(memory_page *memoryPage, int offset, char *val);
int memory_page_setstring(memory_page *memoryPage, int offset, char *val);


#endif //DONGMENDB_FILEMANAGER_H
