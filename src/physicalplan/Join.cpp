//
// Created by sam on 2018/11/15.
//


#include <string>

#include <physicalplan/Join.h>

using namespace std;

int Join::beforeFirst() {
    scan1->beforeFirst();
    scan1->next();
    scan2->beforeFirst();
    return 1;
};

int Join::next() {
    if (scan2->next()) {
        return 1;
    } else {
        scan2->beforeFirst();
        return scan2->next() && scan1->next();
    }
};

int Join::close() {
    int c1 = scan1->close();
    if (c1)
        return c1;
    return scan2->close();
};

variant *Join::getValueByIndex(int index){

    return NULL;
};

int Join::getIntByIndex(int index){
    return -1;
}

string Join::getStringByIndex(int index){
    return string("");
}

int Join::getInt(string tableName, string fieldName) {

    if (scan1->hasField( tableName, fieldName)) {
        return scan1->getInt( tableName, fieldName);
    } else {
        return scan2->getInt( tableName, fieldName);
    };
}

variant* Join::getValue(string fieldName){
    return NULL;
}

string Join::getString(string tableName, string fieldName) {
    if (scan1->hasField( tableName, fieldName)) {
         return scan1->getString( tableName, fieldName);
    } else {
        return scan2->getString( tableName, fieldName);
    };
};

int Join::hasField(string tableName, string fieldName) {
    return (scan1->hasField( tableName, fieldName))
           || (scan2->hasField( tableName, fieldName));
};

field_info *Join::getField(string tableName, string fieldName) {

    field_info *fi = scan1->getField( tableName, fieldName);
    if (fi) {
        return fi;
    } else {
        return scan2->getField( tableName, fieldName);
    }
};

vector<char*> Join::getFieldsName(string tableName) {

    vector<char*> scan1flds = scan1->getFieldsName( tableName);
    vector<char*> scan2flds = scan2->getFieldsName( tableName);

    scan1flds.insert(scan1flds.end(), scan2flds.begin(), scan2flds.end());
    return scan1flds;
};

int Join::setInt(string tableName, string fieldName, int value){
    return -1;
}

int Join::setString(string tableName, string fieldName, string value){
    return -1;
}

 int Join::deleteRecord(){
     return -1;
 };

 int Join::insertRecord(){
     return -1;
 };

 int Join::getRID(record_id *recordID){
     return -1;
 }

 int Join::moveTo(record_id *recordID){
     return -1;
 };