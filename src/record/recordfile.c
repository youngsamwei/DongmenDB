//
// Created by Sam on 2018/1/26.
//

#include "recordfile.h"

int record_file_new(record_file *recordFile, table_info *tableInfo,
                    transaction *tx) {
    recordFile = (record_file *) malloc(sizeof(record_file));
    recordFile->tableInfo = tableInfo;
    recordFile->tx = tx;
    recordFile->fileName = strcat(tableInfo->tableName, RECORD_FILE_EXT);
    if (transaction_size(tx, recordFile->fileName) == 0) {
        record_file_append_block(recordFile);
    }
}

int record_file_close(record_file *recordFile) {
    record_page_close(recordFile->recordPage);
}

int record_file_append_block(record_file *recordFile) {

}

int record_page_close(record_page *recordPage) {

}