//
// Created by Sam on 2018/1/26.
//

#include "recordfile.h"

int record_file_create(record_file *recordFile, table_info *tableInfo,
                    transaction *tx) {

    recordFile->tableInfo = tableInfo;
    recordFile->tx = tx;

    char *fileName = (char *)malloc(MAX_ID_NAME_LENGTH);
    memset(fileName, 0, sizeof(fileName));
    strcpy(fileName,tableInfo->tableName);
    strcat(fileName,RECORD_FILE_EXT);

    recordFile->fileName = fileName;
    if (transaction_size(tx, recordFile->fileName) == 0) {
        record_file_append_block(recordFile);
    }
    record_file_moveto(recordFile, 0);
}

int record_file_close(record_file *recordFile) {
    record_page_close(recordFile->recordPage);
}

int record_file_before_first(record_file *recordFile) {
    record_file_moveto(recordFile, 0);
}

int record_file_next(record_file *recordFile) {
    while (1) {
        if (record_page_next(recordFile->recordPage)) {
            return 1;
        }
        if (record_file_atlast(recordFile)) {
            return 1;
        }
        record_file_moveto(recordFile, recordFile->currentBlkNum + 1);
    }
}

int record_file_atlast(record_file *recordFile){
    return recordFile->currentBlkNum == transaction_size(recordFile->tx, recordFile->fileName) - 1;
}

int record_file_get_int(record_file *recordFile, char *fieldName) {
    return record_page_getint(recordFile->recordPage, fieldName);
}

int record_file_get_string(record_file *recordFile, char *fieldName, char *value) {
    return record_page_getstring(recordFile->recordPage, fieldName, value);
}

int record_file_set_int(record_file *recordFile, char *fieldName, int value) {
    return record_page_setint(recordFile->recordPage, fieldName, value);
}

int record_file_set_string(record_file *recordFile, char *fieldName, char *value) {
    return record_page_setstring(recordFile->recordPage, fieldName, value);
}

int record_file_delete(record_file *recordFile) {
    return record_page_delete(recordFile->recordPage);
}

int record_file_insert(record_file *recordFile) {
    while (!record_page_insert(recordFile->recordPage)) {
        if (record_file_atlast_block(recordFile)) {
            record_file_append_block(recordFile);
        }
        record_file_moveto(recordFile, recordFile->currentBlkNum + 1);
    }
}

int record_file_moveto_recordid(record_file *recordFile, record_id *recordId) {
    record_file_moveto(recordFile, recordId->blockNum);
    record_page_moveto_id(recordFile->recordPage, recordId->id);
}

int record_file_current_recordid(record_file *recordFile, record_id *recordId) {
    recordId = (record_id *) malloc(sizeof(recordId));
    recordId->id = recordFile->recordPage->currentSlot;
    recordId->blockNum = recordFile->currentBlkNum;
    return 0;
}

int record_file_moveto(record_file *recordFile, int blockNum) {
    if (recordFile->recordPage != NULL) {
        record_page_close(recordFile->recordPage);
    }
    recordFile->currentBlkNum = blockNum;
    disk_block *diskBlock = (disk_block *) malloc(sizeof(disk_block));
    record_page *recordPage = (record_page *) malloc(sizeof(record_page));
    recordPage->diskBlock = diskBlock;
    recordPage->tx = recordFile->tx;
    recordPage->tableInfo = recordFile->tableInfo;

    recordFile->recordPage = recordPage;
    return 0;
}

int record_file_atlast_block(record_file *recordFile) {
    return recordFile->currentBlkNum == transaction_size(recordFile->tx, recordFile->fileName) - 1;

}

int record_file_append_block(record_file *recordFile) {
    transaction_append(recordFile->tx, recordFile->fileName, recordFile->tableInfo);
}

/**
 * from RecordFormatter.format&makeDefaultRecord
 * @param recordFile
 * @param memoryPage
 * @return
 */
int record_file_record_formatter(record_file *recordFile, memory_page *memoryPage) {

}

int field_info_create(field_info *fieldInfo, DATA_TYPE type, int length){

    fieldInfo->type=type;
    fieldInfo->length=length;
    return 1;
};

int table_info_create(table_info *tableInfo, char *tableName, array_list fieldsName, hmap_t fields){

    tableInfo->tableName = tableName;
    tableInfo->fieldsName = fieldsName;
    tableInfo->fields = fields;
    tableInfo->offsets = hashmap_create();
    tableInfo->recordLen = 0;
    int pos = 0;

    for (int i = 0; i <= fieldsName.size - 1; i++){
        char *fieldName = (char *)array_list_get(&fieldsName, i);

        void_ptr *value;
        hashmap_get(tableInfo->fields, fieldName, value );
        field_info *fieldInfo = *value;

        integer *ipos = (integer *)malloc(sizeof(integer));
        ipos->val = pos;
        hashmap_put(tableInfo->offsets, fieldName, ipos);

        pos += fieldInfo->length;
    }
    tableInfo->recordLen = pos;
};

int record_page_close(record_page *recordPage) {
    if (recordPage->diskBlock!=NULL){
        transaction_unpin(recordPage->tx,recordPage->diskBlock);
        recordPage->diskBlock = NULL;
    }
}

int record_page_insert(record_page *recordPage) {
    recordPage->currentSlot = -1;
    if (record_page_searchfor(recordPage, RECORD_PAGE_EMPTY)){
        int position = record_page_current_pos(recordPage);
        transaction_setint(recordPage->tx,recordPage->diskBlock, position, RECORD_PAGE_INUSE);
        return 1;
    }
    return 0;
}

int record_page_moveto_id(record_page *recordPage, int id) {
    recordPage->currentSlot = id;
}

int record_page_next(record_page *recordPage){
    record_page_searchfor(recordPage, RECORD_PAGE_INUSE);
};

int record_page_getint(record_page *recordPage, char *fieldName){
    int position  = record_page_fieldpos(recordPage, fieldName);
    return transaction_getint(recordPage->tx, recordPage->diskBlock, position);
};

int record_page_getstring(record_page *recordPage, char *fieldName, char *value){
    int position  = record_page_fieldpos(recordPage, fieldName);
    return transaction_getstring(recordPage->tx,recordPage->diskBlock, position, value);
};

int record_page_setint(record_page *recordPage, char *fieldName, int value){
    int position  = record_page_fieldpos(recordPage, fieldName);
    return transaction_setint(recordPage->tx,recordPage->diskBlock, position, value);
};

int record_page_setstring(record_page *recordPage, char *fieldName, char *value){
    int position  = record_page_fieldpos(recordPage, fieldName);
    return transaction_setstring(recordPage->tx,recordPage->diskBlock, position, value);
};

int record_page_delete(record_page *recordPage){
    int position = record_page_current_pos(recordPage);
    return transaction_setint(recordPage->tx,recordPage->diskBlock, position, RECORD_PAGE_EMPTY);
};

int record_page_searchfor(record_page *recordPage, record_page_status status){
    recordPage->currentSlot++;
    while (record_page_is_valid_slot(recordPage)){
        int position = record_page_current_pos(recordPage);
        if (transaction_getint(recordPage->tx,recordPage->diskBlock, position)==status){
            return 1;
        }
        recordPage->currentSlot++;
    }
    return 0;
};

int record_page_current_pos(record_page *recordPage){
    return recordPage->currentSlot * recordPage->slotSize;
};

int record_page_fieldpos(record_page *recordPage, char *fieldName){};

int record_page_is_valid_slot(record_page *recordPage){
    return record_page_current_pos(recordPage) + recordPage->slotSize <= DISK_BOLCK_SIZE;
}