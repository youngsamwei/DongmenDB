//
// Created by sam on 2018/10/20.
//

#include <dongmensql/sqlstatement.h>

bool equal_sra(const SRA_t *t1, const SRA_t *t2) {
    if (t1 == nullptr && t2 == nullptr) return true;
    if (t1 == nullptr || t2 == nullptr) return false;
    if (t1->t != t2->t) return false;

    switch (t1->t) {
        case SRA_TABLE:
            if (strcmp(t1->table.ref->table_name, t2->table.ref->table_name) != 0) return false;
            if (!(t1->table.ref->alias || t2->table.ref->alias)) return true;
            return strcmp(t1->table.ref->alias, t2->table.ref->alias) == 0;
        case SRA_PROJECT:
            if (!equal_sra(t1->project.sra, t2->project.sra)) return false;
            if (t1->project.order_by.size() != t1->project.order_by.size()) return false;
            if (t1->project.group_by.size() != t1->project.group_by.size()) return false;
            if (t1->project.expr_list.size() != t1->project.expr_list.size()) return false;
            return t1->project.distinct == t2->project.distinct;
        case SRA_SELECT:
            if (!equal_sra(t1->select.sra, t2->select.sra)) return false;
            return (t1->select.cond && t2->select.cond) ||
                   (!(t1->select.cond) && !(t2->select.cond));
        case SRA_JOIN:
        case SRA_FULL_OUTER_JOIN:
        case SRA_LEFT_OUTER_JOIN:
        case SRA_RIGHT_OUTER_JOIN:
            if (!equal_sra(t1->join.sra1, t2->join.sra1)) return false;
            if (!equal_sra(t1->join.sra2, t2->join.sra2)) return false;
            return (t1->join.opt_cond && t2->join.opt_cond) ||
                   (!(t1->join.opt_cond) && !(t2->join.opt_cond));
        case SRA_UNION:
        case SRA_EXCEPT:
        case SRA_INTERSECT:
        case SRA_NATURAL_JOIN:
            return equal_sra(t1->binary.sra1, t1->binary.sra2);
        default:
            return false;
    }
}

bool operator==(const FieldInfo &info1, const FieldInfo &info2) {
    if (info1.type != info2.type) return false;
    if (strcmp(info1.fieldName, info2.fieldName) != 0) return false;
    return info1.length == info2.length;
}

bool equal_table_name(const sql_stmt_create &stmt1, const sql_stmt_create &stmt2) {
    return strcmp(stmt1.tableInfo->tableName, stmt2.tableInfo->tableName) == 0;
}

bool equal_fields_name(const sql_stmt_create &stmt1, const sql_stmt_create &stmt2) {
    for (auto &&it1 = stmt1.tableInfo->fieldsName.cbegin(), it2 = stmt2.tableInfo->fieldsName.cbegin();
         it1 != stmt1.tableInfo->fieldsName.cend() && it2 != stmt2.tableInfo->fieldsName.cend();
         ++it1, ++it2)
        if (strcmp(*it1, *it2) != 0) return false;
    return true;
}

bool equal_fields(const sql_stmt_create &stmt1, const sql_stmt_create &stmt2) {
    for (auto &&it1 = stmt1.tableInfo->fields->cbegin(), it2 = stmt2.tableInfo->fields->cbegin();
         it1 != stmt1.tableInfo->fields->cend() && it2 != stmt2.tableInfo->fields->cend();
         ++it1, ++it2)
        if (it1->first == it2->first && *(it1->second) == *(it2->second)) return false;
    return true;
}

bool equal_table_info(const sql_stmt_create &stmt1, const sql_stmt_create &stmt2) {
    return equal_table_name(stmt1, stmt2) && equal_fields_name(stmt1, stmt2) && equal_fields(stmt1, stmt2);
}

bool equal(const sql_stmt_create &stmt1, const sql_stmt_create &stmt2) {
    return equal_table_info(stmt1, stmt2);
}


bool operator==(const variant &v1, const variant &v2) {
    if (v1.type != v2.type) return false;
    switch (v1.type) {
        case DATA_TYPE_INT:
            return v1.intValue == v2.intValue;
        case DATA_TYPE_CHAR:
            return strcmp(v1.strValue, v2.strValue) == 0;
        case DATA_TYPE_BOOLEAN:
            return v1.booleanValue == v2.booleanValue;
        case DATA_TYPE_DOUBLE:
        case DATA_TYPE_TEXT:
        default:
            return true;
    }
}

bool equal_table_name(const sql_stmt_insert &stmt1, const sql_stmt_insert &stmt2) {
    return strcmp(stmt1.tableName, stmt2.tableName) == 0;
}

bool equal_fields(const sql_stmt_insert &stmt1, const sql_stmt_insert &stmt2) {
    for (auto &&it1 = stmt1.fields.cbegin(), it2 = stmt2.fields.cbegin();
         it1 != stmt1.fields.cend() && it2 != stmt2.fields.cend();
         ++it1, ++it2)
        if (strcmp(*it1, *it2) != 0) return false;
    return true;
}

bool equal_values(const sql_stmt_insert &stmt1, const sql_stmt_insert &stmt2) {
    for (auto &&it1 = stmt1.values.cbegin(), it2 = stmt2.values.cbegin();
         it1 != stmt1.values.cend() && it2 != stmt2.values.cend();
         ++it1, ++it2)
        if (*it1 == *it2) return false;
    return true;
}

bool equal(const sql_stmt_insert &stmt1, const sql_stmt_insert &stmt2) {
    return equal_table_name(stmt1, stmt2) && equal_fields(stmt1, stmt2) && equal_values(stmt1, stmt2);
}


bool equal_table_name(const sql_stmt_update &stmt1, const sql_stmt_update &stmt2) {
    if (!stmt1.tableName && !stmt2.tableName) return true;
    return strcmp(stmt1.tableName, stmt2.tableName) == 0;
}

bool equal_where(const sql_stmt_update &stmt1, const sql_stmt_update &stmt2) {
    if (!stmt1.where && !stmt2.where) return true;
    return equal_sra(stmt1.where, stmt2.where);
}

bool equal_fields_expr(const sql_stmt_update &stmt1, const sql_stmt_update &stmt2) {
    return stmt1.fieldsExpr.size() == stmt2.fieldsExpr.size();
}

bool equal_fields(const sql_stmt_update &stmt1, const sql_stmt_update &stmt2) {
    for (auto &&it1 = stmt1.fields.cbegin(), it2 = stmt2.fields.cbegin();
         it1 != stmt1.fields.cend() && it2 != stmt2.fields.cend();
         ++it1, ++it2)
        if (strcmp(*it1, *it2) != 0) return false;
    return true;
}

bool equal(const sql_stmt_update &stmt1, const sql_stmt_update &stmt2) {
    return equal_table_name(stmt1, stmt2) && equal_where(stmt1, stmt2) &&
           equal_fields_expr(stmt1, stmt2) && equal_fields(stmt1, stmt2);
}


bool equal_table_name(const sql_stmt_delete &stmt1, const sql_stmt_delete &stmt2) {
    if (!stmt1.tableName && !stmt2.tableName) return true;
    return strcmp(stmt1.tableName, stmt2.tableName) == 0;
}

bool equal_where(const sql_stmt_delete &stmt1, const sql_stmt_delete &stmt2) {
    if (!stmt1.where && !stmt2.where) return true;
    return equal_sra(stmt1.where, stmt2.where);
}

bool equal(const sql_stmt_delete &stmt1, const sql_stmt_delete &stmt2) {
    return equal_table_name(stmt1, stmt2) && equal_where(stmt1, stmt2);
}