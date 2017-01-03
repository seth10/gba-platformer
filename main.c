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

int main(void) {

    SetMode(MODE_3 | BG2_ENABLE); //screenmode.h
    // 240 px wide, 160 px tall

    word xpos = 10,
         ypos = 100,
         xvel = 1,
         yvel = 2;

    const word GRAV = 1,
               FLOOR = 30,
               SPEED = 5,
               JUMP = 10;

    while (1) {

        // erase old sprite (point)
        (VideoBuffer)[(160-ypos)*240+xpos] = RGB(0,0,0);

        // get input
        if (KEYS & KEY_LEFT)
            xvel = -1*SPEED;
        if (KEYS & KEY_RIGHT)
            xvel = SPEED;
        if (!KEYS || (!(KEYS & KEY_LEFT) && !(KEYS & KEY_RIGHT))) // oh god please improve this (actually think, simplify)
            xvel = 0;
        if (KEYS & KEY_UP && ypos == FLOOR) // only JUMP if on FLOOR
            yvel = JUMP;

        // simulate
        yvel -= GRAV; // apply gravity
        xpos += xvel; // apply velocity
        ypos += yvel;
        if (ypos < FLOOR) { // pop out of floor
            ypos = FLOOR;
            yvel = 0;
        }

        // draw sprite (point)
        (VideoBuffer)[(160-ypos)*240+xpos] = RGB(31,31,31);

        // pause
        int dummy = 0;
        while (dummy < 5000) dummy++;

    }

}
