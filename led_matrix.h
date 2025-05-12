//
// Created by wolf on 5/12/25.
//

#ifndef LED_MATRIX_H
#define LED_MATRIX_H


#include "maze.h"
#include "matrix.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <linux/fb.h>
#include <netpbm/ppm.h>

#define HEIGHT	(8)
#define WIDTH	(8)

typedef struct led_matrix{
    size_t x;
    size_t y;
    uint16_t *fb;
    int fd;
    struct fb_fix_screeninfo * fb_info;
} led_matrix;

void fb_put_pixel(led_matrix * lm, int x, int y, uint16_t color);
uint16_t scale_RGB_to_16b(unsigned int r, unsigned int g, unsigned b, unsigned int max_val);
void fb_clear(led_matrix * lm, uint16_t color);

led_matrix * init_led_matrix(char * frame_buffer,
                    size_t width,
                    size_t height);

void close_led_matrix(led_matrix * lm);

#endif //LED_MATRIX_H

