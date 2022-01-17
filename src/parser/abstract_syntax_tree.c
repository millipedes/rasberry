/**
 * @file abstract_syntax_tree.c
 * @brief This file contains the functions relating to making an abstract parse 
 * tree from a token stack
 * @author Matthew C. Lindeman
 * @date 12/22/21
 * @bug None known
 * @todo Nothing atm
 */
#include"include/abstract_syntax_tree.h"

/**
* This function initializes an abstaract syntax tree
* @param N/a
* @return The tree that was initialized
*/
ast_t * init_ast(void) {
	ast_t * ast = calloc(1, sizeof(struct ABSTRACT_SYNTAX_TREE));
	ast->node = NULL;
	ast->children = NULL;
	ast->no_children = 0;
	return ast;
}

/**
 * This function generates an ast from a token list
 * @param The token list, the symbol table, and an index (always pass 0)
 * @return The ast
 */
ast_t * generate_tree(token_T ** token_list, symbol_table_t * st, ast_t * ast) {
	int flag = 0;
    int operands = 0;
    token_T *** potential_operands;
    /* See initialize_potential_operands for better description of why this
     * exits
     */

	if(token_list[0]->type == TOKEN_INT || token_list[0]->type == TOKEN_WORD || token_list[0]->type == TOKEN_STRING || token_list[0]->type == TOKEN_FLOAT) {
		ast->node = init_node(token_list, st);
		ast->children = NULL;
		ast->no_children = 0;
		return ast;
    } else {
		switch(token_list[0]->type) {
			case TOKEN_L_PAREN:
                operands = 1;
                potential_operands = initialize_potential_operands(operands);
                potential_operands[0] = get_sub_list(token_list, 1, 
                        get_list_size(token_list));
                ast->no_children++;
                ast->children = calloc(ast->no_children, 
                        sizeof(struct ABSTRACT_SYNTAX_TREE *));
                ast->node = init_node(token_list, st);
                ast->children[0] = init_ast();
                ast->children[0] = generate_tree(potential_operands[0], st,
                        ast->children[0]);
                free_potential_operands(potential_operands, operands);
                return ast;
			case TOKEN_L_BRACKET:
                ast->no_children++;
                ast->children = calloc(ast->no_children, sizeof(struct ABSTRACT_SYNTAX_TREE *));
                ast->children[0] = init_ast();
                ast->node = init_node(token_list, st);
                return ast;
            case TOKEN_ASSIGN:
			case TOKEN_PLUS:
			case TOKEN_MINUS:
			case TOKEN_FS_DIVIDE:
			case TOKEN_STAR_MULT:
			case TOKEN_LESS_THAN:
			case TOKEN_GREATER_THAN:
			case TOKEN_CARROT_POW:
            case TOKEN_EQUAL_TEST:
            case TOKEN_LTE:
            case TOKEN_GTE:
            case TOKEN_NE:
				// BINARY OPERATORS
				ast->no_children++;
				ast->no_children++;
				ast->children = calloc(ast->no_children, 
                        sizeof(struct ABSTRACT_SYNTAX_TREE *));
				ast->node = init_node(token_list, st);
				ast->children[0] = init_ast();
				ast->children[1] = init_ast();
				/** Massive code blurb checking for + INT INT || + EXPR INT
				 * || + INT EXPT || + EXPR EXPR
				 */
                // reprase = W | S | I | F & 1==2
				if((token_list[1]->type == TOKEN_INT
                            || token_list[1]->type == TOKEN_WORD
                            || token_list[1]->type == TOKEN_FLOAT
                            || token_list[1]->type == TOKEN_STRING)
                        && (token_list[1]->type == token_list[2]->type)) {
                           // || (token_list[1]->type == TOKEN_WORD
                           //     && token_list[0]->type == TOKEN_ASSIGN))) {
                    operands = 2;
                    potential_operands = initialize_potential_operands(operands);
					potential_operands[0] = get_sub_list(token_list, 1, 1);
					potential_operands[1] = get_sub_list(token_list, 2, 
                            get_list_size(token_list));
					ast->children[0] = generate_tree(potential_operands[0], st,
                            ast->children[0]);
					ast->children[1] = generate_tree(potential_operands[1], st,
                            ast->children[1]);
                    free_potential_operands(potential_operands, operands);
					return ast;
				} else if((token_list[1]->type == TOKEN_INT
                            || token_list[1]->type == TOKEN_WORD
                            || token_list[1]->type == TOKEN_STRING
                            || token_list[1]->type == TOKEN_FLOAT) && 
                        token_list[2]->type == TOKEN_L_PAREN) {
                    operands = 2;
                    potential_operands = initialize_potential_operands(operands);
					potential_operands[0] = get_sub_list(token_list, 1, 1);
					potential_operands[1] = get_sub_list(token_list, 2,
                            get_list_size(token_list));
					ast->children[0] = generate_tree(potential_operands[0], st,
                            ast->children[0]);
					ast->children[1] = generate_tree(potential_operands[1], st,
                            ast->children[1]);
                    free_potential_operands(potential_operands, operands);
					return ast;
				} else if(token_list[1]->type == TOKEN_L_PAREN) {
					flag = 0;
					for(int i = 1; i < get_list_size(token_list); i++) {
						if(token_list[i]->type == TOKEN_L_PAREN) {
							flag++;
						}
						if(token_list[i]->type == TOKEN_R_PAREN) {
							flag--;
						}
						if(flag == 0) {
                            operands = 2;
                            potential_operands
                                = initialize_potential_operands(operands);
                            potential_operands[0] = get_sub_list(token_list, 1, i);
                            potential_operands[1] = get_sub_list(token_list,
                                    i + 1, get_list_size(token_list));
							ast->children[0] = generate_tree(potential_operands[0],
                                    st, ast->children[0]);
							ast->children[1] = generate_tree(potential_operands[1],
                                    st, ast->children[1]);
                            free_potential_operands(potential_operands,
                                    operands);
							return ast;
						}
					}
				} else if(token_list[1]->type == TOKEN_WORD && token_list[2]->type == TOKEN_L_BRACKET) {
                    operands = 2;
                    potential_operands = initialize_potential_operands(operands);
					potential_operands[0] = get_sub_list(token_list, 1, 1);
					potential_operands[1] = get_sub_list(token_list, 2, 
                            get_list_size(token_list));
					ast->children[0] = generate_tree(potential_operands[0], st,
                            ast->children[0]);
					ast->children[1] = generate_tree(potential_operands[1], st,
                            ast->children[1]);
                    free_potential_operands(potential_operands, operands);
					return ast;
                }
				break;
            default:
				printf("[ABSTRACT SYNTAX TREE]: Garbage Passed As AST NODE\n"
                        "Exiting\n");
				exit(1);
		}
	}
	fprintf(stderr, "[ABSTRACT SYNTAX TREE]: Something went very wrong\n");
	return NULL;
}


