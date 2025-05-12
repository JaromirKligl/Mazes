#include "maze.h"
#include <stdlib.h>
#include <stdio.h>

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
    maze_free(maze);


    return 0;
}