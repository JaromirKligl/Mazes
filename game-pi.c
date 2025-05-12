
#include "led_matrix.h"
#include "mazes.h"


#define FB "/dev/fb0"

#define MAZE_COLOR 0x9B81;
#define BG_COLOR 0x0000;
#define PLAYER_COLOR 0x001F;

struct {
	led_matrix * matrix;
	maze * maze;
	matrix * board;
	size_t player_x;
	size_t player_y;
} game;

uint16_t choose_color(int i, int j){
	int x = game.player_x - 3 + i;
	int y = game.player_y - 3 + j;

	if (x == 0 &&  y ==0)
		return PLAYER_COLOR;

	if ((x >= game.board->x) || (y >= game.board->y))
		return BG_COLOR;

	if (matrix_get(game.board, x, y) == 'X')
	 	return MAZE_COLOR;
	return BG_COLOR;
}

void draw_screen(){
	for(size_t i = 0; i < 8; i++){
		for(size_t j = 0; j < 8; j++){
			fb_put_pixel(game.led_matrix, i, j, choose_color(i,j));
		}
	}
}



int main()
{
	int h = 10;
	int w = 10;

	game.player_x = 0;
	game.player_y = 0;

	game.led_matrix = init_led_matrix(FB, 8, 8);
	game.maze = make_maze(h, w);
	origin_shift(game.maze, h * w * 10);
	game.board = gen_maze_to_matrix(game.maze);

	draw_screen();
	close_led_matrix(m);

}


