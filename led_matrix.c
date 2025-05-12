//
// Created by wolf on 5/12/25.
//

#include "maze.h"
#include "matrix.h"
#include "led_matrix.h"





/** vycisti obsah framebufferu zadanou barvou */
void fb_clear(led_matrix * lm, uint16_t color)
{
    for (size_t i = 0; i < lm->x * lm->y; i++)
        lm->fb[i] = color;
}


/** vlozi pixel zadane barvy na da souradnice x, y */
void fb_put_pixel(led_matrix * lm, int x, int y, uint16_t color)
{
    lm->fb[y * lm->x + x] = color;
}

/** prevede RGB barvu na format 5:6:5, max_val udava maximalni hodnotu slozek r, g, b */
uint16_t scale_RGB_to_16b(unsigned int r, unsigned int g, unsigned b, unsigned int max_val)
{
    double d = max_val;
    // protoze pro mene vyznamne bity jsou zmeny barev minimalni
    // preskalujeme na mensi rozsah, aby nizsi bity byly 0
    uint16_t sr = (r / d) * 16;
    uint16_t sg = (g / d) * 32;
    uint16_t sb = (b / d) * 16;

    return (sr << 11) | (sg << 5) | sb;
}

led_matrix * init_led_matrix(char * frame_buffer,
                    size_t width,
                    size_t height){

    int fd = open(frame_buffer, O_RDWR);
    if (fd < 0) {
        printf("unable to open framebuffer");
        exit(1);
    }

    struct fb_fix_screeninfo * fb_info = malloc(sizeof(struct fb_fix_screeninfo));
    if (fb_info == NULL){
        printf("Error in Allocation");
        exit(1);
    }

    if (ioctl(fd, FBIOGET_FSCREENINFO, fb_info) == -1) {
        printf("unable to obtain device info\n");
        exit(1);
    }

    // namapujeme pamet
    uint16_t *fb = mmap(NULL, width * height * 2,
                        PROT_READ | PROT_WRITE, MAP_SHARED,
                        fd, 0);
    if (fb == NULL) {
        printf("unable to map framebuffer");
        exit(1);
    }

    led_matrix * out = malloc(sizeof(led_matrix));
    if (out == NULL){
        printf("Error in Allocation");
        exit(1);
        }

    out->fb = fb;
    out->fb_info = fb_info;
    out->y = height;
    out->x = width;
    out->fd = fd;

    return out;
}

void close_led_matrix(led_matrix * lm){
    munmap(lm->fb, lm->x * lm->y * 2);
    close(lm->fd);
    free(lm->fb_info);
    free(lm);
}
