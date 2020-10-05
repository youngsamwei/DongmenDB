//
// Created by sam on 2018/10/20.
//

#ifndef DONGMENDB_TEST_STMT_PARSER_H
#define DONGMENDB_TEST_STMT_PARSER_H

#include <relationalalgebra/sra.h>

/**
 * @param t1
 * @param t2
 * @return 两个 SRA 结构是否一致（不判断表达式的值是否相似）
 */
bool equal_sra(const SRA_t *t1, const SRA_t *t2);

// region create table

bool operator==(const FieldInfo &info1, const FieldInfo &info2);

bool equal_table_name(const sql_stmt_create &stmt1, const sql_stmt_create &stmt2);

bool equal_fields_name(const sql_stmt_create &stmt1, const sql_stmt_create &stmt2);

bool equal_fields(const sql_stmt_create &stmt1, const sql_stmt_create &stmt2);

bool equal_table_info(const sql_stmt_create &stmt1, const sql_stmt_create &stmt2);

bool equal(const sql_stmt_create &stmt1, const sql_stmt_create &stmt2);

// endregion

// region insert

bool operator==(const variant &v1, const variant &v2);

bool equal_table_name(const sql_stmt_insert &stmt1, const sql_stmt_insert &stmt2);

bool equal_fields(const sql_stmt_insert &stmt1, const sql_stmt_insert &stmt2);

bool equal_values(const sql_stmt_insert &stmt1, const sql_stmt_insert &stmt2);

bool equal(const sql_stmt_insert &stmt1, const sql_stmt_insert &stmt2);

// endregion

// region update

bool equal_table_name(const sql_stmt_update &stmt1, const sql_stmt_update &stmt2);

bool equal_where(const sql_stmt_update &stmt1, const sql_stmt_update &stmt2);

bool equal_fields_expr(const sql_stmt_update &stmt1, const sql_stmt_update &stmt2);

bool equal_fields(const sql_stmt_update &stmt1, const sql_stmt_update &stmt2);

bool equal(const sql_stmt_update &stmt1, const sql_stmt_update &stmt2);

// endregion

// region delete

bool equal_table_name(const sql_stmt_delete &stmt1, const sql_stmt_delete &stmt2);

bool equal_where(const sql_stmt_delete &stmt1, const sql_stmt_delete &stmt2);

bool equal(const sql_stmt_delete &stmt1, const sql_stmt_delete &stmt2);

// endregion

#endif //DONGMENDB_TEST_STMT_PARSER_H
