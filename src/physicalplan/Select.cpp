//
// Created by sam on 2018/11/15.
//

#include <string>

#include <physicalplan/Select.h>
#include <physicalplan/physicalplan.h>

using namespace std;

Select::Select(physical_scan *scan){
    this->scan = scan;
};

int Select::beforeFirst() {
    physical_scan *scan1  = scan->physicalScanSelect->scan;
    return scan1->beforeFirst(scan1);
};

int Select::next() {
    physical_scan *scan1  = scan->physicalScanSelect->scan;
    Expression *expr = scan->physicalScanSelect->cond;
    while (scan1->next(scan1)){
        variant *var = (variant *)calloc(sizeof(variant),1);
        physical_scan_evaluate_expression(expr, scan1, var);
        if (var->type == DATA_TYPE_BOOLEAN && var->booleanValue){
            return 1;
        }
    }

    return 0;
};

int Select::close() {
    physical_scan *scan1  = scan->physicalScanSelect->scan;
    return scan1->close(scan1);
};

variant* Select::getValueByIndex(int index) {
    physical_scan *scan1 = scan->physicalScanProject->scan;
    return scan1->getValByIndex(scan1, index);
};

int Select::getIntByIndex(int index) {
    physical_scan *scan1 = scan->physicalScanProject->scan;
    return scan1->getIntByIndex(scan1, index);
};

string Select::getStringByIndex(int index) {

    return "";
};

int Select::getInt(string tableName, string fieldName) {
    physical_scan *scan1  = scan->physicalScanSelect->scan;
    return scan1->getInt(scan1, tableName.c_str(), fieldName.c_str());
}

string Select::getString(string tableName, string fieldName) {
    physical_scan *scan1  = scan->physicalScanSelect->scan;
    field_info *fi = getField(tableName, fieldName);
    char *value = (char *) calloc(fi->length, 1);
    scan1->getString(scan1, tableName.c_str(), fieldName.c_str(), value);
    return string(value);
};

int Select::hasField(string tableName,string fieldName) {
    physical_scan *scan1  = scan->physicalScanSelect->scan;
    return scan1->hasField(scan1, tableName.c_str(), fieldName.c_str());
};

field_info* Select::getField(string tableName, string fieldName) {
    physical_scan *scan1  = scan->physicalScanSelect->scan;
    return scan1->getField(scan1, tableName.c_str(), fieldName.c_str());
};

arraylist* Select::getFieldsName(string tableName) {
    physical_scan *scan1  = scan->physicalScanSelect->scan;
    return scan1->getFieldsName(scan1, tableName.c_str());
};

int Select::setInt(string tableName, string fieldName, int value) {
    physical_scan *scan1  = scan->physicalScanSelect->scan;
    return scan1->setInt(scan1, tableName.c_str(), fieldName.c_str(), value);
};

int Select::setString(string tableName, string fieldName, string value) {
    physical_scan *scan1  = scan->physicalScanSelect->scan;
    return scan1->setString(scan1, tableName.c_str(), fieldName.c_str(), value.c_str());
};

int Select::deleteRecord() {
    physical_scan *scan1  = scan->physicalScanSelect->scan;
    return scan1->deleterec(scan1);
};

int Select::insertRecord() {

    return -1;
};

int Select::getRID(record_id *recordID) {

    return -1;
};

int Select::moveTo(record_id *recordID) {

    return -1;
};