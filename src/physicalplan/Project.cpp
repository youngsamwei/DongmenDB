//
// Created by sam on 2018/11/15.
//


#include <string>

#include <physicalplan/Project.h>
#include <physicalplan/physicalplan.h>

using namespace std;

Project::Project(physical_scan *scan){
    this->scan = scan;
};

int Project::beforeFirst() {
    physical_scan *scan1  = scan->physicalScanSelect->scan;
    return scan1->beforeFirst(scan1);
};

int Project::next() {
    physical_scan *scan1 = scan->physicalScanProject->scan;
    return scan1->next(scan1);
};

int Project::close() {
    physical_scan *scan1  = scan->physicalScanSelect->scan;
    return scan1->close(scan1);
};

variant* Project::getValueByIndex(int index) {
    arraylist *exprs = scan->physicalScanProject->expr_list;
    Expression *expr = (Expression *)arraylist_get(exprs, index);
    variant *var = (variant *)calloc(sizeof(variant),1);
    physical_scan_evaluate_expression(expr,scan, var);
    return var;
};

int Project::getIntByIndex(int index) {
    arraylist *exprs = scan->physicalScanProject->expr_list;
    Expression *expr = (Expression *)arraylist_get(exprs, index);
    variant *var = (variant *)calloc(sizeof(variant),1);
    physical_scan_evaluate_expression(expr,scan, var);
    if (var->type == DATA_TYPE_INT){
        return var->intValue;
    }else{
        return 0;
    }
};

string Project::getStringByIndex(int index) {
    arraylist *exprs = scan->physicalScanProject->expr_list;
    Expression *expr = (Expression *)arraylist_get(exprs, index);
    variant *var = (variant *)calloc(sizeof(variant),1);
    physical_scan_evaluate_expression(expr,scan, var);
    if (var->type == DATA_TYPE_CHAR){
        return string(var->strValue);
    }else{
        return string("");
    }
};

int Project::getInt(string tableName, string fieldName) {
    physical_scan *scan1  = scan->physicalScanSelect->scan;
    return scan1->getInt(scan1, tableName.c_str(), fieldName.c_str());
}

string Project::getString(string tableName, string fieldName) {
    physical_scan *scan1  = scan->physicalScanSelect->scan;
    field_info *fi = getField(tableName, fieldName);
    char *value = (char *) calloc(fi->length, 1);
    scan1->getString(scan1, tableName.c_str(), fieldName.c_str(), value);
    return string(value);
};

int Project::hasField(string tableName,string fieldName) {
    physical_scan *scan1  = scan->physicalScanSelect->scan;
    return scan1->hasField(scan1, tableName.c_str(), fieldName.c_str());
};

field_info* Project::getField(string tableName, string fieldName) {
    physical_scan *scan1  = scan->physicalScanSelect->scan;
    return scan1->getField(scan1, tableName.c_str(), fieldName.c_str());
};

arraylist* Project::getFieldsName(string tableName) {
    physical_scan *scan1  = scan->physicalScanSelect->scan;
    return scan1->getFieldsName(scan1, tableName.c_str());
};

int Project::setInt(string tableName, string fieldName, int value) {
    physical_scan *scan1  = scan->physicalScanSelect->scan;
    return scan1->setInt(scan1, tableName.c_str(), fieldName.c_str(), value);
};

int Project::setString(string tableName, string fieldName, string value) {
    physical_scan *scan1  = scan->physicalScanSelect->scan;
    return scan1->setString(scan1, tableName.c_str(), fieldName.c_str(), value.c_str());
};
