//
// Created by sam on 2018/12/4.
//

#ifndef DONGMENDB_SQLSTATEMENT_H
#define DONGMENDB_SQLSTATEMENT_H

enum sql_statement_type {
    SQL_CREATE_DATABASE,
    SQL_CREATE_TABLE,
    SQL_CREATE_VIEW,
    SQL_CREATE_INDEX,
    SQL_CREATE_USER,
    SQL_CREATE_ROLE,
    SQL_DROP,
    SQL_SELECT,
    SQL_INSERT,
    SQL_UPDATE,
    SQL_DELETE,
    SQL_GRANT,
    SQL_REVOKE,
    SQL_DENY
};

class SQLStatement {
public:
    /*sql文本*/
    char *text;
    sql_statement_type type;

    SQLStatement(char *text, sql_statement_type type){
        this->text = strdup(text);
        this->type = type;
    }
};


#endif //DONGMENDB_SQLSTATEMENT_H
