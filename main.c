/* 
 * Main written by Seth Tenembaum beginning Jan 2, 2017
 * 
 * The purpose of this document is to come to a better understanding of the fundamentals of game developmeny by building a simple platformer from scratch at a low level (but not quite down to assembly at the moment).
 * 
 */

#include "gba.h" //memory addresses and type shortcuts
#include "screenmode.h" //video mode bits and SetMode macro
#include "keypad.h" //buttons and input register
#include "gfx.h" //RGB macro, sleep, random

// void drawRect(int x, int y, int width, int height, int red, int green, int blue);

int main(void) {

    SetMode(MODE_3 | BG2_ENABLE); //screenmode.h
    // 240 px wide, 160 px tall

    // explicitly putting 'p' after the position variable to avoid collisions with temporary variables and possibly to pause a moment to think about if this is the variable I mean to be using
    long xp = 10, // x-position
         yp = 100, // y-position
         xv = 1, // x velocity
         yv = 2; // y velocity

    const long grav = 1, // gravity
               flr = 30; // floor (is a keyword?)

    while (1) {

        // erase old point
        (VideoBuffer)[(160-yp)*240+xp] = RGB(0,0,0);

        // simulate
        yv -= grav; // apply gravity
        xp += xv; // apply velocity
        yp += yv;
        if (yp < flr) { // pop out of floor
            yp = flr;
            yv = 0;
        }

        // draw
        (VideoBuffer)[(160-yp)*240+xp] = RGB(31,31,31);

        // pause
        int dummy = 0, counter;
        for (counter = 0; counter < 8000; counter++) dummy += 2;

    }

}

// void drawRect(int x, int y, int w, int h, int r, int g, int b) {
//     int ix, iy;
//     for (ix = x; ix < x+w; ix++)
//          for (iy = y; iy < y+h; iy++)
//              (VideoBuffer)[ ix + iy*240 ] = RGB(r, g, b);
// }
