//
// Created by sam on 2018/11/15.
//

#ifndef DONGMENDB_SELECT_H
#define DONGMENDB_SELECT_H

#include <physicalplan/Scan.h>

class Select : public Scan {
    public:
    Expression *cond;
    physical_scan *scan;

    Select(physical_scan *scan);

    int beforeFirst();

    int next();

    int close();

    variant* getValueByIndex(int index);

    int getIntByIndex(int index);

    string getStringByIndex(int index);

    int getInt(string tableName, string fieldName);

    variant getValue(string fieldName);

    string getString(string tableName, string fieldName);

    int hasField(string tableName, string fieldName);

    field_info *getField(string tableName, string fieldName);

    arraylist *getFieldsName(string tableName);

    int setInt(string tableName, string fieldName, int value);

    int setString(string tableName, string fieldName, string value);

    int deleteRecord();

    int insertRecord();

    int getRID(record_id *recordID);

    int moveTo(record_id *recordID);
};

#endif //DONGMENDB_SELECT_H
