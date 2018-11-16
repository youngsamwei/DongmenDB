//
// Created by sam on 2018/11/15.
//

#include <string>

#include <physicalplan/Select.h>

using namespace std;

int Select::beforeFirst() {
    return scan.beforeFirst();
};

int Select::next() {
    while (scan.next()){
        variant *var = (variant *)calloc(sizeof(variant),1);
        evaluateExpression(cond, scan, var);
        if (var->type == DATA_TYPE_BOOLEAN && var->booleanValue){
            return 1;
        }
    }

    return 0;
};

int Select::close() {
    return scan.close();
};

variant* Select::getValueByIndex(int index) {
    return scan.getValueByIndex(index);
};

int Select::getIntByIndex(int index) {
    return scan.getIntByIndex( index);
};

string Select::getStringByIndex(int index) {

    return "";
};

int Select::getInt(string tableName, string fieldName) {
    return scan.getInt( tableName.c_str(), fieldName.c_str());
}

variant* Select::getValue(string fieldName){
    return NULL;
};

string Select::getString(string tableName, string fieldName) {
    field_info *fi = getField(tableName, fieldName);
    char *value = (char *) calloc(fi->length, 1);
    return scan.getString( tableName.c_str(), fieldName.c_str());
};

int Select::hasField(string tableName,string fieldName) {
    return scan.hasField( tableName.c_str(), fieldName.c_str());
};

field_info* Select::getField(string tableName, string fieldName) {
    return scan.getField( tableName.c_str(), fieldName.c_str());
};

arraylist* Select::getFieldsName(string tableName) {
    return scan.getFieldsName( tableName.c_str());
};

int Select::setInt(string tableName, string fieldName, int value) {
    return scan.setInt( tableName.c_str(), fieldName.c_str(), value);
};

int Select::setString(string tableName, string fieldName, string value) {
    return scan.setString( tableName.c_str(), fieldName.c_str(), value.c_str());
};

int Select::deleteRecord() {
    return scan.deleteRecord();
};

 int Select::insertRecord(){

 };

 int Select::getRID(record_id *recordID){

 };

 int Select::moveTo(record_id *recordID){

 };