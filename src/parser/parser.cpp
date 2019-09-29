//
// Created by Sam on 2018/1/17.
//

#include <string.h>
#include <parser/Tokenizer.h>
#include <parser/parser.h>
#include <dongmensql/expression.h>

/**
 * @brief 创建语法解析器
 * @param tokenizer 词法分析器
 * @return 解析器
 */
Parser::Parser(Tokenizer *tokenizer) {
    this->tokenizer = tokenizer;
    this->currToken = NULL;
    this->parserStateType = PARSER_CORRECT;
    memset(this->parserMessage, 0, sizeof(this->parserMessage));
};

/**
 * @brief 获取下一个token，如果当前的token不为NULL，则不获取下一个token。
 * 如果解析出错则返回NULL,停止解析
 * @param parser 解析器
 * @return Token
 */
Token *Parser::parseNextToken() {
    if (this->parserStateType == PARSER_WRONG) {
        return NULL;
    }
    if (this->currToken == NULL) {
        this->currToken = this->tokenizer->getNextToken();
    }
    return this->currToken;
};

/**
 * @brief 处理完一个token后，需要调用此方法把token销毁。
 * @param parser  解析器
 * @return  NULL
 */
Token *Parser::parseEatToken() {
    if (this->parserStateType == PARSER_WRONG) {
        return NULL;
    }
    this->currToken = NULL;
    return NULL;
};

/**
 * @brief eat掉当前的token，并获取下一个token
 * @param parser
 * @return
 */
Token *Parser::parseEatAndNextToken() {
    if (this->parserStateType == PARSER_WRONG) {
        return NULL;
    }
    this->currToken = this->tokenizer->getNextToken();
    return this->currToken;
};

/**
 * @brief 产生解析错误
 * @param parser 解析器
 * @param message 错误消息
 * @return NULL
 */
void *Parser::parseError( char *message) {
    strcpy(this->parserMessage, message);
    this->parserStateType = PARSER_WRONG;
    return NULL;
};

int Parser::matchToken( TokenType type, char *text){
    Token *token = parseNextToken();
    if (token && token->type == type && stricmp(token->text, text) == 0) {
        parseEatAndNextToken();
        return 1;
    } else {
        strcpy(this->parserMessage, "invalid sql : missing ");
        strcat(this->parserMessage, text);
        return 0;
    }
};



/*使用递归下降法解析表达式
 *
 * 支持：
 * 算术运算 + - * /
 * 比较运算: > < != <= >=
 * 逻辑运算: NOT AND OR
 * 函数:  round, ltrim （仅作为例子，没有实现）
 * 字段变量: 类似student.sname
 * 支持简单的语法检查。
 * 支持的数据类型:数值，字符串
 *
 * 尚未支持的运算符： like， between..and 等
 *
 * 此功能属于语法检查，尚不支持语义检查。比如函数操作符操作数类型匹配，函数传参类型匹配检查等。
 *
 * 入口函数：parseExpressionRD
 * */


Expression *Parser::parseExpressionRD() {
    Expression *expr = parseReadBooleanOr();
    if (this->parserStateType != PARSER_WRONG && this->currToken != NULL
        && this->currToken->type != TOKEN_RESERVED_WORD && this->currToken->type != TOKEN_COMMA &&
        this->currToken->type != TOKEN_SEMICOLON) {
        /*表达式没有处理完，比如只有右括号的情况。*/
        char message[PARSER_MESSAGE_LENTTH];
        sprintf(message, "syntax error: %s.", this->currToken->text);
        parseError(message);
        return NULL;
    }

    return expr;
};

