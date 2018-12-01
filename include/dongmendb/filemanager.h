//
// Created by Sam on 2018/1/26.
//

#ifndef DONGMENDB_FILEMANAGER_H
#define DONGMENDB_FILEMANAGER_H

/**
 * 以block为单位的数据管理。但meemorypage属于缓存管理。
 * 默认的磁盘块的大小，因为测试目的这里设置的过小，一般可以设置为4k
 * 一条记录的总字节长度不能超过块大小。
 *
 */
#include <stdio.h>
#include "dongmendb.h"

#include <map>

using  namespace std;

#define DISK_BOLCK_SIZE 400
/**
 * int类型的字节长度
 */
#define INT_SIZE 4

/**
 * 一个数据库对应一个文件夹，一个数据表对应一个文件。
 * 一个文件包含多个磁盘块。
 */
class MemoryBuffer;
class FileManager;
typedef struct table_info_ table_info;

/**
 * 描述保存在磁盘上的块
 */
class DiskBlock {
public:
    table_info *tableInfo;
    char *fileName;
    int blkNum;


    DiskBlock(char *fileName, int blockNum, table_info *tableInfo);

    char *disk_block_get_num_string();
} ;

/**
 * 与磁盘块对应的大小一致的内存页。
 * 将一个磁盘块的内容保存在一一样大小的内存页中。
 */
typedef struct memory_page_ {
    unsigned char contents[DISK_BOLCK_SIZE];
    FileManager *fileManager;
} memory_page;


class FileManager {
public:
    /**
     * 数据库目录
     */
    char *dbDirectoryName;
    FILE *dbDirectory;
    int isNew;
    map<string, FILE*> *openFiles;


    FileManager(char *directory, const char *dbName);

    int file_manager_read(memory_page *memoryPage, DiskBlock *diskBlock);

    int file_manager_write(memory_page *memoryPage, DiskBlock *diskBlock);

    int file_manager_append(MemoryBuffer *memoryBuffer, char *fileName, table_info *tableInfo);

    int file_manager_size(char *fileName);

    int file_manager_isnew(FileManager *fileManager);

    FILE* file_manager_getfile(char *fileName);

    int file_manager_closefile(char *fileName);

    int file_manager_closeallfile();
} ;



int memory_page_create(memory_page *memoryPage, FileManager *fileManager);

int memory_page_read(memory_page *memoryPage, DiskBlock *block);

int memory_page_write(memory_page *memoryPage, DiskBlock *block);

int memory_page_append(MemoryBuffer *memoryBuffer, char *fileName, table_info *tableInfo);
int memory_page_record_formatter(memory_page *content, table_info *tableInfo);
int memory_page_getint(memory_page *memoryPage, int offset);
int memory_page_setint(memory_page *memoryPage, int offset, int val);

int memory_page_getstring(memory_page *memoryPage, int offset, char *val);
int memory_page_setstring(memory_page *memoryPage, int offset, const char *val);



#endif //DONGMENDB_FILEMANAGER_H
