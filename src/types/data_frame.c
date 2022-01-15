#include"include/data_frame.h"

/**
 * Need to fix circular reference
 */

data_frame_t * init_data_frame(token_T ** token_list) {
    data_frame_t * data_frame = calloc(1, sizeof(struct DATA_FRAME_T));
    data_frame->length = 0;
    //int sym_index = 0;
    int inner_mats = 0;
    int flag = 0;
    int start = 0;
    int end = 0;
    token_type tmp = token_list[1]->type;


    if(tmp == TOKEN_L_BRACKET) {
        for(int i = 1; token_list[i]->type != TOKEN_EOL; i++) {
            if(token_list[i]->type == TOKEN_L_BRACKET) {
                if(flag == 1) {
                    data_frame->length++;
                }
                flag++;
            }
            if(token_list[data_frame->length]->type == TOKEN_R_BRACKET) {
                flag--;
            }
            if(flag == 0) {
                break;
            }
        }
    } else if(tmp == TOKEN_INT) {
        while(token_list[data_frame->length]->type != TOKEN_R_BRACKET) {
            data_frame->length++;
        }
    }
    data_frame->length--;

    // This block accounts for notation like = a []*size
    if(token_list[data_frame->length + 1]->type == TOKEN_STAR_MULT) {
        if(token_list[data_frame->length + 2]->type == TOKEN_INT) {
            data_frame->length = atoi(token_list[data_frame->length + 2]->id);

        } /*else if(token_list[data_frame->length + 2]->type == TOKEN_WORD) {
            sym_index = find_symbol(st, token_list[data_frame->length + 2]->id);
            if(sym_index) {
                data_frame->length = *((int *)st->values[sym_index]);
            } else {
                fprintf(stderr, "[INIT DATA FRAME]: `%s` not defined!\nExiting\n", token_list[data_frame->length + 2]->id);
                exit(1);
            }
        } */else {
            fprintf(stderr, "[INIT DATA FRAME]: Something went very wrong `%s` passed as mat len\nExiting\n", token_list[data_frame->length + 2]->id);
            exit(1);
        }
    }
    switch(tmp) {
        case TOKEN_INT:
            data_frame->type = INTEGER;
            data_frame->comps = calloc(data_frame->length, sizeof(int *));
            for(int i = 0; i < data_frame->length; i++) {
                data_frame->comps[i] = calloc(1, sizeof(int));
                *((int *)data_frame->comps[i]) = atoi(token_list[i + 1]->id);
            }
            return data_frame;
        case TOKEN_L_BRACKET:
            data_frame->type = DATA_FRAME;
            data_frame->comps = calloc(data_frame->length, sizeof(struct DATA_FRAME_T *));
            while((token_list[end]->type == TOKEN_R_BRACKET && token_list[end + 1]->type != TOKEN_SEMICOLON)) {
                if(token_list[end]->type == TOKEN_R_BRACKET && token_list[end + 1]->type == TOKEN_SEMICOLON) {
                    data_frame->comps[inner_mats] = init_data_frame(get_sub_list(token_list, start, end));
                    start = end + 2;
                    inner_mats++;
                }
                end++;
            }
            return data_frame;
        default:
            fprintf(stderr, "[INIT DATA FRAME]: Unexpected token passed to init data_frame\nExiting\n");
            exit(1);
            break;
    }

    return data_frame;
}

data_frame_t * clone_data_frame(data_frame_t * df) {
    data_frame_t * copy = calloc(1, sizeof(struct DATA_FRAME_T));
    copy->length = df->length;
    switch (df->type) {
        case INTEGER:
            copy->comps = calloc(df->length, sizeof(int *));
            for(int i = 0; i < df->length; i++) {
                copy->comps[i] = calloc(1, sizeof(int));
                *((int *)copy->comps[i]) = *((int *)df->comps[i]);
            }
            break;
        case DATA_FRAME:
            copy = calloc(df->length, sizeof(struct DATA_FRAME_T *));
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

void free_data_frame(data_frame_t * df) {
    if(df->type == INTEGER) {
        for(int i = 0; i < df->length; i++) {
            free(df->comps[i]);
        }
        free(df->comps);
        free(df);
    }
}