/**
 * This function evaluates the ast (the whole potential_values and result
 * nonsense is to free, which does work and is a neat solution imo, not super
 * efficient tho maybe)
 * @param The ast, symbol table and a linked list that makes freeing the
 * pointers produced by this funciton possible
 * @return The value of the evaluation of the tree
 */
void * evaluate_tree(ast_t * ast, symbol_table_t * st) {
    ter_t ** potential_values;
    ter_t * ter = calloc(1, sizeof(struct TER_T));
    ter_t * ter2 = NULL;
    //TODO fix this code
    // ter never gets freed for '('
    ter_t * tmp;
    int sym_index = 0;
    switch (ast->node->type) {
        case NODE_INT:
            ter->result = calloc(1, sizeof(int));
            *(int *)ter->result = *(int *)ast->node->value;
            ter->type = INTEGER;
            return ter;
        case NODE_WORD:
            sym_index = find_symbol(st, ast->node->name);
            if(sym_index) {
                switch(st->types[sym_index]) {
                    case INTEGER:
                        /** 
                         * We use the st to get values ie we only care about 
                         * interesting index
                         */
                        ter->result = calloc(1, sizeof(int));
                        *((int *)ter->result) = *((int *)get_st_value(st, sym_index));
                        ter->type = INTEGER;
                        return ter;
                    case STRING:
                        ter->result = deep_copy_string((char *)ter->result, (char *)get_st_value(st, sym_index));
                        ter->type = STRING;
                        break;
                    case FLOAT:
                        ter->result = calloc(1, sizeof(double));
                        *((double *)ter->result) = *((double *)get_st_value(st, sym_index));
                        ter->type = FLOAT;
                        return ter;
                    case DATA_FRAME:
                        ter->result = clone_data_frame((data_frame_t *)get_st_value(st, sym_index));
                        ter->type = DATA_FRAME;
                        return ter;
                    case RESERVED:
                        fprintf(stderr, "TMP DEV FLAG, SOMETHING WENT WRONG, RESERVED EVALED\n");
                        exit(1);
                }
            } else {
                ter->result = calloc(1, sizeof(int));
                *(int *)ter->result = st->no_symbols;
                return ter;
            }
            return ter;
        case NODE_STRING:
            ter->type = STRING;
            ter->result = deep_copy_string((char *)ter->result, (char *)ast->node->value);
            return ter;
        case NODE_FLOAT:
            ter->type = FLOAT;
            ter->result = calloc(1, sizeof(double));
            *(double *)ter->result = *(double *)ast->node->value;
            return ter;
        case NODE_L_PAREN:
            free(ter);
            ter2 = evaluate_tree(ast->children[0], st);
            return ter2;
        case NODE_CARROT_POW:
            //NOTE TO SELF you can add ifs over type for operator 100% generic
            potential_values = initialize_potential_values(ast, st);
            ter->type = potential_values[0]->type;
            ter->result = power_operator(potential_values[0]->result, potential_values[1]->result, ter->type); 
            free_potential_values(potential_values, ast);
            return ter;
        case NODE_PLUS:
            potential_values = initialize_potential_values(ast, st);
            ter->type = potential_values[0]->type;
            ter->result = addition_operator(potential_values[0]->result, potential_values[1]->result, ter->type); 
            free_potential_values(potential_values, ast);
            return ter;
        case NODE_MINUS:
            potential_values = initialize_potential_values(ast, st);
            ter->type = potential_values[0]->type;
            ter->result = subtraction_operator(potential_values[0]->result, potential_values[1]->result, ter->type); 
            free_potential_values(potential_values, ast);
            return ter;
        case NODE_STAR_MULT:
            potential_values = initialize_potential_values(ast, st);
            ter->type = potential_values[0]->type;
            ter->result = multiplication_operator(potential_values[0]->result, potential_values[1]->result, ter->type); 
            free_potential_values(potential_values, ast);
            return ter;
        case NODE_FS_DIVIDE:
            potential_values = initialize_potential_values(ast, st);
            ter->type = potential_values[0]->type;
            ter->result = division_operator(potential_values[0]->result, potential_values[1]->result, ter->type); 
            free_potential_values(potential_values, ast);
            return ter;
        case NODE_LESS_THAN:
            potential_values = initialize_potential_values(ast, st);
            ter->type = potential_values[0]->type;
            ter->result = less_than_operator(potential_values[0]->result, potential_values[1]->result, ter->type); 
            free_potential_values(potential_values, ast);
            return ter;
        case NODE_GREATER_THAN:
            potential_values = initialize_potential_values(ast, st);
            ter->type = potential_values[0]->type;
            ter->result = greater_than_operator(potential_values[0]->result, potential_values[1]->result, ter->type); 
            free_potential_values(potential_values, ast);
            return ter;
        case NODE_EQUAL_TEST:
            potential_values = initialize_potential_values(ast, st);
            ter->type = potential_values[0]->type;
            ter->result = equal_test_operator(potential_values[0]->result, potential_values[1]->result, ter->type); 
            free_potential_values(potential_values, ast);
            return ter;
        case NODE_LTE:
            potential_values = initialize_potential_values(ast, st);
            ter->type = potential_values[0]->type;
            ter->result = less_than_equal_to_operator(potential_values[0]->result, potential_values[1]->result, ter->type); 
            free_potential_values(potential_values, ast);
            return ter;
        case NODE_GTE:
            potential_values = initialize_potential_values(ast, st);
            ter->type = potential_values[0]->type;
            ter->result = greater_than_equal_to_operator(potential_values[0]->result, potential_values[1]->result, ter->type); 
            free_potential_values(potential_values, ast);
            return ter;
        case NODE_NE:
            potential_values = initialize_potential_values(ast, st);
            ter->type = potential_values[0]->type;
            ter->result = not_equal_operator(potential_values[0]->result, potential_values[1]->result, ter->type); 
            free_potential_values(potential_values, ast);
            return ter;
        case NODE_ASSIGN:
            ter->result = calloc(1, sizeof(char *));
            tmp = evaluate_tree(ast->children[1], st);
            ter->type = RESERVED;
            if(make_entry(st, ast->children[0]->node->name, tmp->result, ast->children[1]->node->type)) {
                *((char *)ter->result + 0) = 't';
            } else {
                *((char *)ter->result + 0) = 'f';
            }
            switch (tmp->type) {
                case INTEGER:
                    free(tmp->result);
                    free(tmp);
                    break;
                case DATA_FRAME:
                    free_data_frame((data_frame_t *)tmp->result);
                    free(tmp);
                    break;
                case STRING:
                    free(tmp->result);
                    free(tmp);
                    break;
                case FLOAT:
                    free(tmp->result);
                    free(tmp);
                    break;
                case RESERVED:
                        fprintf(stderr, "[EVAL TREE]: trying to free unexpected data\nExiting\n");
                        exit(1);
            }
            return ter;
        case NODE_DATA_FRAME:
            ter->result = clone_data_frame((data_frame_t *)ast->node->value);
            ter->type = DATA_FRAME;
            return ter;
        default:
            fprintf(stderr, "[ABSTRACT SYNTAX TREE]: evaluate_tree function crashed\n");
            exit(1);
            break;
    }
    fprintf(stderr, "[ABSTRACT SYNTAX TREE]: evaluate_tree function crashed\n");
    exit(1);
}

