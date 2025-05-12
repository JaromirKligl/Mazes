CFLAGS = -Wall -Wextra -ggdb

maze: maze.o matrix.o
	gcc -o maze maze.o matrix.o

maze.o: maze.c
	gcc  $(CFLAGS) -c maze.c

matrix.o: matrix.c
	gcc  $(CFLAGS) -c matrix.c

clean:
	rm -f maze
	rm -f *.o