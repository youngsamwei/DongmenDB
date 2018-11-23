//
// Created by sam on 2018/11/10.
//

#include <test/test_stmt_optimized.h>
#include <dongmensql/optimizer.h>


/*检查SRA_Select操作的条件：
 * 若是单条件，则返回0；
 * 若是and连接的多条件，则返回1*/
int opt_expr_test(Expression *expr) {
    /*term不为空，则表示expr是term，可以终止*/
    if (expr->term != NULL) {
        return 0;
    } else if (expr->opType == TOKEN_AND) {
        return 1;
    }
    return 0;

}

/*检查关系代数表达式中的select是否存在多个and条件串接的情况
 * 若存在，返回1
 * 若不存在，返回0*/
int opt_condition_test(SRA_t *sra) {
    switch (sra->t) {
        case SRA_SELECT: {
            SRA_Select_t select = sra->select;
            Expression *expr = select.cond;
            return opt_expr_test(expr);
        };
        case SRA_TABLE: {
            return 0;
        }
        case SRA_JOIN: {

            int ret = opt_condition_test(sra->join.sra1);
            if (ret) {
                return ret;
            }
            return opt_condition_test(sra->join.sra2);

        }
        case SRA_PROJECT:
            return opt_condition_test(sra->project.sra);
        default:/*其他操作*/
            return 0;
    }
}

/*查找指定的columnName是否在fieldsName中，若在则返回1，否则返回0*/
int opt_search_in_fields(arraylist *fieldsName, const char *columnName){

    for (int i = 0 ; i <=fieldsName->size-1;i++){
        const char *cf = (const char*)fieldsName->data[i];
        if (! strcmp(cf, columnName)){
            /*若相等*/
            return 1;
        }
    }
    return  0;
}

/*判断tableName.columnName是否在sra中*/
int opt_field_test(SRA_t *sra, const char *tableName, const char * columnName, table_manager *tableManager){
    switch (sra->t){
        case SRA_TABLE:{
            if (tableName == NULL){
                table_info *ti = table_manager_get_tableinfo(tableManager, sra->table.ref->table_name, NULL);
                arraylist *fieldsName = ti->fieldsName;
                /*在fieldsName中查找columnName*/
                return opt_search_in_fields(fieldsName, columnName);
            }
            if (!strcmp(tableName, sra->table.ref->table_name)){
                return 1;
            }else{
                return 0;
            }
        }
        case SRA_JOIN:{
            int ret = opt_field_test(sra->join.sra1, tableName, columnName,tableManager);
            if (ret){
                return ret;
            }else {
                return opt_field_test(sra->join.sra2, tableName, columnName, tableManager);
            }
        }
        case SRA_SELECT:
            return opt_field_test(sra->select.sra, tableName, columnName, tableManager);
        default:
            return 0;
    }
}

/* 判断expr中的每个字段是否在sra中, 若存在一个字段不在sra中则返回0， 否则返回1*/
int opt_fields_test(Expression *expr, SRA_t *sra, table_manager *tableManager) {
    if (expr == NULL){
        return 0;
    }
    int ret = 0;
    if (expr->term != NULL) {
        switch(expr->term->t){
            case TERM_COLREF:
                ret = opt_field_test(sra, expr->term->ref->tableName, expr->term->ref->columnName, tableManager);
            default:
                ret = 0;
        }
    }
    if (ret)
        return opt_fields_test(expr->nextexpr, sra, tableManager);
    return 0;
}

int opt_optimed_test(Expression *expr, SRA_t *sra, table_manager *tableManager) {

    switch (sra->t) {
        case SRA_JOIN:
            /*检测expr中的字段属性是否在某个操作对象中，若是则返回1*/
            return opt_fields_test(expr, sra, tableManager);
        default:
            return 0;
    }
}

/*检测每个SRA_Select是否在最优位置*/
int opt_test(SRA_t *sra, table_manager *tableManager) {
    switch (sra->t) {
        case SRA_SELECT: {
            /*寻找子树中JOIM操作，若条件属性均包含在JOIN操作的某个分支中，则返回1*/
            int ret = opt_optimed_test(sra->select.cond, sra->select.sra, tableManager);
            if (ret) {
                return ret;
            } else {
                /*检查子树中的SELECT*/
                return opt_test(sra->select.sra, tableManager);
            }
        };
        case SRA_TABLE: {
            return 0;
        }
        case SRA_JOIN: {

            int ret = opt_test(sra->join.sra1, tableManager);
            if (ret) {
                return ret;
            }
            return opt_test(sra->join.sra2, tableManager);

        }
        case SRA_PROJECT:
            return opt_test(sra->project.sra, tableManager);
        default:/*其他操作*/
            return 0;
    }
}

int TestStmtOptimized::opt_condition_pushdown_test(const char *sqlselect) {
    TokenizerT *tokenizer = TKCreate(sqlselect);
    ParserT *parser = newParser(tokenizer);
    memset(parser->parserMessage, 0, sizeof(parser->parserMessage));

    SRA_t *selectStmt = parse_sql_stmt_select(parser);

    SRA_t *optimizedStmt = dongmengdb_algebra_optimize_condition_pushdown(selectStmt,
                                                                          test_db_ctx->db->metadataManager->tableManager);

    /*检查思路：查找每个SRA_Select， 检查：
     * 1 是否多条件且and连接，若是则返回1 */

    int ret = 0;
    ret = opt_condition_test(optimizedStmt);
    if (ret) {
        return ret;
    }

    /* 2 检查每个SRA_Select 是否在最优位置,若不是则返回1*/
    ret = opt_test(optimizedStmt, test_db_ctx->db->metadataManager->tableManager);

    return ret;
}
