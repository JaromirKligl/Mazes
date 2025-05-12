CFLAGS = -Wall -Wextra -ggdb

maze_gen: maze.o matrix.o maze_gen.o
	gcc -o maze_gen maze.o matrix.o maze_gen.o

maze.o: maze.c
	gcc  $(CFLAGS) -c maze.c

matrix.o: matrix.c
	gcc  $(CFLAGS) -c matrix.c

maze_gen.o: maze_gen.c
	gcc $(CFLAGS) -c maze_gen.c

valgrind: maze_gen
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./maze_gen

clean:
	rm -f maze_gen
	rm -f *.o