#include "lexer1.h"

Lexer init_lexer(const char *source) {
    Lexer lexer;
    lexer.source = strdup(source);
    lexer.source_size = strlen(source);
    lexer.current = 0;
    lexer.line_no = 1;
    return lexer;
}

int is_eof(Lexer *lexer) {
    return lexer->current >= lexer->source_size;
}

char advance(Lexer *lexer) {
    return lexer->source[lexer->current++];
}

char peek(Lexer *lexer) {
    return lexer->source[lexer->current];
}

char peek_next(Lexer *lexer) {
    if (lexer->current + 1 >= lexer->source_size) return '\0';
    return lexer->source[lexer->current + 1];
}

void skip_whitespace(Lexer *lexer) {
    while (isspace(peek(lexer))) {
        if (peek(lexer) == '\n') lexer->line_no++;
        advance(lexer);
    }
}

void skip_comment(Lexer *lexer) {
    while (peek(lexer) != '\n' && !is_eof(lexer)) {
        advance(lexer);
    }
}

Token make_token(Lexer *lexer, TokenType type, const char *lexeme) {
    Token token;
    token.type = type;
    strncpy(token.lexeme, lexeme, 99);
    token.lexeme[99] = '\0';
    token.line_no = lexer->line_no;
    return token;
}

TokenType check_keyword(const char *lexeme) {
    if (strcmp(lexeme, "with") == 0) return TK_WITH;
    if (strcmp(lexeme, "parameters") == 0) return TK_PARAMETERS;
    if (strcmp(lexeme, "end") == 0) return TK_END;
    if (strcmp(lexeme, "while") == 0) return TK_WHILE;
    if (strcmp(lexeme, "union") == 0) return TK_UNION;
    if (strcmp(lexeme, "endunion") == 0) return TK_ENDUNION;
    if (strcmp(lexeme, "definetype") == 0) return TK_DEFINETYPE;
    if (strcmp(lexeme, "as") == 0) return TK_AS;
    if (strcmp(lexeme, "type") == 0) return TK_TYPE;
    if (strcmp(lexeme, "_main") == 0) return TK_MAIN;
    if (strcmp(lexeme, "global") == 0) return TK_GLOBAL;
    if (strcmp(lexeme, "parameter") == 0) return TK_PARAMETER;
    if (strcmp(lexeme, "list") == 0) return TK_LIST;
    if (strcmp(lexeme, "input") == 0) return TK_INPUT;
    if (strcmp(lexeme, "output") == 0) return TK_OUTPUT;
    if (strcmp(lexeme, "int") == 0) return TK_INT;
    if (strcmp(lexeme, "real") == 0) return TK_REAL;
    if (strcmp(lexeme, "endwhile") == 0) return TK_ENDWHILE;
    if (strcmp(lexeme, "if") == 0) return TK_IF;
    if (strcmp(lexeme, "then") == 0) return TK_THEN;
    if (strcmp(lexeme, "endif") == 0) return TK_ENDIF;
    if (strcmp(lexeme, "read") == 0) return TK_READ;
    if (strcmp(lexeme, "write") == 0) return TK_WRITE;
    if (strcmp(lexeme, "return") == 0) return TK_RETURN;
    if (strcmp(lexeme, "call") == 0) return TK_CALL;
    if (strcmp(lexeme, "record") == 0) return TK_RECORD;
    if (strcmp(lexeme, "endrecord") == 0) return TK_ENDRECORD;
    if (strcmp(lexeme, "else") == 0) return TK_ELSE;
    return TK_FIELDID;
}

