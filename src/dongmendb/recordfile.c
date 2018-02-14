//
// Created by Sam on 2018/1/26.
//

#include <utils/utils.h>
#include "dongmendb/recordfile.h"

int record_file_create(record_file *recordFile, table_info *tableInfo,
                       transaction *tx) {

    recordFile->tableInfo = tableInfo;
    recordFile->tx = tx;
    recordFile->recordPage = NULL;
    recordFile->fileName = NULL;
    recordFile->currentBlkNum = -1;

    char *fileName = new_id_name();
    strcpy(fileName, tableInfo->tableName);
    strcat(fileName, RECORD_FILE_EXT);

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
            return 0;
        }
        record_file_moveto(recordFile, recordFile->currentBlkNum + 1);
    }
}

int record_file_atlast(record_file *recordFile) {
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
        /*逻辑问题*/
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
    recordId = (record_id *) malloc(sizeof(record_id));
    recordId->id = recordFile->recordPage->currentSlot;
    recordId->blockNum = recordFile->currentBlkNum;
    return 0;
}

int record_file_moveto(record_file *recordFile, int currentBlkNum) {
    if (recordFile->recordPage != NULL) {
        record_page_close(recordFile->recordPage);
    }
    recordFile->currentBlkNum = currentBlkNum;
    disk_block *diskBlock = (disk_block *) malloc(sizeof(disk_block));
    diskBlock->blkNum = currentBlkNum;
    diskBlock->fileName = recordFile->fileName;
    record_page *recordPage = record_page_create(recordFile->tx, recordFile->tableInfo, diskBlock);

    recordFile->recordPage = recordPage;
    return DONGMENDB_OK;
}

int record_file_atlast_block(record_file *recordFile) {
    int size = transaction_size(recordFile->tx, recordFile->fileName) - 1;
    int result = recordFile->currentBlkNum == size;
    return result;
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

field_info *field_info_create(enum data_type type, int length) {
    field_info *fieldInfo = (field_info *) calloc(sizeof(field_info), 1);
    fieldInfo->type = type;
    fieldInfo->length = length;
    fieldInfo->fieldName = NULL;
    return fieldInfo;
};

table_info *table_info_create(char *tableName, arraylist *fieldsName, hmap_t fields) {
    table_info *tableInfo = (table_info *) malloc(sizeof(table_info));
    tableInfo->tableName = tableName;
    tableInfo->fieldsName = fieldsName;
    tableInfo->fields = fields;
    tableInfo->offsets = hashmap_create();
    tableInfo->recordLen = 0;
    int pos = 0;

    int count = fieldsName->size - 1;
    for (int i = 0; i <= count; i++) {
        char *fieldName = (char *) arraylist_get(fieldsName, i);

        void_ptr *value = (void_ptr *) malloc(sizeof(void_ptr *));
        hashmap_get(tableInfo->fields, fieldName, value);
        field_info *fieldInfo = *value;

        integer *ipos = (integer *) malloc(sizeof(integer));
        ipos->val = pos;
        hashmap_put(tableInfo->offsets, fieldName, ipos);

        if (fieldInfo->type == DATA_TYPE_CHAR) {
            /*增加字符串长度的存储 位置*/
            pos += fieldInfo->length + INT_SIZE;
        } else if (fieldInfo->type == DATA_TYPE_INT) {
            pos += fieldInfo->length;
        }

    }
    tableInfo->recordLen = pos;
    return tableInfo;
};

int table_info_free(table_info *tableInfo) {
    /*free hashmap offsets*/
    // free hashmap fields
    // free arraylist fieldsName
    return DONGMENDB_OK;
}

int table_info_offset(table_info *tableInfo, char *fieldName) {
    void_ptr *ptr = (void_ptr) malloc(sizeof(void_ptr));
    hashmap_get(tableInfo->offsets, fieldName, ptr);
    integer *ipos = *ptr;
    return ipos->val;
};

record_page *record_page_create(transaction *tx, table_info *tableInfo, disk_block *diskBlock) {
    record_page *recordPage = (record_page *) malloc(sizeof(record_page));
    recordPage->diskBlock = diskBlock;
    recordPage->tx = tx;
    recordPage->tableInfo = tableInfo;
    /*保留一个整型的位置保存slot的状态*/
    recordPage->slotSize = tableInfo->recordLen + INT_SIZE;
    recordPage->currentSlot = -1;
    diskBlock->tableInfo = tableInfo;
    transaction_pin(tx, diskBlock);
    return recordPage;
};

int record_page_close(record_page *recordPage) {
    if (recordPage->diskBlock != NULL) {
        transaction_unpin(recordPage->tx, recordPage->diskBlock);
        recordPage->diskBlock = NULL;
    }
}

int record_page_insert(record_page *recordPage) {
    recordPage->currentSlot = -1;
    if (record_page_searchfor(recordPage, RECORD_PAGE_EMPTY)) {
        int position = record_page_current_pos(recordPage);
        transaction_setint(recordPage->tx, recordPage->diskBlock, position, RECORD_PAGE_INUSE);
        return 1;
    }
    return 0;
}

int record_page_moveto_id(record_page *recordPage, int id) {
    recordPage->currentSlot = id;
}

int record_page_next(record_page *recordPage) {
    record_page_searchfor(recordPage, RECORD_PAGE_INUSE);
};

int record_page_getint(record_page *recordPage, char *fieldName) {
    int position = record_page_fieldpos(recordPage, fieldName);
    return transaction_getint(recordPage->tx, recordPage->diskBlock, position);
};

int record_page_getstring(record_page *recordPage, char *fieldName, char *value) {
    int position = record_page_fieldpos(recordPage, fieldName);
    return transaction_getstring(recordPage->tx, recordPage->diskBlock, position, value);
};

int record_page_setint(record_page *recordPage, char *fieldName, int value) {
    int position = record_page_fieldpos(recordPage, fieldName);
    return transaction_setint(recordPage->tx, recordPage->diskBlock, position, value);
};

int record_page_setstring(record_page *recordPage, char *fieldName, char *value) {
    int position = record_page_fieldpos(recordPage, fieldName);
    return transaction_setstring(recordPage->tx, recordPage->diskBlock, position, value);
};

int record_page_delete(record_page *recordPage) {
    int position = record_page_current_pos(recordPage);
    return transaction_setint(recordPage->tx, recordPage->diskBlock, position, RECORD_PAGE_EMPTY);
};

int record_page_searchfor(record_page *recordPage, record_page_status status) {
    recordPage->currentSlot++;
    while (record_page_is_valid_slot(recordPage)) {
        int position = record_page_current_pos(recordPage);
        if (transaction_getint(recordPage->tx, recordPage->diskBlock, position) == status) {
            return 1;
        }
        recordPage->currentSlot++;
    }
    return 0;
};

int record_page_current_pos(record_page *recordPage) {
    return recordPage->currentSlot * recordPage->slotSize;
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
int record_page_fieldpos(record_page *recordPage, char *fieldName) {
    int offset = table_info_offset(recordPage->tableInfo, fieldName);

    return recordPage->currentSlot * recordPage->slotSize + offset + INT_SIZE;
};

int record_page_is_valid_slot(record_page *recordPage) {
    return record_page_current_pos(recordPage) + recordPage->slotSize <= DISK_BOLCK_SIZE;
}