/*由parseReadDouble修改而来*/
Expression *Parser::parseReadLiteral() {
    Expression *expr0 = NULL;
    Token *token = parseNextToken();
    if (token == NULL) {
        parseError("syntax error: missing number.");
        return NULL;
    }
    if ((token->type == TOKEN_FLOAT || token->type == TOKEN_DECIMAL
         || token->type == TOKEN_EXP_FLOAT)) {

        /*处理数值,当前支持整型*/
        expr0 = new Expression(token->type, NULL);
        TermExpr *term = expr0->newTermExpr();
        term->t = TERM_LITERAL;
        Literal *literal
                = new IntLiteral(DATA_TYPE_INT, token->text);
        term->val = literal;
        expr0->term = term;
        parseEatToken();
        return expr0;
    } else if (token->type == TOKEN_STRING || token->type == TOKEN_CHAR) {
        /*处理string和char,都转换为string*/
        expr0 = new Expression(TOKEN_STRING, NULL);
        TermExpr *term = expr0->newTermExpr();
        term->t = TERM_LITERAL;
        Literal *literal
                = new Literal(DATA_TYPE_CHAR);

        /*去掉引号*/
        if (token->text[0] == '"') {
            int len = strlen(token->text) - 1;
            char *v = token->text + 1;
            char *value = (char *) calloc(1, sizeof(char) * len);
            strcpy(value, v);
            value[len - 1] = '\0';
            literal->original_value = value;
        }else{
            literal->original_value = strdup(token->text);
        }

        term->val = literal;
        expr0->term = term;
        parseEatToken();
        return expr0;
    } else {
        char message[PARSER_MESSAGE_LENTTH];
        sprintf(message, "syntax error: unenabled data type :%s.", token->text);
        parseError(message);
        return NULL;
    }
};

Expression *Parser::parseReadArgument() {
    Expression *expr0 = NULL;
    Token *token = parseNextToken();
    if (token == NULL) {
        parseError("syntax error: missing argument.");
        return NULL;
    }
    if (token->type == TOKEN_COMMA) {
        parseEatToken();
    }
    expr0 = parseReadExpr();
    return expr0;
};

Expression *Parser::parseReadBuiltin() {
    Expression *expr0 = NULL, *expr1 = NULL;
    Token *token = parseNextToken();
    if (token == NULL) {
        parseError("syntax error: missing id or number.");
        return NULL;
    }
    if (token->type == TOKEN_WORD) {
        char *text = strdup(token->text);
        token = parseEatAndNextToken();

        if (token != NULL && token->type == TOKEN_OPEN_PAREN) {
            /*函数*/
            if (stricmp(text, "ltrim") == 0) {
                /*函数参数个数的检测*/
                token = parseEatAndNextToken();
                /*如果紧跟着右括号，则丢失参数*/
                if (token->type == TOKEN_CLOSE_PAREN) {
                    char message[PARSER_MESSAGE_LENTTH];
                    sprintf(message, "syntax error: function %s missing argument.", text);

                    parseError(message);
                    return NULL;
                }
                Expression *param0 = parseReadArgument();
                token = parseNextToken();
                /*如果解析参数完毕，接下来是逗号，则太多参数*/
                if (token->type == TOKEN_COMMA) {
                    char message[PARSER_MESSAGE_LENTTH];
                    sprintf(message, "syntax error: function %s too many  argument.", text);
                    parseError(message);
                    return NULL;
                }
                expr0 = new Expression(TOKEN_FUN, param0);
            } else if (stricmp(text, "round") == 0) {
                /*函数参数个数的检测*/
                token = parseEatAndNextToken();
                if (token->type == TOKEN_CLOSE_PAREN) {
                    char message[PARSER_MESSAGE_LENTTH];
                    sprintf(message, "syntax error: function %s missing  argument.", text);
                    parseError(message);
                    return NULL;
                }
                Expression *param0 = parseReadArgument();
                token = parseNextToken();
                if (token->type != TOKEN_COMMA) {
                    char message[PARSER_MESSAGE_LENTTH];
                    sprintf(message, "syntax error: function %s missing  argument.", text);
                    parseError(message);
                    return NULL;
                }
                Expression *param1 = parseReadArgument();
                token = parseNextToken();
                if (token->type == TOKEN_COMMA) {
                    char message[PARSER_MESSAGE_LENTTH];
                    sprintf(message, "syntax error: function %s too many  argument.", text);
                    parseError(message);
                    return NULL;
                }
                expr0 = concatExpression(param0, param1);
                expr0 = new Expression(TOKEN_FUN, expr0);
            } else {
                char message[PARSER_MESSAGE_LENTTH];
                sprintf(message, "syntax error: unsupported function  :%s.", text);
                parseError(message);
                return NULL;
            }
            token = parseNextToken();

            if (token == NULL || token->type != TOKEN_CLOSE_PAREN) {
                parseError("syntax error: syntax error: missing ')'.");
                return NULL;
            } else {
                /*处理掉右括号*/
                parseEatToken();
            }
            /*如果是function，则在expr的term_id中保存函数名称*/
            TermExpr *term = expr0->newTermExpr();
            term->t = TERM_FUNC;
            term->id = text;
            expr0->term = term;
            return expr0;
        } else {
            /*此处是标识符处理: column_ref, 两种形式 student.*, student.sno,  sno */
            ColumnReference *columnReference = new ColumnReference(text);
            expr0 = new Expression(TOKEN_WORD, NULL);
            TermExpr *term = expr0->newTermExpr();
            term->t = TERM_COLREF;
            term->ref = columnReference;
            expr0->term = term;
        }
    } else if (token->type == TOKEN_MULTIPLY) {
        /* 在select中的* */
        char *text = strdup(token->text);
        ColumnReference *columnReference = new ColumnReference(text);
        expr0 = new Expression(TOKEN_WORD, NULL);
        TermExpr *term = expr0->newTermExpr();
        term->t = TERM_COLREF;
        term->ref = columnReference;
        expr0->term = term;
        parseEatAndNextToken();
    } else {
        expr0 = parseReadLiteral();
    }
    return expr0;
};

