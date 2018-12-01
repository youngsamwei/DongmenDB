//
// Created by sam on 2018/11/15.
//

#ifndef DONGMENDB_SCAN_H
#define DONGMENDB_SCAN_H

#include <string>
#include <dongmendb/dongmendb.h>
#include <parser/expression.h>

using namespace std;

/*扫描接口*/
class Scan {
public:
    dongmendb *m_db;
    Transaction *m_tx;

    virtual int beforeFirst()=0;

    virtual int next()=0;

    virtual int close()=0;

    virtual variant *getValueByIndex(int index)=0;

    virtual int getIntByIndex(int index)=0;

    virtual string getStringByIndex(int index)=0;

    virtual int getInt(string tableName, string fieldName) = 0;

    virtual variant* getValue(string fieldName)=0;

    virtual string getString(string tableName, string fieldName)=0;

    virtual int hasField(string tableName,string fieldName)=0;

    virtual field_info* getField(string tableName, string fieldName)=0;

    virtual  vector<char*> getFieldsName(string tableName)=0;

    virtual int setInt(string tableName, string fieldName, int value)=0;

    virtual int setString(string tableName, string fieldName, string value)=0;

    virtual int deleteRecord()=0;

    virtual int insertRecord()=0;

    virtual int getRID(record_id *recordID)=0;

    virtual int moveTo(record_id *recordID)=0;

    Expression * evaluateExpression(Expression *expr, Scan* scan, variant *var);

};



#endif //DONGMENDB_SCAN_H
