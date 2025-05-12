#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "maze.h"
#include "matrix.h"



// Not use of void is here correct bcs its constant function
char set_right_dir(char i){
    (void)i;
    return DIR_RIGHT;
}

maze * make_maze(int width, int hight) {
    maze * out = malloc(sizeof(maze));
    if (out == NULL) {
        printf("Error in Allcoation");
        exit(1);
    }
    out->board = make_matrix(width, hight);
    out->root_x = width - 1;
    out->root_y = hight - 1;

    //Setup perfect maze
    matrix_inplace_map(out->board, set_right_dir);

    for(int i = 0; i < hight; i++) {
        matrix_set(out->board, width - 1, i, DIR_DOWN);
    }
    set_at_root(out, DIR_ROOT);
    return out;
}




char choose_direciton(maze * maze) {
    char options[4];
    int count = 0;

    if (maze->root_x < maze->board->x - 1) {
        options[count++] = DIR_RIGHT;
    }
    if (maze->root_x > 0) {
        options[count++] = DIR_LEFT;
    }
    if (maze->root_y > 0) {
        options[count++] = DIR_UP;
    }
    if (maze->root_y < maze->board->y - 1) {
        options[count++] = DIR_DOWN;
    }

    if (count == 0) {
        return 0;
    }

    return options[rand() % count];
}



void origin_shift_step(maze * maze) {

    char direction = choose_direciton(maze);
    set_at_root(maze, direction);
    switch (direction){

        case DIR_RIGHT:
            maze->root_x += 1;
            break;

        case DIR_LEFT:
            maze->root_x -= 1;
            break;

        case DIR_UP:
            maze->root_y -= 1;
            break;

        case DIR_DOWN:
            maze->root_y += 1;
            break;
        default:
            exit(1);
    }
    set_at_root(maze, DIR_ROOT);

}

void origin_shift(maze * maze, int iterations) {
    srand(time(NULL));
    while(iterations) {
        origin_shift_step(maze);
        iterations--;
    }
}



char set_space(char u) {
    (void)u;
    return 'X';
}

int is_even(size_t num) {
    return num % 2;
}

void print_maze(maze * maze) {
    matrix * mm = make_matrix((maze->board->x * 2) + 1,(maze->board->y * 2) + 1);

    matrix_inplace_map(mm,set_space);

    for (size_t i = 0; i < mm->x; i++) {{
        for (size_t j = 0; j < mm->y; j++) {
            if (is_even(j) && is_even(i)){
                matrix_set(mm,i,j,' ');
                }
            }
        }
    }

    for (size_t k = 0; k < maze->board->x; k++) {
        for (size_t j = 0; j < maze->board->y; j++) {
            char direction = matrix_get(maze->board,k,j);
            switch(direction){
                case DIR_RIGHT:
                    matrix_set(mm,(k + 1) * 2, (j * 2) + 1,' ');
                    break;
                case DIR_DOWN:
                    matrix_set(mm,(k * 2) + 1, (j + 1) * 2,' ');
                    break;
                case DIR_UP:
                    matrix_set(mm,(k * 2) + 1, (j * 2),' ');
                    break;
                case DIR_LEFT:
                    matrix_set(mm,(k * 2), (j * 2) + 1,' ');
                    break;
                default:
                    break;
            }
        }
    }

    print_matrix(mm);
    free(mm);

}


int main(int argv, char * argc[]) {
    int x = 5;
    int y = 5;
    if (argv > 2) {
        x = atoi(argc[1]);
        y = atoi(argc[2]);
    }

    maze * maze = make_maze(x,y);
    origin_shift(maze,x * y * 10);
    print_maze(maze);
    return 0;
}