Expression *Parser::parseReadParen() {
    Expression *expr0 = NULL;
    Token *token = parseNextToken();
    if (token == NULL) {
        parseError("syntax error.");
        return NULL;
    }
    if (token->type == TOKEN_OPEN_PAREN) {
        parseEatAndNextToken();
        expr0 = parseReadBooleanOr();
        token = parseNextToken();
        if (token == NULL || (token != NULL && token->type != TOKEN_CLOSE_PAREN)) {
            parseError("syntax error: missing ')'.");
            return NULL;
        } else {
            parseEatToken();
        }
    } else {
        expr0 = parseReadBuiltin();
    }
    return expr0;
};

Expression *Parser::parseReadUnary() {
    Expression *expr0 = NULL, *expr1 = NULL;
    Token *token = parseNextToken();
    if (token == NULL) {
        parseError("syntax error.");
        return NULL;
    }
    if (token != NULL && (token->type == TOKEN_NOT || token->type == TOKEN_MINUS
                          || token->type == TOKEN_PLUS)) {
        parseEatAndNextToken();
        expr1 = parseReadParen();
        expr0 = new Expression(token->type, expr1);
    } else {
        expr0 = parseReadParen();
    }
    return expr0;
};

Expression *Parser::parseReadPower() {
    Expression *expr0 = NULL, *expr1 = NULL;
    Token *token = parseNextToken();
    expr0 = parseReadUnary();
    token = parseNextToken();

    if (token != NULL && token->type == TOKEN_POWER) {
        /*需要特殊处理*/
        token = parseEatAndNextToken();
        expr1 = parseReadPower();
    }
    /*整合为一个表达式*/
    return expr0;
};

Expression *Parser::parseReadTerm() {
    Expression *expr0 = NULL, *expr1 = NULL, *expr2 = NULL;
    expr0 = parseReadPower();
    Token *token = parseNextToken();

    while (token != NULL && (token->type == TOKEN_MULTIPLY || token->type == TOKEN_DIVIDE)) {
        /*需要处理多个连续的乘除*/
        parseEatToken();
        /*获得乘法的右表达式*/
        expr1 = parseReadPower();
        /*左右表达式合并为一个后缀表达式*/
        expr0 = concatExpression(expr0, expr1);
        /*在后缀表达式前增加乘法操作*/
        expr2 = new Expression(token->type, expr0);
        expr0 = expr2;
        token = parseNextToken();
    }

    return expr0;
};

