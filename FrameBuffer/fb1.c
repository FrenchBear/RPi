// fbdraw1.c
// Drawings with frame buffer
// From raspberrycompote.blogspot.com/2014/04/low-level-graphics-on-raspberry-pi.html
// Code from fbtestXX.c and fbtest6.c
// Only using Bresenham algorithms (non aliased) from line and circle
//
// 2016-06-05	PV	Adapted fbtestXX.c to support all depths and not only 8-bit palette

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/ioctl.h>
#include <sys/mman.h>

// default framebuffer palette
typedef enum {
    BLACK        =  0, /*   0,   0,   0 */
    BLUE         =  1, /*   0,   0, 172 */
    GREEN        =  2, /*   0, 172,   0 */
    CYAN         =  3, /*   0, 172, 172 */
    RED          =  4, /* 172,   0,   0 */
    PURPLE       =  5, /* 172,   0, 172 */
    ORANGE       =  6, /* 172,  84,   0 */
    LTGREY       =  7, /* 172, 172, 172 */
    GREY         =  8, /*  84,  84,  84 */
    LIGHT_BLUE   =  9, /*  84,  84, 255 */
    LIGHT_GREEN  = 10, /*  84, 255,  84 */
    LIGHT_CYAN   = 11, /*  84, 255, 255 */
    LIGHT_RED    = 12, /* 255,  84,  84 */
    LIGHT_PURPLE = 13, /* 255,  84, 255 */
    YELLOW       = 14, /* 255, 255,  84 */
    WHITE        = 15  /* 255, 255, 255 */
} COLOR_INDEX_T;

static unsigned short def_r[] =
    { 0,   0,   0,   0, 172, 172, 172, 168,
     84,  84,  84,  84, 255, 255, 255, 255};
static unsigned short def_g[] =
    { 0,   0, 168, 168,   0,   0,  84, 168,
     84,  84, 255, 255,  84,  84, 255, 255};
static unsigned short def_b[] =
    { 0, 172,   0, 168,   0, 172,   0, 168,
     84, 255,  84, 255,  84, 255,  84, 255};

// 'global' variables to store screen info
char *fbp = 0;
struct fb_var_screeninfo vinfo;
struct fb_fix_screeninfo finfo;

// helper function to 'plot' a pixel in given color
void put_pixel_Palette8(int x, int y, int c)
{
    // calculate the pixel's byte offset inside the buffer
    unsigned int pix_offset = x + y * finfo.line_length;

    // now this is about the same as 'fbp[pix_offset] = value'
    *((char*)(fbp + pix_offset)) = c;

}

void put_pixel_RGB565(int x, int y, int r, int g, int b)
{
    // remember to change main(): vinfo.bits_per_pixel = 16;
    // or on RPi just comment out the whole 'Change vinfo'
    // and: screensize = vinfo.xres * vinfo.yres * 
    //                   vinfo.bits_per_pixel / 8;

    // calculate the pixel's byte offset inside the buffer
    // note: x * 2 as every pixel is 2 consecutive bytes
    unsigned int pix_offset = x * 2 + y * finfo.line_length;

    // now this is about the same as 'fbp[pix_offset] = value'
    // but a bit more complicated for RGB565
    unsigned short c = ((r / 8) << 11) + ((g / 4) << 5) + (b / 8);
    // or: c = ((r / 8) * 2048) + ((g / 4) * 32) + (b / 8);
    // write 'two bytes at once'
    *((unsigned short*)(fbp + pix_offset)) = c;

}

void put_pixel_RGB24(int x, int y, int r, int g, int b)
{
    // remember to change main(): vinfo.bits_per_pixel = 24;
    // and: screensize = vinfo.xres * vinfo.yres *
    //                   vinfo.bits_per_pixel / 8;

    // calculate the pixel's byte offset inside the buffer
    // note: x * 3 as every pixel is 3 consecutive bytes
    unsigned int pix_offset = x * 3 + y * finfo.line_length;

    // now this is about the same as 'fbp[pix_offset] = value'
    *((char*)(fbp + pix_offset)) = b;
    *((char*)(fbp + pix_offset + 1)) = g;
    *((char*)(fbp + pix_offset + 2)) = r;

}

void put_pixel_RGB32(int x, int y, int r, int g, int b)
{
    unsigned int pix_offset = x * 4 + y * finfo.line_length;

    *((char*)(fbp + pix_offset)) = b;
    *((char*)(fbp + pix_offset + 1)) = g;
    *((char*)(fbp + pix_offset + 2)) = r;

}

void put_pixel(int x, int y, int c)
{
	if (vinfo.bits_per_pixel == 8) {
		put_pixel_Palette8(x, y, c);
	}
	else if (vinfo.bits_per_pixel == 16) {
		put_pixel_RGB565(x, y, def_r[c], def_g[c], def_b[c]);
	}
	else if (vinfo.bits_per_pixel == 24) {
		put_pixel_RGB24(x, y,  def_r[c], def_g[c], def_b[c]);
	}
	else if (vinfo.bits_per_pixel == 32) {
		put_pixel_RGB32(x, y,  def_r[c], def_g[c], def_b[c]);
	}
}

// helper function to draw a line in given color
// (uses Bresenham's line algorithm)
void draw_line(int x0, int y0, int x1, int y1, int c) {
    int dx = x1 - x0;
    dx = (dx >= 0) ? dx : -dx; // abs()
    int dy = y1 - y0;
    dy = (dy >= 0) ? dy : -dy; // abs()
    int sx;
    int sy;
    if (x0 < x1)
        sx = 1;
    else
        sx = -1;
    if (y0 < y1)
        sy = 1;
    else
        sy = -1;
    int err = dx - dy;
    int e2;
    int done = 0;
    while (!done) {
        put_pixel(x0, y0, c);
        if ((x0 == x1) && (y0 == y1))
            done = 1;
        else {
            e2 = 2 * err;
            if (e2 > -dy) {
                err = err - dy;
                x0 = x0 + sx;
            }
            if (e2 < dx) {
                err = err + dx;
                y0 = y0 + sy;
            }
        }
    }
}

