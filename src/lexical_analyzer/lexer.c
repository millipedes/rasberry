#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include"include/lexer.h"
#include"../constants_macros/include/constants.h"

lexer_T * init_lexer(char * source) {
    lexer_T * lexer = calloc(1, sizeof(struct LEXER_STRUCT *));
    lexer->source = source;
    lexer->src_size = strnlen(source, MAX_LINE);
    lexer->c = lexer->source[0];
    return lexer;
}

token_T * lexer_next_token(lexer_T * lexer) {
    while(lexer->c != '\n' && lexer->c != '\r') {
        lexer_skip_whitespace(lexer);

        if(isalpha(lexer->c)) {
            return lexer_parse_word(lexer);
        }

        if(isdigit(lexer->c)) {
            return lexer_parse_digit(lexer);
        }

        switch(lexer->c) {
            case '(':
                lexer_advance(lexer);
                return init_token((char *)"(", TOKEN_L_PAREN);
            case ')':
                lexer_advance(lexer);
                return init_token((char *)")", TOKEN_R_PAREN);
            case '^':
                lexer_advance(lexer);
                return init_token((char *)"^", TOKEN_CARROT_POW);
            case '\n':
            case '\r':
                lexer_advance(lexer);
                return init_token((char *)"\n", TOKEN_EOL);
            case '+':
                lexer_advance(lexer);
                return init_token((char *)"+", TOKEN_PLUS);
            case '-':
                lexer_advance(lexer);
                return init_token((char *)"-", TOKEN_MINUS);
            case '*':
                lexer_advance(lexer);
                return init_token((char *)"*", TOKEN_STAR_MULT);
            case '/':
                lexer_advance(lexer);
                return init_token((char *)"/", TOKEN_FS_DIVIDE);
            case '<':
                lexer_advance(lexer);
                return init_token((char *)"<", TOKEN_LESS_THAN);
            case '>':
                lexer_advance(lexer);
                return init_token((char *)">", TOKEN_GREATER_THAN);
            case'[':
                lexer_advance(lexer);
                return init_token((char *)"[", TOKEN_L_BRACKET);
            case']':
                lexer_advance(lexer);
                return init_token((char *)"]", TOKEN_R_BRACKET);
            case';':
                lexer_advance(lexer);
                return init_token((char *)";", TOKEN_SEMICOLON);
            default:
                printf("[Error Generated from Lexer]: Unexpected Character `%c`\n", lexer->c);
                exit(1);
                break;
        }
    }
    return init_token('\0', TOKEN_EOL);
}

token_T * lexer_parse_digit(lexer_T * lexer) {
    char * integer = (char *)calloc(1, sizeof(char));
    char * digit = (char *)calloc(1, sizeof(char));
    while(isdigit(lexer->c)) {
        integer = (char *)realloc(integer, (strlen(integer) + 2) * sizeof(char));
        *(digit + 0) = lexer->c;
        strncat(integer, digit, MAX_LINE);
        lexer_advance(lexer);
    }
    free(digit);
    return init_token(integer, TOKEN_INT);
}

token_T * lexer_parse_word(lexer_T * lexer) {
    char * keyword = (char *)calloc(1, sizeof(char));
    char * tmp = (char *)calloc(1, sizeof(char));
    // As long as current char is a character in the alphabet advance
    while(isalpha(lexer->c)) {
        keyword = (char *)realloc(keyword, (strnlen(keyword, MAX_LINE) + 1) * sizeof(char));
        *tmp = lexer->c;
        strncat(keyword, tmp, MAX_LINE);
        lexer_advance(lexer);
    }
    free(tmp);
    return init_token(keyword, TOKEN_WORD);
}

void lexer_advance(lexer_T * lexer) {
    if((lexer->c != '\n' && lexer->c != '\r') && lexer->i < lexer->src_size) {
        lexer->i++;
        lexer->c = lexer->source[lexer->i];
    }
}

void lexer_skip_whitespace(lexer_T * lexer) {
    if(lexer->c == ' ') {
        lexer_advance(lexer);
    }
}

void free_lexer(lexer_T * lexer) {
    if(lexer) {
        free(lexer);
    }
}
