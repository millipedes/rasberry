/**
 * @file This file contains the fucntion definitions for the functions of token.c
 * @brief N/a
 * @author Matthew C. Lindeman
 * @date 12/22/21
 * @bug None known
 * @todo Nothing atm
 */
#ifndef TOK_H
#define TOK_H
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"../../constants_macros/include/constants.h"
#include"token_types.h"

typedef struct TOKEN_STRUCT {
    char * id;
    token_type type;
} token_T;

token_T * init_token(char * id, int type);
void print_token_list(token_T ** token_list);
char * token_to_str(token_T * token);
int is_operator(token_T * token);
int operator_degree(token_T * token);
void free_token(token_T * token);
void free_token_list(token_T ** token_list);

#endif
