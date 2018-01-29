//
// Created by Sam on 2018/1/26.
//

#include <malloc.h>
#include "transaction.h"

int transaction_commit(transaction *transaction) {

}

int transaction_rollback(transaction *transaction) {

}

int transaction_recover(transaction *transaction) {

}

int transaction_pin(transaction *transaction, disk_block *block) {
    return buffer_list_pin(transaction->bufferList, block);
};

int transaction_unpin(transaction *transaction, disk_block *block) {
    buffer_list_unpin(transaction->bufferList, block);
};

int transaction_getint(transaction *transaction, disk_block *block, int offset) {
    //concurrency_manager_slock(transaction->concurrencyManager, block);
    memory_buffer *buffer ;
    buffer_list_get_buffer(transaction->bufferList, block, buffer);
    return buffer_getint(buffer, offset);
};

int transaction_setint(transaction *transaction, disk_block *block, int offset, int value) {
    //concurrency_manager_xlock(transaction->concurrencyManager, block);
    memory_buffer *buffer ;
    buffer_list_get_buffer(transaction->bufferList, block, buffer);
    int lsn = 0;//recovery_manager_setint(transaction->recoveryManager, buffer, offset, value);
    return buffer_setint(buffer, offset, value, transaction->txNum, lsn);
};

int transaction_getstring(transaction *transaction, disk_block *block, int offset, char *value) {
    //concurrency_manager_slock(transaction->concurrencyManager, block);
    memory_buffer *buffer ;
    buffer_list_get_buffer(transaction->bufferList, block, buffer);
    return buffer_getstring(buffer, offset, value);
};

int transaction_setstring(transaction *transaction, disk_block *block, int offset, char *value) {
    //concurrency_manager_xlock(transaction->concurrencyManager, block);
    memory_buffer *buffer ;
    buffer_list_get_buffer(transaction->bufferList, block, buffer);
    int lsn = 0;//recovery_manager_setstring(transaction->recoveryManager, buffer, offset, value);
    return buffer_setstring(buffer, offset, value, transaction->txNum, lsn);
};

int transaction_size(transaction *tx, char *fileName) {
    return 1;
    //file_manager_size(fileName);
}

int transaction_append(transaction *tx, char *fileName, disk_block *block) {
    //concurrency_manager_xlock

    buffer_list_pin_new(tx->bufferList, fileName, block);
    transaction_unpin(tx, block);
    return 1;
}

int transaction_next_txnum(transaction *tx) {
    return 1;
};

int buffer_list_pin(buffer_list *bufferList, disk_block *block){};
int buffer_list_unpin(buffer_list *bufferList, disk_block *block){};
int buffer_list_get_buffer(buffer_list *bufferList, disk_block *block, memory_buffer *buffer){};
int buffer_list_pin_new(buffer_list *bufferList, char *fileName, disk_block *block){};
