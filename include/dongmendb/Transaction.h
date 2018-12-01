//
// Created by Sam on 2018/1/26.
//

#ifndef DONGMENDB_TRANSACTION_H
#define DONGMENDB_TRANSACTION_H

#include "DongmenDB.h"

#include <vector>
#include <map>

using namespace std;


#define  END_OF_FILE -1

class BufferManager;
class DongmenDB;

typedef void *void_ptr;

typedef struct dongmendb_ dongmendb;
typedef struct recovery_manager_ recovery_manager;
typedef struct concurrency_manager_ concurrency_manager;
typedef struct buffer_list_ buffer_list;
typedef struct buffer_manager_ buffer_manager;

static int next_tx_num = 1;

class Transaction {
public:
    DongmenDB *db;
    int txNum;
    recovery_manager *recoveryManager;
    concurrency_manager *concurrencyManager;
    buffer_list *bufferList;

    Transaction(DongmenDB *db);
    int transaction_commit();
    int transaction_rollback();
    int transaction_recover();

    int transaction_pin(DiskBlock *block);
    int transaction_unpin(DiskBlock *block);

    int transaction_getint(DiskBlock *block, int offset);
    int transaction_setint(DiskBlock *block, int offset, int value);
    int transaction_getstring(DiskBlock *block, int offset, char *value);
    int transaction_setstring(DiskBlock *block, int offset, const char *value);

    int transaction_size(char *fileName);
    int transaction_append(char *fileName, table_info *tableInfo);

    int transaction_next_txnum();
} ;

typedef struct buffer_list_ {
    map<string, MemoryBuffer*> *buffers;
    vector<DiskBlock *> pins;
    BufferManager *bufferManager;
} buffer_list;


int buffer_list_pin(buffer_list *bufferList, DiskBlock *block);
int buffer_list_unpin(buffer_list *bufferList, DiskBlock *block);
int buffer_list_unpin_all(buffer_list *bufferList);
MemoryBuffer *buffer_list_get_buffer(buffer_list *bufferList, DiskBlock *block);
int buffer_list_pin_new(buffer_list *bufferList, char *fileName, void_ptr *block, table_info *tableInfo);



#endif //DONGMENDB_TRANSACTION_H
