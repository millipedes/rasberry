/**
 * @file This file contains the function definitions of functions used for the
 * abstract syntax tree nodes
 * @brief N/a
 * @author Matthew C. Lindeman
 * @date 12/22/21
 * @bug None known
 * @todo Nothing atm
 */
#ifndef ASTN_H
#define ASNT_H
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"node_types.h"
#include"../../tokenizer/include/token.h"
#include"../../symbol_table/include/symbol_table.h"
#include"../../tokenizer/include/token_types.h"
#include"../../constants_macros/include/constants.h"
#include"../../symbol_table/include/operator.h"
#include"../../types/include/data_frame.h"

typedef struct AST_NODE_T {
	char * name;
    node_type type;
	void * value;
} ast_node_t;

ast_node_t * init_node(token_T ** token, symbol_table_t * st);
void * access_modifer(token_T ** token_list, symbol_table_t * st, ast_node_t * node, int st_index);
void free_node(ast_node_t * node);

#endif
