
#include "led_matrix.h"

#include "maze.h"


#define FB "/dev/fb0"

#define MAZE_COLOR 0xF81F;
#define BG_COLOR 0x0000;
#define PLAYER_COLOR 0x001F;
#define END_COLOR 0xFC64;

#include <stdlib.h>
#include <stdio.h>

#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/ioctl.h>
#include <poll.h>
#include <linux/input.h>

#define JOYSTICK_DEVID "Raspberry Pi Sense HAT Joystick"
#define INPUT_DEV_PATH "/dev/input"


/**
 * Vyzkousi otevrit zarizeni a podle jmena overi, jestli se jedna joystick.
 * Pokud ano, vrati otevreny file descriptor, jinak vraci -1.
 */
static int try_joystick_dev(char *dev_path)
{
	int fd = open(dev_path, O_RDONLY);
	if (fd < 0) return -1;

	// zkontrolujeme jmeno
	char name[64];
	if (ioctl(fd, EVIOCGNAME(sizeof(name)), name) == -1) {
		close(fd);
		return -1;
	}

	if (!strcmp(name, JOYSTICK_DEVID)) return fd;

	close(fd);
	return -1;

}

/**
 * Mezi vstupnimi zarizenimi najde joystick a vrati jeho file descriptor.
 * Jinak vraci -1.
 */
static int find_joystick_dev()
{
	struct dirent *dir;
	DIR *d = opendir(INPUT_DEV_PATH);
	if (!d) {
		printf("Unable to open dev. directory\n");
		return -1;
	}

	int fd = -1;
	while ((dir = readdir(d)) != NULL) {
		char *name = dir->d_name;
		if (!strncmp(name, "event", 5)) {
			char *full_path = malloc(strlen(INPUT_DEV_PATH) + strlen(name) + 2);
			strcpy(full_path, INPUT_DEV_PATH);
			strcat(full_path, "/");
			strcat(full_path, name);
			fd = try_joystick_dev(full_path);
			if (fd >= 0) {
				printf("Joystick found as device: %s\n", full_path);
				break;
			}
		}
	}
	closedir(d);
	return fd;
}

static int process_event(int fd)
{
	struct input_event ev;
	int cnt = read(fd, &ev, sizeof(ev));
	if (cnt < (int)sizeof(ev)) {
		printf("read error\n");
		return -1;
	} else {
		if (ev.type == EV_KEY) {
			//printf("T:%i:%i:%i\n", ev.type, ev.code, ev.value);
			switch (ev.code) {
			case KEY_UP:
			  return 1;
			  break;

			case KEY_DOWN:
			  return 2;
			  break;

			case KEY_LEFT:
			  return 3;
			  break;
			case KEY_RIGHT:
			  return 4;
			  break;
			}

		}

	}
	return 0;

}


struct {
	led_matrix * matrix;
	maze * maze;
	matrix * board;
	size_t player_x;
	size_t player_y;
} game;

uint16_t choose_color(size_t i, size_t j){
	int x = game.player_x - 3 + i;
	int y = game.player_y - 3 + j;

	if (i == 3 &&  j == 3)
		return PLAYER_COLOR;

	if ((x == (int)game.board->x - 2) &&
	    (y == (int)game.board->y - 2))
	  return END_COLOR;

	if ((x < 0) | (y < 0))
	  return BG_COLOR;

	if ((x >= (int)game.board->x) || (y >= (int)game.board->y))
		return BG_COLOR;

	if (matrix_get(game.board, x, y) == 'X')
	 	return MAZE_COLOR;
	return BG_COLOR;
}


void draw_screen(){
	for(size_t i = 0; i < 8; i++){
		for(size_t j = 0; j < 8; j++){
			fb_put_pixel(game.matrix, i, j, choose_color(i,j));
		}
	}
}

void move_player(int x, int y){
  int new_x = game.player_x + x;
  int new_y = game.player_y + y;

  if (matrix_get(game.board, new_x, new_y) == 'X')
    return;

  game.player_x = new_x;
  game.player_y = new_y;


}



int main(int argv, char * argc[])
{
	int h = 10;
	int w = 10;

	if (argv > 2){
	  w = atoi(argc[1]);
	  h = atoi(argc[2]);
	}

	game.player_x = 1;
	game.player_y = 1;

	game.matrix = init_led_matrix(FB, 8, 8);
	game.maze = make_maze(w, h);
	origin_shift(game.maze, h * w * 10);
	game.board = gen_maze_to_matrix(game.maze);



	int timeout = 1000;
	int fd = find_joystick_dev();

	struct pollfd pfd;
	pfd.fd = fd;
	pfd.events = POLLIN;
	draw_screen();
	while (1) {
		int x = poll(&pfd, 1, timeout);
		if (x < 0) return -1;
		if (pfd.revents & POLLIN) {
			int status = process_event(fd);
			switch(status){
			case -1:
				close(fd);
				close_led_matrix(game.matrix);
				free(game.maze);
				exit(1);
				break;

			case 3:
			  move_player(-1,0);
			  draw_screen();
				break;
			case 4:
			  move_player(1,0);
			  draw_screen();
				break;

			case 0:
			  break;

			case 1:
			  move_player(0,-1);
			  draw_screen();
			  break;

			case 2:
			  move_player(0,1);
			  draw_screen();

			}
		}

		if ((game.player_x == game.board->x - 2) &&
		    (game.player_y == game.board->y - 2))
		  break;


	}
	free(game.maze);
	close(fd);
	close_led_matrix(game.matrix);

}

