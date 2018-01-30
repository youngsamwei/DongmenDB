//
// Created by Sam on 2018/1/26.
//

#include <sys/stat.h>
#include <utils.h>
#include <io.h>
#include "filemanager.h"

int file_manager_new(file_manager *fileManager, char *directory, char *dbName) {

    /*如果对应的文件夹不存在*/
    int exists = access(dbName, F_OK);
    if ( exists != 0){
        mkdir(dbName);
    }

    fileManager->dbDirectoryName = dbName;
    fileManager->dbDirectory = fopen(dbName, "rw");
    fileManager->openFiles = hashmap_create();
};

int file_manager_read(file_manager *fileManager, unsigned char *buffer, disk_block *diskBlock) {
    //buffer_clear(buffer);
    memset(buffer, 0, sizeof(buffer));
    FILE *fp = fopen(diskBlock->fileName, "r");
    fseek(fp, diskBlock->blkNum * DISK_BOLCK_SIZE, SEEK_SET);
    fread(buffer, sizeof(char), sizeof(buffer), fp);
    fclose(fp);
};

int file_manager_write(file_manager *fileManager, unsigned char *buffer, disk_block *diskBlock) {
    FILE *fp = fopen(diskBlock->fileName, "w");
    fseek(fp, diskBlock->blkNum * DISK_BOLCK_SIZE, SEEK_SET);
    fwrite(buffer, sizeof(char), sizeof(buffer), fp);
    fclose(fp);
};

int file_manager_append(file_manager *fileManager, char *fileName, unsigned char *buffer, table_info *tableInfo) {
    int newBlockNum = file_manager_size(fileManager, fileName);
    disk_block *diskBlock;
    disk_block_new(fileName, newBlockNum, tableInfo, diskBlock);
    file_manager_write(fileManager, buffer, diskBlock);
};

int file_manager_size(file_manager *fileManager, char *fileName) {
    struct _stat stats;
    int result = _stat(fileName, &stats);
    if (result != 0){
        return 0;
    }
    return stats.st_size / DISK_BOLCK_SIZE;
};

int file_manager_isnew(file_manager *fileManager) {
    return fileManager->isNew;
};

int file_manager_getfile(file_manager *fileManager, char *fileName, FILE *fp) {
    void_ptr *file;
    hashmap_get(fileManager->openFiles, fileName, file);
    if (file == NULL) {
        char *fname;
        strcat(fname, fileManager->dbDirectoryName);
        strcat(fname, "/");
        strcat(fname, fileName);
        fp = fopen(fname, "rw");
        hashmap_put(fileManager->openFiles, fileName, fp);
    }
    fp = (FILE *) file;
    return 1;
};

int disk_block_new(char *fileName, int blockNum, table_info *tableInfo, disk_block *diskBlock) {
    diskBlock = (disk_block *) malloc(sizeof(disk_block));
    diskBlock->fileName = fileName;
    diskBlock->blkNum = blockNum;
    diskBlock->tableInfo = tableInfo;
    return 1;
};

char *disk_block_get_num_string(disk_block *block){
    char *blockNum;
    itoa(block->blkNum, blockNum, 10);
    char *blockName = strcat(block->tableInfo->tableName,blockNum);
    return blockName;
};

int memory_page_create(memory_page *memoryPage, file_manager *fileManager){

    memoryPage->fileManager =fileManager;
    return 1;
};

int memory_page_read(memory_page *memoryPage, disk_block *block) {
    file_manager_read(memoryPage->fileManager, memoryPage->contents, block);
};

int memory_page_write(memory_page *memoryPage, disk_block *block) {
    file_manager_write(memoryPage->fileManager, memoryPage->contents, block);
};

int memory_page_append(memory_page *memoryPage, char *fileName, table_info *tableInfo) {
    file_manager_append(memoryPage->fileManager, fileName, memoryPage->contents, tableInfo);
};

int memory_page_record_formatter(memory_page *contents, table_info *tableInfo) {
    int recsize = tableInfo->recordLen + INT_SIZE;
    for (int pos = 0; pos + recsize < DISK_BOLCK_SIZE; pos+=recsize){
        memory_page_setint(contents, pos, RECORD_PAGE_EMPTY);
        for (int i = 0; i <= tableInfo->fieldsName.size - 1; i ++){
            char *fieldName = (char *)array_list_get(&tableInfo->fieldsName, i);

            void_ptr *fielddesc;
            hashmap_get(tableInfo->fields, fieldName, fielddesc);
            field_info *fieldInfo =  *fielddesc;

            void_ptr *ofvalue ;
            hashmap_get(tableInfo->offsets, fieldName, ofvalue);
            integer *offset  =  *ofvalue;

            if(fieldInfo->type == DATA_TYPE_INT){
                memory_page_setint(contents, pos + INT_SIZE + offset->val, 0);
            }else{
                memory_page_setstring(contents, pos + INT_SIZE + offset->val, "");
            }
        }
    }
}

int memory_page_getint(memory_page *memoryPage, int offset) {
    return bytes2int(memoryPage->contents[offset],
                     memoryPage->contents[offset + 1],
                     memoryPage->contents[offset + 2],
                     memoryPage->contents[offset + 3]
    );

};

int memory_page_setint(memory_page *memoryPage, int offset, int val) {
    memoryPage->contents[offset] = val >> 24;
    memoryPage->contents[offset + 1] = val >> 16;
    memoryPage->contents[offset + 2] = val >> 8;
    memoryPage->contents[offset + 3] = val;
};

int memory_page_getstring(memory_page *memoryPage, int offset, char *val) {
    int len = memory_page_getint(memoryPage, offset);
    val = (char *) malloc(sizeof(char));
    memcpy(val, memoryPage->contents, len);
    return 1;
};

int memory_page_setstring(memory_page *memoryPage, int offset, char *val) {
    int len = strlen(val);
    memory_page_setint(memoryPage, offset, len);
    memcpy(memoryPage->contents + offset + 1, val, sizeof(val));
    return 1;
};
