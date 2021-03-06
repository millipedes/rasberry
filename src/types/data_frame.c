/**
* @file data_frame.c
* @brief This file contains the functions relating to the data_frame type
* @author Matthew C. Lindeman
* @date
* @bug None Currently
* @todo Nothing Currently
*/
#include"include/data_frame.h"

/**
 * This function initializes a data frame from a token list.
 * @param The token list
 * @return the data frame
 */
data_frame_t * init_data_frame(token_T ** token_list) {
    data_frame_t * data_frame = calloc(1, sizeof(struct DATA_FRAME_T));
    data_frame->length = 0;
    token_T *** p_df = NULL;
    p_df_index_t * pdfi = init_p_df_index_t();
    int tl_index = 1;
    /*Starts at 1 because the first index will be L_BRACKET*/
    int flag = 0;
    
    if(token_list[tl_index]->type == TOKEN_L_BRACKET) {
        do {
            switch (token_list[tl_index]->type) {
                case TOKEN_R_BRACKET:
                    flag--;
                    if(flag == 0) {
                        if(pdfi->size == 0) {
                            pdfi->bracs[0] = tl_index;
                            pdfi->size++;
                        } else {
                            pdfi->size++;
                            pdfi->bracs = realloc(pdfi->bracs, pdfi->size * sizeof(int));
                            pdfi->bracs[pdfi->size - 1] = tl_index;
                        }
                    }
                    break;
                case TOKEN_L_BRACKET:
                    flag++;
                    if(flag == 1) {
                        data_frame->length++;
                    }
                    break;
                default:
                    break;
            }
            tl_index++;
        } while(flag != -1);

        data_frame->comps = calloc(data_frame->length, sizeof(struct DATA_FRAME_T *));
        data_frame->type = DATA_FRAME;
        p_df = initialize_potential_data_frames(data_frame->length);
        int j = 1;
        for(int i = 0; i < data_frame->length; i++) {
            p_df[i] = get_sub_list(token_list, j, pdfi->bracs[i]);
            data_frame->comps[i] = init_data_frame(p_df[i]);
            j = pdfi->bracs[i];
            j++;
        }

    } else if(token_list[tl_index]->type == TOKEN_INT) {
        while(token_list[tl_index]->type != TOKEN_R_BRACKET) {
            tl_index++;
            data_frame->length++;
        }
        data_frame->comps = calloc(data_frame->length, sizeof(int *));
        data_frame->type = INTEGER;
        for (int i = 0; i < data_frame->length; i++) {
            ((int **)data_frame->comps)[i] = calloc(1, sizeof(int));
            *((int **)data_frame->comps)[i] = atoi(token_list[i + 1]->id);
        }
    } else if(token_list[tl_index]->type == TOKEN_STRING) {
        while(token_list[tl_index]->type != TOKEN_R_BRACKET) {
            tl_index++;
            data_frame->length++;
        }
        data_frame->comps = calloc(data_frame->length, sizeof(char *));
        data_frame->type = STRING;
        for (int i = 0; i < data_frame->length; i++) {
            data_frame->comps[i] = deep_copy_string(((char **)data_frame->comps)[i], token_list[i + 1]->id);
        }
    } else if(token_list[tl_index]->type == TOKEN_FLOAT) {
        while(token_list[tl_index]->type != TOKEN_R_BRACKET) {
            tl_index++;
            data_frame->length++;
        }
        data_frame->comps = calloc(data_frame->length, sizeof(double *));
        data_frame->type = FLOAT;
        for (int i = 0; i < data_frame->length; i++) {
            ((double **)data_frame->comps)[i] = calloc(1, sizeof(double));
            *((double **)data_frame->comps)[i] = atof(token_list[i + 1]->id);
        }
    }

    if(p_df) {
        free_potential_data_frames(p_df, data_frame->length);
    }
    free_p_df_index_t(pdfi);
    return data_frame;
}

