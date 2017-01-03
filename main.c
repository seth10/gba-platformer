/* 
 * Main written by Seth Tenembaum beginning Jan 2, 2017
 * 
 * The purpose of this document is to come to a better understanding of the fundamentals of game developmeny by building a simple platformer from scratch at a low level (but not quite down to assembly at the moment).
 * 
 * TODO
 * - fix issue when holding left and right (actually not possible on physical hardware...)
 * - subpixel positioning and detach movement speed from frame rate
 * - add a single platform
 * - implement a global coordinate system, array of platform positions and sizes, and a camera
 * 
 */

#include "gba.h" //memory addresses and type shortcuts
#include "screenmode.h" //video mode bits and SetMode macro
#include "keypad.h" //buttons and input register
#include "gfx.h" //RGB macro, sleep, random

int main(void) {

    SetMode(MODE_3 | BG2_ENABLE); //screenmode.h
    // 240 px wide, 160 px tall

    s32 xpos = 10, // must be signed
        ypos = 100,
        xvel = 0,
        yvel = 0,
        xacc = 0,
        yacc = 0;

    const s32 GRAV  = 1,
              FLOOR = 30,
              SPEED = 7,
              JUMP  = 10;

    // draw floor
    word i;
    for (i = 0; i < 240; i++)
        (VideoBuffer)[(160-FLOOR+1)*240+i] = RGB(0,10,31);

    while (1) {

        // erase old sprite (well, point)
        (VideoBuffer)[(160-ypos)*240+xpos] = RGB(0,0,0);

        // get input
        if (KEYS & KEY_LEFT)
            xacc--;
        if (KEYS & KEY_RIGHT)
            xacc++;
        if (!(KEYS & (KEY_LEFT|KEY_RIGHT))) // decelerate if neither pressed
            if (xvel > 0)
                xacc = -1;
            else if (xvel < 0)
                xacc = 1;
            else
                xacc = 0;
        if (KEYS & KEY_UP && ypos == FLOOR) // only jump if on floor
            yvel = JUMP;

        // simulate
        yvel -= GRAV; // apply gravity

        xvel += xacc; // apply acceleration
        yvel += yacc;

        if (xvel > SPEED) // enfore maximum velocities
            xvel = SPEED;
        if (xvel < -1*SPEED)
           xvel = -1*SPEED;

        // apply velocity
        if (ypos > FLOOR)
            xpos += 0.7*xvel; // air resistance
        else
            xpos += xvel; 
        ypos += yvel;

        if (xpos > 240) // wrap x-coordinate
            xpos -= 240;
        if (xpos < 0)
            xpos += 240;
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
