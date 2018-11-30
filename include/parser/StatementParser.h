//
// Created by sam on 2018/11/30.
//

#ifndef DONGMENDB_STATEMENTPARSER_H
#define DONGMENDB_STATEMENTPARSER_H

/* 在src_experiment\exp_01_stmt_parser\exp_01_03_select.c 中实现*/
class SelectParser : public Parser{
public:
    SelectParser(Tokenizer *tokenizer):Parser(tokenizer){
    }

    SRA_t *parse_sql_stmt_select();

    vector<Expression*> parseFieldsExpr();
    SRA_t *parseTablesExpr();
};

class CreateParser : public Parser{
    sql_stmt_create *parseCreate();
};

#endif //DONGMENDB_STATEMENTPARSER_H