Token lex(Lexer *lexer) {
    skip_whitespace(lexer);

    if (is_eof(lexer)) return make_token(lexer, TK_EOF, "EOF");

    char c = advance(lexer);

    if (c == '%') {
        skip_comment(lexer);
        skip_whitespace(lexer);
        if (is_eof(lexer)) return make_token(lexer, TK_EOF, "EOF");
        c = advance(lexer);
    }

    // Multi-character tokens and identifiers
    if (isalpha(c)) {
        size_t start = lexer->current - 1;
        while (isalnum(peek(lexer))) advance(lexer);
        size_t length = lexer->current - start;
        char *lexeme = strndup(&lexer->source[start], length);
        TokenType type = check_keyword(lexeme);
        Token token = make_token(lexer, type, lexeme);
        free(lexeme);
        return token;
    }

    if (c == '_') {
        size_t start = lexer->current - 1;
        while (isalnum(peek(lexer)) || isdigit(peek(lexer))) advance(lexer);
        size_t length = lexer->current - start;
        char *lexeme = strndup(&lexer->source[start], length);
        if (strcmp(lexeme, "_main") == 0) {
            Token token = make_token(lexer, TK_MAIN, lexeme);
            free(lexeme);
            return token;
        }
        Token token = make_token(lexer, TK_FUNID, lexeme);
        free(lexeme);
        return token;
    }

    if (isdigit(c)) {
        size_t start = lexer->current - 1;
        while (isdigit(peek(lexer))) advance(lexer);
        if (peek(lexer) == '.' && isdigit(peek_next(lexer))) {
            advance(lexer);
            while (isdigit(peek(lexer))) advance(lexer);
            if (peek(lexer) == 'E' && (peek_next(lexer) == '+' || peek_next(lexer) == '-' || isdigit(peek_next(lexer)))) {
                advance(lexer);
                if (peek(lexer) == '+' || peek(lexer) == '-') advance(lexer);
                while (isdigit(peek(lexer))) advance(lexer);
                size_t length = lexer->current - start;
                char *lexeme = strndup(&lexer->source[start], length);
                Token token = make_token(lexer, TK_RNUM, lexeme);
                free(lexeme);
                return token;
            }
            size_t length = lexer->current - start;
            char *lexeme = strndup(&lexer->source[start], length);
            Token token = make_token(lexer, TK_RNUM, lexeme);
            free(lexeme);
            return token;
        }
        size_t length = lexer->current - start;
        char *lexeme = strndup(&lexer->source[start], length);
        Token token = make_token(lexer, TK_NUM, lexeme);
        free(lexeme);
        return token;
    }

    // Single character tokens
    switch (c) {
        case '=': return make_token(lexer, TK_ASSIGNOP, "=");
        case '(': return make_token(lexer, TK_OP, "(");
        case ')': return make_token(lexer, TK_CL, ")");
        case '[': return make_token(lexer, TK_SQL, "[");
        case ']': return make_token(lexer, TK_SQR, "]");
        case '+': return make_token(lexer, TK_PLUS, "+");
        case '-': return make_token(lexer, TK_MINUS, "-");
        case '*': return make_token(lexer, TK_MUL, "*");
        case '/': return make_token(lexer, TK_DIV, "/");
        case ',': return make_token(lexer, TK_COMMA, ",");
        case ';': return make_token(lexer, TK_SEM, ";");
        case ':': return make_token(lexer, TK_COLON, ":");
        case '.': return make_token(lexer, TK_DOT, ".");
        case '~': return make_token(lexer, TK_NOT, "~");
        case '<': 
            if (peek(lexer) == '=') {
                advance(lexer);
                return make_token(lexer, TK_LE, "<=");
            }
            return make_token(lexer, TK_LT, "<");
        case '>':
            if (peek(lexer) == '=') {
                advance(lexer);
                return make_token(lexer, TK_GE, ">=");
            }
            return make_token(lexer, TK_GT, ">");
        case '!':
            if (peek(lexer) == '=') {
                advance(lexer);
                return make_token(lexer, TK_NE, "!=");
            }
            break;
        case '#': {
            size_t start = lexer->current - 1;
            while (isalnum(peek(lexer))) advance(lexer);
            size_t length = lexer->current - start;
            char *lexeme = strndup(&lexer->source[start], length);
            Token token = make_token(lexer, TK_RUID, lexeme);
            free(lexeme);
            return token;
        }
        case '&': {
            if (peek(lexer) == '&' && peek_next(lexer) == '&') {
                advance(lexer);
                advance(lexer);
                return make_token(lexer, TK_AND, "&&&");
            }
            break;
        }
        case '@': {
            if (peek(lexer) == '@' && peek_next(lexer) == '@') {
                advance(lexer);
                advance(lexer);
                return make_token(lexer, TK_OR, "@@@");
            }
            break;
        }
    }

    // If no valid token is found, return an error token
    return make_token(lexer, TK_ERROR, "ERROR");
}

char *read_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Could not open file %s\n", filename);
        exit(EXIT_FAILURE);
    }

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *buffer = (char *)malloc(length + 1);
    if (!buffer) {
        fprintf(stderr, "Could not allocate memory for reading file\n");
        exit(EXIT_FAILURE);
    }

    fread(buffer, 1, length, file);
    buffer[length] = '\0';

    fclose(file);
    return buffer;
}


// void test(){
//     Token token;
//     do {
//         token = lex(&lexer);
//         printf("Token: %s, Lexeme: %s, Line: %d\n", 
//             (token.type == TK_ASSIGNOP) ? "TK_ASSIGNOP" :
//             (token.type == TK_NUM) ? "TK_NUM" :
//             (token.type == TK_FIELDID) ? "TK_FIELDID" :
//             (token.type == TK_FUNID) ? "TK_FUNID" :
//             (token.type == TK_OP) ? "TK_OP" :
//             (token.type == TK_CL) ? "TK_CL" :
//             (token.type == TK_PLUS) ? "TK_PLUS" :
//             (token.type == TK_SEM) ? "TK_SEM" :
//             (token.type == TK_MAIN) ? "TK_MAIN" :
//             (token.type == TK_ERROR) ? "TK_ERROR" :
//             (token.type == TK_EOF) ? "TK_EOF" : "UNKNOWN",
//             token.lexeme, token.line_no);
//     } while (token.type != TK_EOF);

//     free(lexer.source);
//     free(source_code);
//     return 0;
// }
