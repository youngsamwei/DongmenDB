//
// Created by sam on 2018/12/4.
//

#ifndef DONGMENDB_SQLSTATEMENT_H
#define DONGMENDB_SQLSTATEMENT_H

#define SQL_NOTVALID (-1)
#define SQL_NULL (0)
#define SQL_INTEGER_1BYTE (1)
#define SQL_INTEGER_2BYTE (2)
#define SQL_INTEGER_4BYTE (4)
#define SQL_TEXT (13)

#define STMT_CREATE (0)
#define STMT_SELECT (1)
#define STMT_INSERT (2)
#define STMT_DELETE (3)

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
