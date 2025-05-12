//
// Created by wolf on 5/11/25.
//

#ifndef MAZES_H
#define MAZES_H
#include "matrix.h"
#include <stdlib.h>
#include <stdio.h>


#define DIR_ROOT  (0)
#define DIR_LEFT  (1)
#define DIR_RIGHT (2)
#define DIR_UP    (4)
#define DIR_DOWN  (8)

typedef struct maze {
    matrix * board;
    size_t root_x;
    size_t root_y;
} maze;

/*
Creates maze reprezented as directed graph.
you need to run origin_shift function on it to make random maze
 */
maze * make_maze(int width, int hight);

/*
Shifhts the maze to the new one , if you want completly new random maze,
i suggested to set iterations to width * height * 10
 */
void origin_shift(maze * maze, int iterations);

void maze_free(maze * maze);

void print_maze(maze * maze);

matrix * gen_maze_to_matrix(maze * maze);

#endif //MAZES_H
