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

#define DISK_BOLCK_SIZE 400
/**
 * int类型的字节长度
 */
#define INT_SIZE 4

/**
 * 一个数据库对应一个文件夹，一个数据表对应一个文件。
 * 一个文件包含多个磁盘块。
 */

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

#endif //DONGMENDB_FILEMANAGER_H
