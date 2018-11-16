//
// Created by sam on 2018/11/15.
//


#include <string>

#include <physicalplan/Project.h>

using namespace std;

Project::Project(Scan *scan){
    this->scan = scan;
};

int Project::beforeFirst() {
    return scan->beforeFirst();
};

int Project::next() {
    return scan->next();
};

int Project::close() {
    return scan->close();
};

variant* Project::getValueByIndex(int index) {
    Expression *expr = (Expression *)arraylist_get(expr_list, index);
    variant *var = (variant *)calloc(sizeof(variant),1);
    evaluateExpression(expr,scan, var);
    return var;
};

int Project::getIntByIndex(int index) {
    Expression *expr = (Expression *)arraylist_get(expr_list, index);
    variant *var = (variant *)calloc(sizeof(variant),1);
    evaluateExpression(expr,scan, var);
    if (var->type == DATA_TYPE_INT){
        return var->intValue;
    }else{
        return 0;
    }
};

string Project::getStringByIndex(int index) {
    Expression *expr = (Expression *)arraylist_get(expr_list, index);
    variant *var = (variant *)calloc(sizeof(variant),1);
    evaluateExpression(expr,scan, var);
    if (var->type == DATA_TYPE_CHAR){
        return string(var->strValue);
    }else{
        return string("");
    }
};

int Project::getInt(string tableName, string fieldName) {
    return scan->getInt( tableName.c_str(), fieldName.c_str());
}

string Project::getString(string tableName, string fieldName) {
    field_info *fi = getField(tableName, fieldName);
    char *value = (char *) calloc(fi->length, 1);
    return scan->getString( tableName.c_str(), fieldName.c_str());
};

int Project::hasField(string tableName,string fieldName) {
    return scan->hasField( tableName.c_str(), fieldName.c_str());
};

field_info* Project::getField(string tableName, string fieldName) {
    return scan->getField( tableName.c_str(), fieldName.c_str());
};

arraylist* Project::getFieldsName(string tableName) {
    return scan->getFieldsName( tableName.c_str());
};

int Project::setInt(string tableName, string fieldName, int value) {
    return scan->setInt( tableName.c_str(), fieldName.c_str(), value);
};

int Project::setString(string tableName, string fieldName, string value) {
    return scan->setString( tableName.c_str(), fieldName.c_str(), value.c_str());
};
