//
// Created by jaromir on 3/7/25.
//
#ifndef MATRIX_H
#define MATRIX_H

#include <stdio.h>
#include <stdlib.h>

/* Matrix is 1D array of data of size x*y
 * cols are computed as skip of x items
 * */
typedef struct matrix{

    char * data;
    size_t x;
    size_t y;
}
        matrix;


static inline void check_values(matrix * in, size_t x, size_t y){
    if ((in->x <= x) ||
        (in->y <= y)){
        printf("Coords are out of bounds");
        //exit(1);
        }

}

//Creates matrix of size x,y
matrix * make_matrix (size_t x, size_t y);

//sets a value into coords of matrix
void matrix_set(matrix * in, size_t x, size_t y, char new_value);

//gets a value from coords
char matrix_get(matrix * in, size_t x, size_t y);

//maps a function (char -> char) onto matrix, destructively
void matrix_inplace_map(matrix * in, char(*fun)(char));

//Prints a matrix as A1,1 A1,2 A1,3 ... A1,x \n row \n ...
void print_matrix(matrix * in);

//maps a function (char,char -> char) throw two matrices and creates one new (side-effect: none)
matrix * matrix_map_2(matrix * in1, matrix * in2, char(*fun)(char,char));

//maps a function (char -> char) onto matrix and creates a new one (side-effect: none)
matrix * matrix_map(matrix * in, char(*fun)(char));

//free a matrix
void matrix_free (matrix * in);

/* Multiplies two matrices A[X,J] * B[J,Y] -> C[X,Y]
* and creates a new one (side-effect: none)
*/
matrix * matrix_mul (matrix * in, matrix * in_2);

//Adds two matrices together and creates a new one (side-effect: none)
matrix * matrix_add (matrix * in, matrix * in_2);

//Sub two matrices IN - IN2 and creates a new one (side-effect: none)
matrix * matrix_sub (matrix * in, matrix * in_2);

//Maps SQRT function into matrix and creates new one (side-effect: none)
matrix *  matrix_sqrt (matrix * in);
#endif //MATRIX_H
