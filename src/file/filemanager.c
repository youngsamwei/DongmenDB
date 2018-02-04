//
// Created by Sam on 2018/1/26.
//

#include <sys/stat.h>
#include <utils.h>
#include <io.h>
#include <arraylist.h>
#include "filemanager.h"

int file_manager_new(file_manager *fileManager, char *directory, char *dbName) {
    fileManager->isNew = 0;
    /*如果对应的文件夹不存在*/
    int exists = access(dbName, F_OK);
    if (exists != 0) {
        fileManager->isNew = 1;
        mkdir(dbName);
    }

    fileManager->dbDirectoryName = dbName;
    fileManager->dbDirectory = fopen(dbName, "rw");
    fileManager->openFiles = hashmap_create();
};

int file_manager_read(file_manager *fileManager, memory_page *memoryPage, disk_block *diskBlock) {
    void_ptr *pfile = (void_ptr *) malloc(sizeof(void_ptr *));
    file_manager_getfile(fileManager, diskBlock->fileName, pfile);
    FILE *fp = *pfile;
    //memset(memoryPage->contents, 0, sizeof(memoryPage->contents));
    fseek(fp, diskBlock->blkNum * DISK_BOLCK_SIZE, SEEK_SET);
    fread(memoryPage->contents, sizeof(char), sizeof(memoryPage->contents), fp);
};

int file_manager_write(file_manager *fileManager, memory_page *memoryPage, disk_block *diskBlock) {
    void_ptr *pfile = (void_ptr *) calloc(sizeof(void_ptr *), 1);
    file_manager_getfile(fileManager, diskBlock->fileName, pfile);
    FILE *fp = *pfile;
    fseek(fp, diskBlock->blkNum * DISK_BOLCK_SIZE, SEEK_SET);
    int size = sizeof(memoryPage->contents);
    fwrite(memoryPage->contents, sizeof(char), size, fp);
};

int file_manager_append(file_manager *fileManager, memory_buffer *memoryBuffer, char *fileName, table_info *tableInfo) {
    int newBlockNum = file_manager_size(fileManager, fileName);
    disk_block *diskBlock = (disk_block *) calloc(sizeof(disk_block), 1);

    memoryBuffer->block = diskBlock;
    disk_block_new(fileName, newBlockNum, tableInfo, diskBlock);
    file_manager_write(fileManager, memoryBuffer->contents, diskBlock);
};

int file_manager_size(file_manager *fileManager, char *fileName) {
    void_ptr *pfile = (void_ptr)malloc(sizeof(void_ptr));
    file_manager_getfile(fileManager, fileName, pfile);

    FILE *file = *pfile;

    fseek(file,0L,SEEK_END); /* 定位到文件末尾 */
    int flen=ftell(file); /* 得到文件大小 */
    return flen / DISK_BOLCK_SIZE;
}

int file_manager_isnew(file_manager *fileManager) {
    return fileManager->isNew;
};

int file_manager_getfile(file_manager *fileManager, char *fileName, void_ptr *file) {
    int found = hashmap_get(fileManager->openFiles, fileName, file);

    if (found == HMAP_E_NOTFOUND) {
        char *fname = new_id_name();
        strcat(fname, fileManager->dbDirectoryName);
        strcat(fname, "/");
        strcat(fname, fileName);
        FILE *f = fopen(fname, "rwb+");
        if (f == NULL) {
            f = fopen(fname, "wb+");
            if (f == NULL) {
                /*TODO:error*/
                return DONGMENGDB_ERROR_IO;
            }
            fclose(f);
            f = fopen(fname, "rwb+");
        }

        hashmap_put(fileManager->openFiles, fileName, f);
        *file = f;
    }

    return DONGMENGDB_OK;
};

int disk_block_new(char *fileName, int blockNum, table_info *tableInfo, disk_block *diskBlock) {

    diskBlock->fileName = fileName;
    diskBlock->blkNum = blockNum;
    diskBlock->tableInfo = tableInfo;

    return 1;
};

char *disk_block_get_num_string(disk_block *block) {
    char *blockNum = new_id_name();
    itoa(block->blkNum, blockNum, 10);

    char *blockName = new_id_name();
    strcat(blockName, block->tableInfo->tableName);
    strcat(blockName, blockNum);
    return blockName;
};

int memory_page_create(memory_page *memoryPage, file_manager *fileManager) {
    memset(memoryPage->contents, 0, sizeof(memoryPage->contents));
    memoryPage->fileManager = fileManager;
    return 1;
};

int memory_page_read(memory_page *memoryPage, disk_block *block) {
    file_manager_read(memoryPage->fileManager, memoryPage, block);
};

int memory_page_write(memory_page *memoryPage, disk_block *block) {
    file_manager_write(memoryPage->fileManager, memoryPage, block);
};

int memory_page_append(memory_buffer *memoryBuffer, char *fileName, table_info *tableInfo) {
    file_manager_append(memoryBuffer->contents->fileManager, memoryBuffer, fileName, tableInfo);
};

int memory_page_record_formatter(memory_page *contents, table_info *tableInfo) {
//    memset(contents->contents, 0, DISK_BOLCK_SIZE);
    int recsize = tableInfo->recordLen + INT_SIZE;
    for (int pos = 0; pos + recsize <= DISK_BOLCK_SIZE; pos += recsize) {
        memory_page_setint(contents, pos, RECORD_PAGE_EMPTY);
        int count = tableInfo->fieldsName->size - 1;
        for (int i = 0; i <= count; i++) {
            char *fieldName = (char *) arraylist_get(tableInfo->fieldsName, i);

            void_ptr *fielddesc = (void_ptr *) malloc(sizeof(void_ptr *));
            hashmap_get(tableInfo->fields, fieldName, fielddesc);
            field_info *fieldInfo = *fielddesc;

            int offset = table_info_offset(tableInfo, fieldName);

            if (fieldInfo->type == DATA_TYPE_INT) {
                memory_page_setint(contents, pos + INT_SIZE + offset, 0);
            } else {
                memory_page_setstring(contents, pos + INT_SIZE + offset, " ");
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
//    val = (char *) malloc(len);
//    memset(val, 0, len);
    memcpy(val, memoryPage->contents + offset + INT_SIZE, len);
    return 1;
};

int memory_page_setstring(memory_page *memoryPage, int offset, char *val) {
    int len = strlen(val);
    memory_page_setint(memoryPage, offset, len);
    memcpy(memoryPage->contents + offset + INT_SIZE, val, len);
    return 1;
};
