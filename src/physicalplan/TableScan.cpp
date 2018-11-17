//
// Created by sam on 2018/11/15.
//

#include <physicalplan/TableScan.h>
#include <string>

using namespace std;

TableScan::TableScan(dongmendb *db, string tableName, transaction *tx){
    this->m_db = db;
    this->m_tx = tx;
    this->m_tableInfo = table_manager_get_tableinfo(db->metadataManager->tableManager, tableName.c_str(), tx);
    this->m_recordFile = (record_file *) malloc(sizeof(record_file));
    record_file_create(this->m_recordFile, this->m_tableInfo, tx);
};

int TableScan::beforeFirst() {
   return  record_file_before_first(m_recordFile);
};

int TableScan::next() {
    return record_file_next(m_recordFile);
};

int TableScan::close() {
    return record_file_close(m_recordFile);
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
    return record_file_get_int(m_recordFile, fieldName.c_str());
}

variant* TableScan::getValue(string fieldName){
    return NULL;
};

string TableScan::getString(string tableName, string fieldName) {
    if (!tableName.empty() && stricmp(m_tableInfo->tableName, tableName.c_str()) != 0) {
        return "";
    }
    field_info *fi = getField(tableName, fieldName);
    char *value = (char *) calloc(fi->length, 1);
    record_file_get_string(m_recordFile, fieldName.c_str(), value);
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

field_info* TableScan::getField(string tableName, string fieldName) {
    if (!tableName.empty() && stricmp(m_tableInfo->tableName, tableName.c_str()) != 0){
        return NULL;
    }
    hmap_t fields = m_tableInfo->fields;
    void_ptr *fiptr = (void_ptr *)calloc(sizeof(void_ptr), 1);
    hashmap_get(fields, fieldName.c_str(), fiptr);
    if (*fiptr){
        field_info *fi = (field_info *)*fiptr;
        return fi;
    }else{
        return NULL;
    }
};

arraylist* TableScan::getFieldsName(string tableName) {
    if (!tableName.empty() || stricmp(tableName.c_str(), m_tableInfo->tableName) == 0){
        return m_tableInfo->fieldsName;
    }
    /*如果不是指定的tableName，则返回空的arraylist*/
    return arraylist_create();
};

int TableScan::setInt(string tableName, string fieldName, int value) {
    return record_file_set_int(m_recordFile, fieldName.c_str(), value);
};

int TableScan::setString(string tableName, string fieldName, string value) {
    return record_file_set_string(m_recordFile, fieldName.c_str(), value.c_str());
};

int TableScan::deleteRecord() {
    return record_file_delete(m_recordFile);
};

int TableScan::insertRecord() {
    return record_file_insert(m_recordFile);
};

int TableScan::getRID(record_id *recordID) {
    return record_file_current_recordid(m_recordFile, recordID);
};

int TableScan::moveTo(record_id *recordID) {
    return record_file_moveto_recordid(m_recordFile, recordID);
};