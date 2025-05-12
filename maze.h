//
// Created by wolf on 5/11/25.
//

#ifndef MAZES_H
#define MAZES_H
#include "matrix.h"

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


static inline void set_at_root(maze * maze, char value) {
    matrix_set(maze->board, maze->root_x, maze->root_y, value);
}



#endif //MAZES_H