data_frame_t * clone_data_frame(data_frame_t * df) {
    data_frame_t * copy = calloc(1, sizeof(struct DATA_FRAME_T));
    copy->length = df->length;
    switch (df->type) {
        case INTEGER:
            copy->type = df->type;
            copy->comps = calloc(df->length, sizeof(int *));
            for(int i = 0; i < df->length; i++) {
                copy->comps[i] = calloc(1, sizeof(int));
                *((int *)copy->comps[i]) = *((int *)df->comps[i]);
            }
            break;
        case FLOAT:
            copy->type = df->type;
            copy->comps = calloc(df->length, sizeof(double *));
            for(int i = 0; i < df->length; i++) {
                copy->comps[i] = calloc(1, sizeof(double));
                *((double *)copy->comps[i]) = *((double *)df->comps[i]);
            }
            break;
        case STRING:
            //printf("TYPE: %d\n", df->type);
            copy->type = df->type;
            copy->comps = calloc(df->length, sizeof(char **));
            for (int i = 0; i < df->length; i++) {
                copy->comps[i] = deep_copy_string(((char **)copy->comps)[i], ((char **)df->comps)[i]);
            }
            break;
        case DATA_FRAME:
            copy->type = df->type;
            copy->comps = calloc(df->length, sizeof(struct DATA_FRAME_T *));
            for(int i = 0; i < df->length; i++) {
                copy->comps[i] = clone_data_frame((data_frame_t *)df->comps[i]);
            }
            break;
        case RESERVED:
            fprintf(stderr, "[CLONE DATA] Error\nExiting\n");
            break;
    }
    return copy;
}

char * clone_string(char * src, char * dest) {
    size_t len = strnlen(src, MAX_LEN);
	dest = calloc(len + 1, sizeof(char));
	for(int i = 0; i < strnlen(src, MAX_LEN) + 1; i++) {
		*(dest + i) = *(src + i);
	}
	return dest;
}

/**
 * This funciton generates a deep copy of the list between start and end 
 * inclusive. If the sequence is not terminated by an EOL_TOKEN then this
 * function adds one.
 * @param The token_list, the start index (starting at 0), and the end index
 * (starting at 0).
 * @return The sub list of tokens
 */
token_T ** get_sub_list(token_T ** list, int start, int end) {
	if(start > end) {
		fprintf(stderr, "[ABSTRACT SYNTAX TREE]: from get_sub_list START: `%d`"
                " END `%d`\nExiting", start, end);
		exit(1);
	}
    token_T ** sub_list;

	if(start == end) {
		token_T ** sub_list = calloc(2, sizeof(struct TOKEN_T *));
		sub_list[0] = init_token(list[start]->id, list[start]->type);
        sub_list[1] = init_token((char *)"0", TOKEN_EOL);
		return sub_list;
	}

    // get_list_size rets list[size]->type == EOL, i.e. check one more
    if(list[end]->type == TOKEN_EOL) {
        sub_list = calloc(end - start + 1, sizeof(struct TOKEN_T *));
        for(int i = start; i < (end + 1); i++) {
            sub_list[i - start] = init_token(list[i]->id, list[i]->type);
        }
    } else {
        sub_list = calloc(end - start + 2, sizeof(struct TOKEN_T *));
        for(int i = start; i < (end + 2); i++) {
            if(i == end + 1) {
                    sub_list[i - start] = init_token((char *)"0", TOKEN_EOL);
            } else {
                sub_list[i - start] = init_token(list[i]->id, list[i]->type);
            }
        }
    }
    return sub_list;
}

token_T *** initialize_potential_data_frames(int number_of_df) {
    token_T *** list_of_list = calloc(number_of_df,
            sizeof(struct token_T **));
    return list_of_list;
}

void free_potential_data_frames(token_T *** list_of_list, int number_of_data_frames) {
    for (int i = 0; i < number_of_data_frames; i++) {
        for (int j = 0; list_of_list[i][j]->type != TOKEN_EOL; j++) {
            free_token(list_of_list[i][j]);
            if(list_of_list[i][j + 1]->type == TOKEN_EOL) {
                free_token(list_of_list[i][j + 1]);
                break;
            }
        }
        free(list_of_list[i]);
    }
    free(list_of_list);
}

