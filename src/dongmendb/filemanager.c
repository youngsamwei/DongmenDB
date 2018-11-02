//
// Created by Sam on 2018/1/26.
//

#include <sys/stat.h>
#include <utils/utils.h>
#include <io.h>
#include <utils/arraylist.h>
#include "dongmendb/filemanager.h"

int file_manager_new(file_manager *fileManager, char *directory, const char *dbName) {
    fileManager->isNew = 0;
    /*如果对应的文件夹不存在*/
    int exists = access(dbName, F_OK);
    if (exists != 0) {
        fileManager->isNew = 1;
        mkdir(dbName);
    }

    fileManager->dbDirectoryName = strdup(dbName);
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
                return DONGMENDB_ERROR_IO;
            }
            fclose(f);
            f = fopen(fname, "rwb+");
        }

        hashmap_put(fileManager->openFiles, fileName, f);
        *file = f;
    }

    return DONGMENDB_OK;
};

int file_manager_closefile(file_manager *fileManager, char *fileName){
    void_ptr *file;
    int found = hashmap_get(fileManager->openFiles, fileName, file);
    if (found == HMAP_E_KEYUSED){
        fclose((FILE *)file);
    }
    hashmap_remove(fileManager->openFiles, fileName, file);
}

int _iter_closefile(char * filename, void_ptr fnIterValue, void_ptr arg){
    fclose((FILE *)fnIterValue);
}

int file_manager_closeallfile(file_manager *fileManager){
    hashmap_iterate(fileManager->openFiles, _iter_closefile, NULL);
    hashmap_clear(fileManager->openFiles);
}

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

/**
 * recordlen由整型数据的INTSIZE和字符数据的LEN + INTSIZE构成。
 *
 * @param contents
 * @param tableInfo
 * @return
 */
int memory_page_record_formatter(memory_page *contents, table_info *tableInfo) {
//    memset(contents->contents, 0, DISK_BOLCK_SIZE);
    /*保留一个整型位置保存slot的使用状态,默认RECORD_PAGE_EMPTY*/
    int recsize = tableInfo->recordLen + INT_SIZE;
    for (int pos = 0; pos + recsize <= DISK_BOLCK_SIZE; pos += recsize) {
        /*先写slot的使用状态*/
        memory_page_setint(contents, pos, RECORD_PAGE_EMPTY);
        /*计算当前记录的偏移量, +slot的使用状态的整型值偏移量*/
        int recoffset = pos + INT_SIZE;
        int count = tableInfo->fieldsName->size - 1;
        for (int i = 0; i <= count; i++) {
            char *fieldName = (char *) arraylist_get(tableInfo->fieldsName, i);

            void_ptr *fielddesc = (void_ptr *) malloc(sizeof(void_ptr *));
            hashmap_get(tableInfo->fields, fieldName, fielddesc);
            field_info *fieldInfo = *fielddesc;

            int offset = table_info_offset(tableInfo, fieldName);

            if (fieldInfo->type == DATA_TYPE_INT) {
                memory_page_setint(contents, recoffset + offset, 0);
            } else {
                memory_page_setstring(contents, recoffset + offset, " ");
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

/**
 * 先读取字符串长度，再根据长度读取字符串
 * @param memoryPage
 * @param offset 偏移量
 * @param val 返回结果
 * @return 返回状态
 */
int memory_page_getstring(memory_page *memoryPage, int offset, char *val) {
    int len = memory_page_getint(memoryPage, offset);
    /*增加一个整型的偏移量*/
    offset += INT_SIZE;
//    val = (char *) malloc(len);
//    memset(val, 0, len);
    memcpy(val, memoryPage->contents + offset , len);
    val[len]='\0';
    return 1;
};

/**
 * 写字符串
 * @param memoryPage
 * @param offset 偏移量
 * @param val 要写的值
 * @return 返回状态
 */
int memory_page_setstring(memory_page *memoryPage, int offset, char *val) {
    int len = strlen(val);
    /*先写字符串长度*/
    memory_page_setint(memoryPage, offset, len);
    /*增加一个整型的偏移量*/
    offset += INT_SIZE;
    memcpy(memoryPage->contents + offset , val, len);
    return 1;
};
