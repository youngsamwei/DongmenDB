//
// Created by sam on 2018/11/15.
//

#include <string>

#include <physicalplan/Select.h>

using namespace std;

int Select::beforeFirst() {
    return scan->beforeFirst();
};

int Select::next() {
    while (scan->next()){
        variant *var = (variant *)calloc(1, sizeof(variant));
        evaluateExpression(cond, scan, var);
        if (var->type == DATA_TYPE_BOOLEAN && var->booleanValue){
            return 1;
        }
    }

    return 0;
};

int Select::close() {
    return scan->close();
};

variant* Select::getValueByIndex(int index) {
    return scan->getValueByIndex(index);
};

int Select::getIntByIndex(int index) {
    return scan->getIntByIndex( index);
};

string Select::getStringByIndex(int index) {

    return "";
};

int Select::getInt(string tableName, string fieldName) {
    return scan->getInt( tableName, fieldName);
}

variant* Select::getValue(string fieldName){
    return NULL;
};

string Select::getString(string tableName, string fieldName) {
    FieldInfo *fi = getField(tableName, fieldName);
    char *value = (char *) calloc(1, fi->length * sizeof(char));
    return scan->getString( tableName, fieldName);
};

int Select::hasField(string tableName,string fieldName) {
    return scan->hasField( tableName, fieldName);
};

FieldInfo * Select::getField(string tableName, string fieldName) {
    return scan->getField( tableName, fieldName);
};

vector<char*> Select::getFieldsName(string tableName) {
    return scan->getFieldsName( tableName);
};

int Select::setInt(string tableName, string fieldName, int value) {
    return scan->setInt( tableName, fieldName, value);
};

int Select::setString(string tableName, string fieldName, string value) {
    return scan->setString( tableName, fieldName, value);
};

int Select::deleteRecord() {
    return scan->deleteRecord();
};

 int Select::insertRecord(){

 };

 int Select::getRID(RecordID *recordID){

 };

 int Select::moveTo(RecordID *recordID){

 };