/**
 * This function gets the size of a given token_list
 * @param the token list
 * @return th size as an integer
 */
int get_list_size(token_T ** list) {
	int size = 0;
	for(int i = 0; list[i]->type != TOKEN_EOL; i++) {
		size++;
	}
	return size;
}

void print_data_frame(data_frame_t * df) {
    switch (df->type) {
        case INTEGER:
            printf("[");
            for(int i = 0; i < df->length; i++) {
                if(i == 0) {
                    printf("%d", *((int **)df->comps)[i]);
                } else {
                    printf(" %d", *((int **)df->comps)[i]);
                }
            }
            printf("]");
            break;
        case FLOAT:
            printf("[");
            for(int i = 0; i < df->length; i++) {
                if(i == 0) {
                    printf("%lf", *((double *)df->comps[i]));
                } else {
                    printf(" %lf", *((double *)df->comps[i]));
                }
            }
            printf("]");
            break;
        case STRING:
            printf("[");
            for(int i = 0; i < df->length; i++) {
                if(i == 0) {
                    printf("%s", (char *)df->comps[i]);
                } else {
                    printf(" %s", (char *)df->comps[i]);
                }
            }
            printf("]");
            break;
        case DATA_FRAME:
            printf("[");
            for (int i = 0; i < df->length; i++) {
                print_data_frame(((data_frame_t **)df->comps)[i]);
            }
            printf("]");
            break;
        case RESERVED:
            fprintf(stderr, "[PRINT DATA FRAME]: RESERVED data frame type\nExiting\n");
            exit(1);
    }
}

void free_data_frame(data_frame_t * df) {
    if(df->type == INTEGER) {
        for(int i = 0; i < df->length; i++) {
            free(((int **)df->comps)[i]);
        }
        if(df->comps) {
            free(df->comps);
        }
        if(df) {
            free(df);
        }
    } else if(df->type == DATA_FRAME) {
        for(int i = 0; i < df->length; i++) {
            free_data_frame(((data_frame_t **)df->comps)[i]);
        }
        if(df->comps) {
            free(df->comps);
        }
        if(df) {
            free(df);
        }
    } else if(df->type == FLOAT) {
        for(int i = 0; i < df->length; i++) {
            free(((double **)df->comps)[i]);
        }
        if(df->comps) {
            free(df->comps);
        }
        if(df) {
            free(df);
        }
    } else if(df->type == STRING) {
        for(int i = 0; i < df->length; i++) {
            free(((char **)df->comps)[i]);
        }
        if(df->comps) {
            free(df->comps);
        }
        if(df) {
            free(df);
        }
    }
}

p_df_index_t * init_p_df_index_t(void) {
    p_df_index_t * pdfi = calloc(1, sizeof(struct P_DF_INDEX_T));
    pdfi->dfe_type = RESERVED;
    pdfi->bracs = calloc(1, sizeof(int));
    pdfi->size = 0;
    return pdfi;
}

void free_p_df_index_t(p_df_index_t * pdfi) {
    if(pdfi->bracs) {
        free(pdfi->bracs);
    }
    if(pdfi) {
        free(pdfi);
    }
}


void * df_addition_operator(void * value_one, void * value_two) {
    return NULL; 
}

void * df_subtraction_operator(void * value_one, void * value_two) {
   return NULL; 
}

void * df_division_operator(void * value_one, void * value_two) {
   return NULL; 
}

void * df_multiplication_operator(void * value_one, void * value_two) {
   return NULL; 
}

void * df_power_operator(void * value_one, void * value_two) {
   return NULL; 
}

void * df_less_than_operator(void * value_one, void * value_two) {
   return NULL; 
}

void * df_greater_than_operator(void * value_one, void * value_two) {
   return NULL; 
}

void * df_equal_test_operator(void * value_one, void * value_two) {
    return NULL;
}

void * df_less_than_equal_to_operator(void * value_one, void * value_two) {
    return NULL;
}

void * df_greater_than_equal_to_operator(void * value_one, void * value_two) {
    return NULL;
}

void * df_not_equal_operator(void * value_one, void * value_two) {
    return NULL;
}