// helper function to draw a rectangle outline in given color
void draw_rect(int x0, int y0, int w, int h, int c) {
    draw_line(x0, y0, x0 + w, y0, c); // top
    draw_line(x0, y0, x0, y0 + h, c); // left
    draw_line(x0, y0 + h, x0 + w, y0 + h, c); // bottom
    draw_line(x0 + w, y0, x0 + w, y0 + h, c); // right
}

// helper function to draw a rectangle outline in given color
void fill_rect(int x0, int y0, int w, int h, int c) {
    int y;
    for (y = 0; y < h; y++) {
        draw_line(x0, y0 + y, x0 + w, y0 + y, c);
    }
}

// helper function to draw a circle outline in given color
// (uses Bresenham's circle algorithm)
void draw_circle(int x0, int y0, int r, int c)
{
    int x = r;
    int y = 0;
    int radiusError = 1 - x;

    while(x >= y)
    {
        // top left
        put_pixel(-y + x0, -x + y0, c);
        // top right
        put_pixel(y + x0, -x + y0, c);
        // upper middle left
        put_pixel(-x + x0, -y + y0, c);
        // upper middle right
        put_pixel(x + x0, -y + y0, c);
        // lower middle left
        put_pixel(-x + x0, y + y0, c);
        // lower middle right
        put_pixel(x + x0, y + y0, c);
        // bottom left
        put_pixel(-y + x0, x + y0, c);
        // bottom right
        put_pixel(y + x0, x + y0, c);

        y++;
        if (radiusError < 0)
        {
            radiusError += 2 * y + 1;
        } else {
            x--;
            radiusError+= 2 * (y - x + 1);
        }
    }
}

// helper function to draw a filled circle in given color
// (uses Bresenham's circle algorithm)
void fill_circle(int x0, int y0, int r, int c) {
    int x = r;
    int y = 0;
    int radiusError = 1 - x;

    while(x >= y)
    {
        // top
        draw_line(-y + x0, -x + y0, y + x0, -x + y0, c);
        // upper middle
        draw_line(-x + x0, -y + y0, x + x0, -y + y0, c);
        // lower middle
        draw_line(-x + x0, y + y0, x + x0, y + y0, c);
        // bottom 
        draw_line(-y + x0, x + y0, y + x0, x + y0, c);

        y++;
        if (radiusError < 0)
        {
            radiusError += 2 * y + 1;
        } else {
            x--;
            radiusError+= 2 * (y - x + 1);
        }
    }
}


// helper function for drawing - no more need to go mess with
// the main function when just want to change what to draw...
void draw() {

    unsigned int x;
    
    // some pixels
    for (x = 0; x < vinfo.xres; x+=5) {
        put_pixel(x, vinfo.yres / 2, WHITE);
    }

    // some lines (note the quite likely 'Moire pattern')
    for (x = 0; x < vinfo.xres; x+=20) {
        draw_line(0, 0, x, vinfo.yres - 1, GREEN);
    }
    
    // some rectangles
    draw_rect(vinfo.xres / 4, vinfo.yres / 2 + 10, vinfo.xres / 4, vinfo.yres / 4, PURPLE);    
    draw_rect(vinfo.xres / 4 + 10, vinfo.yres / 2 + 20, vinfo.xres / 4 - 20, vinfo.yres / 4 - 20, PURPLE);    
    fill_rect(vinfo.xres / 4 + 20, vinfo.yres / 2 + 30, vinfo.xres / 4 - 40, vinfo.yres / 4 - 40, YELLOW);    

    // some circles
    unsigned int d;
    for(d = 10; d < vinfo.yres / 6; d+=10) {
        draw_circle(3 * vinfo.xres / 4, vinfo.yres / 4, d, RED);
    }
    
    fill_circle(3 * vinfo.xres / 4, 3 * vinfo.yres / 4, vinfo.yres / 6, ORANGE);
    fill_circle(3 * vinfo.xres / 4, 3 * vinfo.yres / 4, vinfo.yres / 8, RED);

}


int main(int argc, char* argv[])
{

    int fbfd = 0;
    long int screensize = 0;


    // Open the file for reading and writing
    fbfd = open("/dev/fb0", O_RDWR);
    if (fbfd == -1) {
        printf("Error: cannot open framebuffer device.\n");
        return(1);
    }
    printf("The framebuffer device was opened successfully.\n");

    // Get variable screen information
    if (ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo) == -1) {
        printf("Error reading variable information.\n");
    }
    printf("Original %dx%d, %dbpp\n", vinfo.xres, vinfo.yres,
         vinfo.bits_per_pixel );

    // Get fixed screen information
    if (ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo) == -1) {
        printf("Error reading fixed information.\n");
    }

    // map fb to user mem
    screensize = finfo.smem_len;
    fbp = (char*)mmap(0,
                        screensize,
                        PROT_READ | PROT_WRITE,
                        MAP_SHARED,
                        fbfd,
                        0);

    if ((int)fbp == -1) {
        printf("Failed to mmap.\n");
    }
    else {
        // draw...
        draw();
        sleep(5);
    }

    // cleanup
    // unmap fb file from memory
    munmap(fbp, screensize);

    // close fb file    
    close(fbfd);

    return 0;
}
