//
// Created by sam on 2018/11/15.
//


#include <string>

#include <physicalplan/Join.h>

using namespace std;

Join::Join(physical_scan *scan1, physical_scan *scan2) {
    this->scan1 = scan1;
    this->scan2 = scan2;
};

int Join::beforeFirst() {
    scan1->beforeFirst(scan1);
    scan1->next(scan1);
    scan2->beforeFirst(scan2);
    return 1;
};

int Join::next() {
    if (scan2->next(scan2)) {
        return 1;
    } else {
        scan2->beforeFirst(scan2);
        return scan2->next(scan2) && scan1->next(scan1);
    }
};

int Join::close() {
    int c1 = scan1->close(scan1);
    if (c1)
        return c1;
    return scan2->close(scan2);
};

int Join::getInt(string tableName, string fieldName) {

    if (scan1->hasField(scan1, tableName.c_str(), fieldName.c_str())) {
        return scan1->getInt(scan1, tableName.c_str(), fieldName.c_str());
    } else {
        return scan2->getInt(scan2, tableName.c_str(), fieldName.c_str());
    };
}

string Join::getString(string tableName, string fieldName) {
    if (scan1->hasField(scan1, tableName.c_str(), fieldName.c_str())) {
        field_info *fi = scan1->getField(scan1, tableName.c_str(), fieldName.c_str());
        char *value = (char *) calloc(fi->length, 1);
        scan1->getString(scan1, tableName.c_str(), fieldName.c_str(), value);
        return string(value);
    } else {
        field_info *fi = scan2->getField(scan2, tableName.c_str(), fieldName.c_str());
        char *value = (char *) calloc(fi->length, 1);
        scan1->getString(scan2, tableName.c_str(), fieldName.c_str(), value);
        return string(value);;
    };
};

int Join::hasField(string tableName, string fieldName) {
    return (scan1->hasField(scan1, tableName.c_str(), fieldName.c_str()))
           || (scan2->hasField(scan2, tableName.c_str(), fieldName.c_str()));
};

field_info *Join::getField(string tableName, string fieldName) {

    field_info *fi = scan1->getField(scan1, tableName.c_str(), fieldName.c_str());
    if (fi) {
        return fi;
    } else {
        return scan2->getField(scan2, tableName.c_str(), fieldName.c_str());
    }
};

arraylist *Join::getFieldsName(string tableName) {

    arraylist *scan1flds = scan1->getFieldsName(scan1, tableName.c_str());
    arraylist *scan2flds = scan2->getFieldsName(scan2, tableName.c_str());
    arraylist *all = arraylist_create();
    arraylist_add_all(all, scan1flds);
    arraylist_add_all(all, scan2flds);
    return all;
};
