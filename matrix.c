#include <stdio.h>
#include <stdlib.h>
#include "matrix.h"


matrix * make_matrix(size_t x, size_t y){
    matrix * output = (matrix *)malloc(sizeof(matrix));
    char * data = (char *)calloc((x * y), sizeof(char));

    if (output == NULL || data == NULL){
        printf("Allocation Error");
        exit(1);
    }
    output->data = data;
	output->x = x;
	output->y = y;
	return output;
}



char * matrix_place_by_coords(matrix * in, size_t x, size_t y){
	check_values(in, x ,y);
	size_t delim = in->x;
	char * out = in->data;
	out += ((delim*y) + x);
	return out;
}

char matrix_get(matrix * in, size_t x, size_t y){
	char * place = matrix_place_by_coords(in, x, y);
	return *(place);
}

void matrix_set(matrix * in, size_t x, size_t y, char new_value){
	char * place = matrix_place_by_coords(in, x, y);
	*place = new_value;
}

void matrix_inplace_map(matrix * in, char(*fun)(char)){
    char * end_p = in->data + (in->x * in->y);
    for(char * i = in->data; i < end_p; i += 1){
        *i = fun(*i);
    }
}

matrix * matrix_map(matrix * in, char(*fun)(char)){
    matrix * output = make_matrix(in->x, in->y);
    char * out_p = output->data;
    char * end_p = in->data + (in->x * in->y);

    for(char * i = in->data; i < end_p; i += 1, out_p += 1){
        *out_p = fun(*i);
    }
    return output;
}

matrix * matrix_map_2(matrix * in1, matrix * in2, char(*fun)(char,char)){
    if ((in1->x != in2->x ||
        in1->y != in2->y)){
        printf("Cannot map over two matrices with different size");
        exit(1);
    }

    matrix * output = make_matrix(in1->x, in1->y);
    char * in1_p = in1-> data;
    char * in2_p = in2-> data;
    char * out_p = output->data;
    char * end_p = out_p + (in1->x * in1->y);

    while(out_p < end_p){
        *out_p = fun(*in1_p, *in2_p);
        out_p += 1;
        in1_p += 1;
        in2_p += 1;
    }

    return output;
}

void print_matrix(matrix * in){
    char * end_p = in->data + (in->x * in->y);
    for(char * i = in->data; i < end_p;){
        for(size_t j = 0 ; j < in->x; j++, i += 1){
            printf("%c ", *i);
        }
        printf("\n");
    }
}

static char add(char x, char y){
    return x + y;
}

static char sub(char x, char y){
    return x + y;
}

matrix *  matrix_add (matrix * in, matrix * in_2){
    return matrix_map_2(in, in_2, add);
}

matrix *  matrix_sub (matrix * in, matrix * in_2){
    return matrix_map_2(in, in_2, sub);
}

void matrix_free (matrix * in){
    free(in->data);
    in->data = NULL;
    free(in);
    in = NULL;
}

matrix * matrix_mul (matrix * in, matrix * in_2){
    if (in->y != in_2->x){
        printf("Matrices are unstable for multiplication");
        exit(1);
    }
    matrix * output = make_matrix(in->x,in_2->y);
    char * out_p = output->data;
    for (size_t i = 0; i < in->y; i++){
        for(size_t j = 0; j < in_2->x; j++){
            char place = 0;

            for(size_t k = 0; k < in->x; k++){
                place += matrix_get(in, k, i) * matrix_get(in_2, j, k);
            }
            *out_p = place;
            out_p +=1;
        }

    }
    return output;
}
