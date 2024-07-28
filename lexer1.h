#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_BUFFER_SIZE 1024

typedef enum {
    TK_ASSIGNOP,
    TK_COMMENT,
    TK_FIELDID,
    TK_ID,
    TK_NUM,
    TK_RNUM,
    TK_FUNID,
    TK_RUID,
    TK_WITH,
    TK_PARAMETERS,
    TK_END,
    TK_WHILE,
    TK_UNION,
    TK_ENDUNION,
    TK_DEFINETYPE,
    TK_AS,
    TK_TYPE,
    TK_MAIN,
    TK_GLOBAL,
    TK_PARAMETER,
    TK_LIST,
    TK_SQL,
    TK_SQR,
    TK_INPUT,
    TK_OUTPUT,
    TK_INT,
    TK_REAL,
    TK_COMMA,
    TK_SEM,
    TK_COLON,
    TK_DOT,
    TK_ENDWHILE,
    TK_OP,
    TK_CL,
    TK_IF,
    TK_THEN,
    TK_ENDIF,
    TK_READ,
    TK_WRITE,
    TK_RETURN,
    TK_PLUS,
    TK_MINUS,
    TK_MUL,
    TK_DIV,
    TK_CALL,
    TK_RECORD,
    TK_ENDRECORD,
    TK_ELSE,
    TK_AND,
    TK_OR,
    TK_NOT,
    TK_LT,
    TK_LE,
    TK_EQ,
    TK_GT,
    TK_GE,
    TK_NE,
    TK_ERROR,
    TK_EOF
} TokenType;

typedef struct {
    TokenType type;
    char lexeme[100];
    int line_no;
} Token;

typedef struct {
    char *source;
    size_t source_size;
    size_t current;
    int line_no;
} Lexer;

Lexer init_lexer(const char* source);
int is_eof(Lexer *lexer);
char advance (Lexer *lexer);
char peek(Lexer *lexer);
char peek_next(Lexer *lexer);
void skip_whitespace(Lexer *lexer);
void skip_comment(Lexer *lexer);
Token make_token(Lexer *lexer, TokenType type, const char *lexeme);
TokenType check_keyword(const char *lexeme);
Token lex(Lexer *lexer);
char *read_file(const char *filename);

#endif