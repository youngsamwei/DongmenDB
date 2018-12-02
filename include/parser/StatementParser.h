//
// Created by sam on 2018/11/30.
//

#ifndef DONGMENDB_STATEMENTPARSER_H
#define DONGMENDB_STATEMENTPARSER_H

/* 在src_experiment\exp_01_stmt_parser\exp_01_03_select.c 中实现*/
class SelectParser : public Parser {
public:
    SelectParser(Tokenizer *tokenizer) : Parser(tokenizer) {
    }

    SRA_t *parse_sql_stmt_select();

    vector<Expression *> parseFieldsExpr();

    SRA_t *parseTablesExpr();
    vector<Expression*> parseOrderExpr(Parser *parser) {
        vector<Expression*> v;
        return v; };
};

class CreateParser : public Parser {
public:
    CreateParser(Tokenizer *tokenizer) : Parser(tokenizer) {
    }

    sql_stmt_create *parse_sql_stmt_create();
    FieldInfo *parse_sql_stmt_columnexpr();

};
/*  在src_experiment\exp_01_stmt_parser\exp_01_02_insert.c 中实现*/;

class InsertParser : public Parser {
public:
    InsertParser(Tokenizer *tokenizer) : Parser(tokenizer) {
    }

    sql_stmt_insert *parse_sql_stmt_insert();
};

/* 在src_experiment\exp_01_stmt_parser\exp_01_04_update.c 中实现*/

class UpdateParser : public Parser {
public:
    UpdateParser(Tokenizer *tokenizer) : Parser(tokenizer) {
    }

    sql_stmt_update *parse_sql_stmt_update();

};

/* 在src_experiment\exp_01_stmt_parser\exp_01_05_delete.c 中实现*/

class DeleteParser : public Parser {
public:
    DeleteParser(Tokenizer *tokenizer) : Parser(tokenizer) {
    }

    sql_stmt_delete *parse_sql_stmt_delete();

};


#endif //DONGMENDB_STATEMENTPARSER_H
