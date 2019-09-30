//
// Created by sam on 2018/11/15.
//

#include <physicalplan/TableScan.h>
#include <string>

using namespace std;

TableScan::TableScan(DongmenDB *db, string tableName, Transaction *tx){
    this->m_db = db;
    this->m_tx = tx;
    this->m_tableInfo = db->tableManager->table_manager_get_tableinfo( tableName.c_str(), tx);
    this->m_recordFile = (RecordFile *) malloc(sizeof(RecordFile));
    this->m_recordFile = new RecordFile(this->m_tableInfo, tx);
};

int TableScan::beforeFirst() {
   return  m_recordFile->record_file_before_first();
};

int TableScan::next() {
    return m_recordFile->record_file_next();
};

int TableScan::close() {
    return m_recordFile->record_file_close();
};

variant * TableScan::getValueByIndex(int index){
    return NULL;
};

int TableScan::getIntByIndex(int index) {
    return -1;
};

string TableScan::getStringByIndex(int index) {
return string("");
};

int TableScan::getInt(string tableName, string fieldName) {
    if (!tableName.empty() && stricmp(m_tableInfo->tableName, tableName.c_str()) != 0) {
        return 0;
    }
    return m_recordFile->record_file_get_int(fieldName.c_str());
}

variant* TableScan::getValue(string fieldName){
    return NULL;
};

string TableScan::getString(string tableName, string fieldName) {
    if (!tableName.empty() && stricmp(m_tableInfo->tableName, tableName.c_str()) != 0) {
        return "";
    }
    FieldInfo *fi = getField(tableName, fieldName);
    char *value = (char *) calloc(1, sizeof(char) * (fi->length));
    m_recordFile->record_file_get_string( fieldName.c_str(), value);
    return string(value);
};

int TableScan::hasField(string tableName,string fieldName) {
    if (!tableName.empty() && stricmp(m_tableInfo->tableName, tableName.c_str()) != 0){
        return 0;
    }
    if (getField( tableName, fieldName)){
        return 1;
    }else{
        return 0;
    }
};

FieldInfo * TableScan::getField(string tableName, string fieldName) {
    if (!tableName.empty() && stricmp(m_tableInfo->tableName, tableName.c_str()) != 0){
        return NULL;
    }

    map<string, FieldInfo *>::iterator it = m_tableInfo->fields->find(fieldName);
    if (it != m_tableInfo->fields->end()){
        return it->second;
    }else{
        return NULL;
    }
};

vector<char*> TableScan::getFieldsName(string tableName) {
    if (tableName.empty() || stricmp(tableName.c_str(), m_tableInfo->tableName) == 0){
        return m_tableInfo->fieldsName;
    }
    vector<char*> v;
    /*如果不是指定的tableName，则返回空的arraylist*/
    return v;
};

int TableScan::setInt(string tableName, string fieldName, int value) {
    return m_recordFile->record_file_set_int( fieldName.c_str(), value);
};

int TableScan::setString(string tableName, string fieldName, string value) {
    return m_recordFile->record_file_set_string(fieldName.c_str(), value.c_str());
};

int TableScan::deleteRecord() {
    return m_recordFile->record_file_delete();
};

int TableScan::insertRecord() {
    return m_recordFile->record_file_insert();
};

int TableScan::getRID(RecordID *recordID) {
    return m_recordFile->record_file_current_recordid( recordID);
};

int TableScan::moveTo(RecordID *recordID) {
    return m_recordFile->record_file_moveto_recordid( recordID);
};