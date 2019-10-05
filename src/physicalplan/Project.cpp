//
// Created by sam on 2018/11/15.
//


#include <string>

#include <physicalplan/Project.h>
#include <dongmensql/column.h>

using namespace std;

int Project::setOriginalExprList(vector<Expression*> exprList){
    this->original_expr_list = exprList;
    processExpressionList();
};

int Project::processExpressionList(){
   /* 处理 形如student.*的字段表示*/
    this->expr_list.clear();
    for (int i = 0; i < this->original_expr_list.size(); i++){
        Expression *expr = this->original_expr_list.at(i);
        if (expr->term !=NULL && expr->term->t == TERM_COLREF){
            ColumnReference *columnReference = expr->term->ref;
            if (stricmp(columnReference->columnName, "*")==0){
                string tablename;
                if (columnReference->tableName){
                    tablename = columnReference->tableName;
                }else{
                    tablename = "";
                }
                vector<char*> names = this->getFieldsName(tablename);
                for (int j = 0; j < names.size(); j ++){
                    char *name = names.at(j);
                    ColumnReference *ref = new ColumnReference(name);
                    Expression *expr0 = new Expression(TOKEN_WORD, NULL);
                    TermExpr *term = expr0->newTermExpr();
                    term->t = TERM_COLREF;
                    term->ref = ref;
                    expr0->term = term;
                    this->expr_list.push_back(expr0);
                }

            }else{
                this->expr_list.push_back(expr);
            }
        }else{
            this->expr_list.push_back(expr);
        }
    }
    return 1;
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

variant *Project::getValueByIndex(int index) {
    Expression *expr =  expr_list.at(index);
    variant *var = (variant *) calloc(1, sizeof(variant));
    evaluateExpression(expr, scan, var);
    return var;
};

int Project::getIntByIndex(int index) {
    Expression *expr = expr_list.at(index);
    variant *var = (variant *) calloc(1, sizeof(variant));
    evaluateExpression(expr, scan, var);
    if (var->type == DATA_TYPE_INT) {
        return var->intValue;
    } else {
        return 0;
    }
};

string Project::getStringByIndex(int index) {
    Expression *expr = expr_list.at(index);
    variant *var = (variant *) calloc(1, sizeof(variant));
    evaluateExpression(expr, scan, var);
    if (var->type == DATA_TYPE_CHAR) {
        return string(var->strValue);
    } else {
        return string("");
    }
};

int Project::getInt(string tableName, string fieldName) {
    return scan->getInt(tableName, fieldName);
}

variant *Project::getValue(string fieldName) {
    return NULL;
};

string Project::getString(string tableName, string fieldName) {
    FieldInfo *fi = getField(tableName, fieldName);
    char *value = (char *) calloc(1, sizeof(char) * fi->length);
    return scan->getString(tableName, fieldName);
};

int Project::hasField(string tableName, string fieldName) {
    return scan->hasField(tableName, fieldName);
};

FieldInfo *Project::getField(string tableName, string fieldName) {
    return scan->getField(tableName, fieldName);
};

vector<char*> Project::getFieldsName(string tableName) {
    return scan->getFieldsName(tableName);
};

int Project::setInt(string tableName, string fieldName, int value) {
    return scan->setInt(tableName, fieldName, value);
};

int Project::setString(string tableName, string fieldName, string value) {
    return scan->setString(tableName, fieldName, value);
};

int Project::deleteRecord() {
    return -1;
};

int Project::insertRecord() {
    return -1;
};

int Project::getRID(RecordID *recordID) {
    return -1;
};

int Project::moveTo(RecordID *recordID) {
    return -1;
};