/** This function requires some explaining in my opinion, so here we go :|
 *
 *
 * This function is a type of "wrapper" for making an efficient way to 
 * dynamically allocate a list of token_T**'s while still being able to 
 * reasonaby free them.  The need for this function arises from the fact that
 * across stack frames, the EOL_TOKEN gets lost from the list (as get_sub_list 
 * is called multiple times).  I could replace EOL_TOKEN on all lists, but this
 * would lead to one with two EOL_TOKENs and this would make it very difficult 
 * to free.
 * @param
 * @return
 */
token_T *** initialize_potential_operands(int number_of_operands) {
    token_T *** list_of_list = calloc(number_of_operands,
            sizeof(struct TOKEN_T **));
    return list_of_list;
}

/**
 * The free function for the potential_operands
 * @param the potential_operands and the number of operands
 * @return N/a
 */
void free_potential_operands(token_T *** list_of_list, int number_of_operands) {
    for (int i = 0; i < number_of_operands; i++) {
        free_token_list(list_of_lists[i]);
    }
    free(list_of_list);
}

// TODO ter_t smh
ter_t ** initialize_potential_values(ast_t * ast, symbol_table_t * st) {
    ter_t ** pv = calloc(ast->no_children, sizeof(struct TER_T));
    for(int i = 0; i < ast->no_children; i++) {
        pv[i] =  evaluate_tree(ast->children[i], st);
    }
    return pv;
}

void free_potential_values(ter_t ** ter, ast_t * ast) {
    for (int i = 0; i < ast->no_children; i++) {
        free(ter[i]->result);
        free(ter[i]);
    }
    free(ter);
}

/**
 * This function prints an ast
 * @param the ast
 * @return N/a
 */
void print_tree(ast_t * ast) {
	if(ast->children == NULL) {
		printf("%s\n", ast->node->name);
	} else if(ast->children != NULL) {
		printf("%s\n", ast->node->name);
		for(int i = 0; i < ast->no_children; i++) {
			print_tree(ast->children[i]);
		}
	}
}

/**
 * This function frees a given ast
 * @param the ast
 * @return N/a
 */
void free_tree(ast_t * ast) {
	if(ast->children == NULL) {
		if(ast->node) {
			free_node(ast->node);
		}
		if(ast) {
			free(ast);
		}
	} else if(ast->children != NULL) {
		for(int i = 0; i < ast->no_children; i++) {
			free_tree(ast->children[i]);
		}
		if(ast->children) {
			free(ast->children);
		}
		if(ast->node) {
			free_node(ast->node);
		}
		if(ast) {
			free(ast);
		}
	}
}