/*合并两个后缀表达式*/
Expression *Parser::concatExpression(Expression *expr0, Expression *expr1) {
    if (expr0 == NULL)
        return expr1;
    Expression *expr2 = expr0;
    while (expr2->nextexpr != NULL) {
        expr2 = expr2->nextexpr;
    }
    expr2->nextexpr = expr1;
    return expr0;
}

Expression *Parser::parseReadExpr() {
    Expression *expr0 = NULL, *expr1 = NULL, *expr2 = NULL;
    Token *token = parseNextToken();
    if (token == NULL) {
        parseError("syntax error.");
        return NULL;
    }
    if (token->type == TOKEN_PLUS || token->type == TOKEN_MINUS) {
        /*处理正负号*/
        parseEatToken();
        expr0 = parseReadTerm();
        expr0 = new Expression(token->type, expr0);
    } else {
        expr0 = parseReadTerm();
    }
    /*需要递归处理，连续的+-操作*/
    token = parseNextToken();

    while (token != NULL && (token->type == TOKEN_PLUS || token->type == TOKEN_MINUS)) {
        parseEatToken();
        expr1 = parseReadTerm();
        expr0 = concatExpression(expr0, expr1);
        expr2 = new Expression(token->type, expr0);
        expr0 = expr2;
        token = parseNextToken();
    }
    return expr0;
};

Expression *Parser::parseReadBooleanComparison() {
    Expression *expr0 = NULL, *expr1 = NULL, *expr2 = NULL;
    expr0 = parseReadExpr();
    Token *token = parseNextToken();

    if (token != NULL && (token->type == TOKEN_GE || token->type == TOKEN_LE
                          || token->type == TOKEN_GT || token->type == TOKEN_LT)) {
        parseEatAndNextToken();
        expr1 = parseReadExpr();
        expr0 = concatExpression(expr0, expr1);
        expr0 = new Expression(token->type, expr0);
    }
    return expr0;
};

Expression *Parser::parseReadBooleanEquality() {
    Expression *expr0 = NULL, *expr1 = NULL;
    Token *token = parseNextToken();
    expr0 = parseReadBooleanComparison();
    token = parseNextToken();

    if (token != NULL && (token->type == TOKEN_NOT_EQUAL || token->type == TOKEN_EQ)) {
        parseEatAndNextToken();
        expr1 = parseReadBooleanComparison();
        expr0 = concatExpression(expr0, expr1);
        expr0 = new Expression(token->type, expr0);
    } else if (token != NULL && token->type == TOKEN_NOT) {
        expr0 = new Expression(token->type, expr0);
    }
    return expr0;
};

Expression *Parser::parseReadBooleanAnd() {
    Expression *expr0 = NULL, *expr1 = NULL, *expr2 = NULL;
    Token *token = parseNextToken();
    expr0 = parseReadBooleanEquality();
    token = parseNextToken();

    while (token != NULL && token->type == TOKEN_AND) {
        parseEatToken();
        expr1 = parseReadBooleanEquality();
        expr0 = concatExpression(expr0, expr1);
        expr2 = new Expression(token->type, expr0);
        expr0 = expr2;
        token = parseNextToken();
    }
    return expr0;

};

Expression *Parser::parseReadBooleanOr() {
    Expression *expr0, *expr1 = NULL, *expr2 = NULL;
    Token *token = parseNextToken();
    expr0 = parseReadBooleanAnd();
    token = parseNextToken();

    while (token != NULL && token->type == TOKEN_OR) {
        parseEatToken();
        expr1 = parseReadBooleanAnd();
        expr0 = concatExpression(expr0, expr1);
        expr2 = new Expression(token->type, expr0);
        expr0 = expr2;
        token = parseNextToken();
    }

    return expr0;
};
