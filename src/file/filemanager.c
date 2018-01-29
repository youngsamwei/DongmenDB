//
// Created by Sam on 2018/1/26.
//

#include "filemanager.h"


int file_manager_new(file_manager *fileManager, char *directory, char *dbName){};

int file_manager_read(file_manager *fileManager, memory_buffer *buffer, disk_block *diskBlock){};

int file_manager_write(file_manager *fileManager, memory_buffer *buffer, disk_block *diskBlock){};

int file_manager_append(file_manager *fileManager,char *fileName, memory_buffer *buffer){};

int file_manager_size(file_manager *fileManager, char *fileName){};

int file_manager_isnew(file_manager *fileManager){};

int file_manager_getfile(file_manager *fileManager, char *fileName){};


int disk_block_new(char *fileName, int blockNum){};

int memory_page_read(memory_page *memoryPage, disk_block *block){};

int memory_page_write(memory_page *memoryPage, disk_block *block){};

int memory_page_append(memory_page *memoryPage, char *fileName){};

int memory_page_getint(memory_page *memoryPage, int offset){};
int memory_page_setint(memory_page *memoryPage, int offset, int val){};

int memory_page_getstring(memory_page *memoryPage, int offset, char *val){};
int memory_page_setstring(memory_page *memoryPage, int offset, char *val){};
