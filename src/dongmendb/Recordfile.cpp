//
// Created by Sam on 2018/1/26.
//

#include <utils/utils.h>
#include <iostream>
#include "dongmendb/Recordfile.h"

RecordFile::RecordFile(TableInfo *tableInfo,
                       Transaction *tx) {

    this->tableInfo = tableInfo;
    this->tx = tx;
    this->recordPage = NULL;
    this->fileName = NULL;
    this->currentBlkNum = -1;

    char *fileName = new_id_name();
    strcpy(fileName, tableInfo->tableName);
    strcat(fileName, RECORD_FILE_EXT);

    this->fileName = fileName;
    if (tx->transaction_size( this->fileName) == 0) {
        record_file_append_block();
    }
    record_file_moveto( 0);
}

int RecordFile::record_file_close() {
    this->recordPage->record_page_close();
}

int RecordFile::record_file_before_first() {
    record_file_moveto( 0);
}

int RecordFile::record_file_next() {
    while (1) {
        if (this->recordPage->record_page_next()) {
            return 1;
        }
        if (record_file_atlast()) {
            return 0;
        }
        record_file_moveto( this->currentBlkNum + 1);
    }
}

int RecordFile::record_file_atlast() {
    return this->currentBlkNum == tx->transaction_size(this->fileName) - 1;
}

int RecordFile::record_file_get_int(const char *fieldName) {
    return this->recordPage->record_page_getint(fieldName);
}

int RecordFile::record_file_get_string(const char *fieldName, char *value) {
    return this->recordPage->record_page_getstring(fieldName, value);
}

int RecordFile::record_file_set_int(const char *fieldName, int value) {
    return this->recordPage->record_page_setint(fieldName, value);
}

int RecordFile::record_file_set_string(const char *fieldName, const char *value) {
    return this->recordPage->record_page_setstring(fieldName, value);
}

int RecordFile::record_file_delete() {
    return this->recordPage->record_page_delete();
}

int RecordFile::record_file_insert() {
    while (!this->recordPage->record_page_insert()) {
        /*逻辑问题*/
        if (record_file_atlast_block()) {
            record_file_append_block();
        }
        record_file_moveto( this->currentBlkNum + 1);
    }
}

int RecordFile::record_file_moveto_recordid(record_id *recordId) {
    record_file_moveto( recordId->blockNum);
    this->recordPage->record_page_moveto_id( recordId->id);
}

int RecordFile::record_file_current_recordid(record_id *recordId) {
    recordId = (record_id *) malloc(sizeof(record_id));
    recordId->id = this->recordPage->currentSlot;
    recordId->blockNum = this->currentBlkNum;
    return 0;
}

int RecordFile::record_file_moveto(int currentBlkNum) {
    if (this->recordPage != NULL) {
        this->recordPage-> record_page_close();
    }
    this->currentBlkNum = currentBlkNum;
    DiskBlock *diskBlock = new DiskBlock(this->fileName, currentBlkNum, this->tableInfo);

    this->recordPage = new RecordPage(this->tx, this->tableInfo, diskBlock);
    return DONGMENDB_OK;
}

int RecordFile::record_file_atlast_block() {
    int size = tx->transaction_size(this->fileName) - 1;
    int result = this->currentBlkNum == size;
    return result;
}

int RecordFile::record_file_append_block() {
    this->tx->transaction_append(this->fileName, this->tableInfo);
}

/**
 * from RecordFormatter.format&makeDefaultRecord
 * @param recordFile
 * @param memoryPage
 * @return
 */
int RecordFile::record_file_record_formatter(MemoryPage *memoryPage) {

}

FieldInfo::FieldInfo(enum data_type type, int length, char* fieldName) {

    this->type = type;
    this->length = length;
    this->hashCode = bkdr_hash(fieldName);
    this->fieldName = strdup(fieldName);

};

