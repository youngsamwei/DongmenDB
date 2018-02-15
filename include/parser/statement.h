//
// Created by Sam on 2018/1/17.
//

#ifndef DONGMENDB_STATEMENT_H
#define DONGMENDB_STATEMENT_H

#include <utils/arraylist.h>
#include "dongmendb/recordfile.h"
#include "dongmensql/column.h"
#include "parser.h"

#ifdef __cplusplus
extern "C" {
#endif
typedef struct ParserT_  ParserT;
typedef struct sql_stmt_create_ {
    table_info *tableInfo;

    Constraint_t *constraints;

} sql_stmt_create;

typedef struct sql_stmt_insert_ {
    char *tableName;
    arraylist *fields;
    arraylist *values;
} sql_stmt_insert;

/*
enum AlterType {
    ALTER_ADD,
    ALTER_REMOVE
};

typedef struct AlterStmt_ {
    char *tableName;
    enum AlterType type;
    ColumnsExpr *columnsExpr;
} AlterStmt;


typedef struct DeleteStmt_ {
    char *tableName;
    Expression *whereExpr;
} DeleteStmt;

typedef struct UpdateStmt_ {
    char *tableName;
    SetExpr *setExpr;
    Expression *whereExpr;
} UpdateStmt;
*/

sql_stmt_create *sql_stmt_create_create(char *tableName,
                                        arraylist *fieldsName, hmap_t columns,
                                        Constraint_t *constraints);
/*
AlterStmt *createAlterStmt(char *tableName, enum AlterType type,
                           ColumnsExpr *columnsExpr);

DeleteStmt *createDeleteStmt(char *tableName,
                             Expression *whereExpr);

UpdateStmt *createUpdateStmt(char *tableName,
                             SetExpr *setExpr,
                             Expression *whereExpr);

*/

sql_stmt_create *parse_sql_stmt_create(ParserT *parser);
/*
AlterStmt *parseAlter(ParserT *parser);
InsertStmt *parseInsert(ParserT *parser);
DeleteStmt *parseDelete(ParserT *parser);
UpdateStmt *parseUpdate(ParserT *parser);
*/
/*
TermExpr *parseTermExpr(ParserT *parser);
BinaryExpr *parseBinaryExpr(ParserT *parser);
UnaryExpr *parseUnaryExpr(ParserT *parser);
arraylist *parseGroupExpr(ParserT *parser);
 */
arraylist *parseOrderExpr(ParserT *parser);
/*
ColumnsExpr *parseColumnsExpr(ParserT *parser);
SetExpr *parseSetExpr(ParserT *parser);
*/
#ifdef __cplusplus
}
#endif

#endif //DONGMENDB_STATEMENT_H
