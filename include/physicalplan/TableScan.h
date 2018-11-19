//
// Created by sam on 2018/11/15.
//

#ifndef DONGMENDB_TABLESCAN_H
#define DONGMENDB_TABLESCAN_H


#include <physicalplan/Scan.h>

/*表扫描*/
class TableScan : public Scan {

public:
    record_file *m_recordFile;
    table_info *m_tableInfo;

    TableScan(dongmendb *db, string tableName, transaction *tx);

    int beforeFirst();

    int next();

    int close();

    variant *getValueByIndex(int index);

    int getIntByIndex(int index);

    string getStringByIndex(int index);

    int getInt(string tableName, string fieldName);

    variant* getValue(string fieldName);

    string getString(string tableName, string fieldName);

    int hasField(string tableName, string fieldName);

    field_info* getField(string tableName, string fieldName);

    vector<char*>* getFieldsName(string tableName);

    int setInt(string tableName, string fieldName, int value);

    int setString(string tableName, string fieldName, string value);

    int deleteRecord();

    int insertRecord();

    int getRID(record_id *recordID);

    int moveTo(record_id *recordID);
};

#endif //DONGMENDB_TABLESCAN_H