TableInfo::TableInfo (const char *tableName, vector<char*> fieldsName,  map<string, FieldInfo *>  *lfields) {

    this->tableName = new_id_name();
    strcpy( this->tableName, tableName);
    this->fieldsName = fieldsName;
    this->fields = lfields;

    /*TODO：需要释放*/
//
    std::map<unsigned int, int> *lfs = new std::map<unsigned int, int>();

    this->recordLen = 0;
    int pos = 0;

    int count = fieldsName.size() - 1;
    for (int i = 0; i <= count; i++) {
        const char* fieldName = fieldsName.at( i);

        FieldInfo *fieldInfo = lfields->find(fieldName)->second;

        (*lfs)[fieldInfo->hashCode]=  pos;
//        lfs->insert(pair<string,integer*>(fieldName, ipos));

        if (fieldInfo->type == DATA_TYPE_CHAR) {
            /*增加字符串长度的存储 位置*/
            pos += fieldInfo->length + INT_SIZE;
        } else if (fieldInfo->type == DATA_TYPE_INT) {
            pos += fieldInfo->length;
        }

    }
    this->offsets = lfs;
    this->recordLen = pos;
};

int TableInfo::table_info_free() {
    /*free hashmap offsets*/
    // free hashmap fields
    // free arraylist fieldsName
    return DONGMENDB_OK;
}

int TableInfo::table_info_offset( const char *fieldName) {
    unsigned int fid = bkdr_hash(fieldName);
    return offsets->find(fid)->second;
};

RecordPage::RecordPage(Transaction *tx, TableInfo *tableInfo, DiskBlock *diskBlock) {
    this->diskBlock = diskBlock;
    this->tx = tx;
    this->tableInfo = tableInfo;
    /*保留一个整型的位置保存slot的状态*/
    this->slotSize = tableInfo->recordLen + INT_SIZE;
    this->currentSlot = -1;
    diskBlock->tableInfo = tableInfo;
    tx->transaction_pin( diskBlock);
};

int RecordPage::record_page_close() {
    if (this->diskBlock != NULL) {
        this->tx->transaction_unpin(this->diskBlock);
        this->diskBlock = NULL;
    }
}

int RecordPage::record_page_insert() {
    this->currentSlot = -1;
    if (record_page_searchfor(RECORD_PAGE_EMPTY)) {
        int position = record_page_current_pos();
        this->tx->transaction_setint(this->diskBlock, position, RECORD_PAGE_INUSE);
        return 1;
    }
    return 0;
}

int RecordPage::record_page_moveto_id(int id) {
    this->currentSlot = id;
}

int RecordPage::record_page_next() {
    record_page_searchfor( RECORD_PAGE_INUSE);
};

int RecordPage::record_page_getint(const char *fieldName) {
    int position = record_page_fieldpos( fieldName);
    return this->tx->transaction_getint(this->diskBlock, position);
};

int RecordPage::record_page_getstring(const char *fieldName, char *value) {
    int position = record_page_fieldpos( fieldName);
    return this->tx->transaction_getstring(this->diskBlock, position, value);
};

int RecordPage::record_page_setint(const char *fieldName, int value) {
    int position = record_page_fieldpos( fieldName);
    return this->tx->transaction_setint( this->diskBlock, position, value);
};

int RecordPage::record_page_setstring(const char *fieldName, const char *value) {
    int position = record_page_fieldpos( fieldName);
    return this->tx->transaction_setstring( this->diskBlock, position, value);
};

int RecordPage::record_page_delete() {
    int position = record_page_current_pos();
    return this->tx->transaction_setint( this->diskBlock, position, RECORD_PAGE_EMPTY);
};

int RecordPage::record_page_searchfor(record_page_status status) {
    this->currentSlot++;
    while (record_page_is_valid_slot()) {
        int position = record_page_current_pos();
        if (this->tx->transaction_getint( this->diskBlock, position) == status) {
            return 1;
        }
        this->currentSlot++;
    }
    return 0;
};

int RecordPage::record_page_current_pos() {
    return this->currentSlot * this->slotSize;
};

/**
 * 计算字段在page中的位置.
 * 每个page由若干slot构成，每个slot保存一条记录。
 * slotsize=recsize + Intsize,有一个intsize保存slot使用状态。
 *
 * @param recordPage
 * @param fieldName
 * @return
 */
int RecordPage::record_page_fieldpos(const char *fieldName) {
    unsigned int fid = bkdr_hash(fieldName);
    int offset = this->tableInfo->offsets->find(fid)->second;

    return this->currentSlot * this->slotSize + offset + INT_SIZE;
};

int RecordPage::record_page_is_valid_slot() {
    return record_page_current_pos() + this->slotSize <= DISK_BOLCK_SIZE;
}