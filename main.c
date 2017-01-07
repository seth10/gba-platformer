/* 
 * Main written by Seth Tenembaum beginning Jan 2, 2017
 * 
 * The purpose of this document is to come to a better understanding of the fundamentals of game developmeny by building a simple platformer from scratch at a low level (but not quite down to assembly at the moment).
 * 
 * TODO
 * - subpixel positioning and detach movement speed from frame rate
 * - add a single platform
 * - implement a global coordinate system, array of platform positions and sizes, and a camera
 * - try out the tile graphics modes
 * 
 */

#include "gba.h" // memory addresses and type shortcuts
#include "screenmode.h" // video mode bits and SetMode macro
#include "keypad.h" // buttons and input register
#include "gfx.h" // RGB macro, sleep, random
                 // also now has drawing functions and display timing helpers

int main(void) {

    // 240 px wide, 160 px tall
    SetMode(MODE_3 | BG2_ENABLE); //screenmode.h

    s32 xpos = 10, // must be signed (negative (leftward) velocities etc. are possible)
        ypos = 100,
        xvel = 0,
        yvel = 0,
        xacc = 0,
        yacc = 0;

    const s32 GRAV  = 1,
              FLOOR = 30,
              SPEED = 7,
              JUMP  = 10;
    const double AIR_RES = 0.7; // air resistance

    // draw floor
    drawRect(0, SCREEN_HEIGHT-FLOOR, SCREEN_WIDTH, 1, 0,10,31);
    // subtracting the y-value from SCREEN_HEIGHT because 0,0 is the top-left, not bottom-left

    while (1) {

        // don't write to VRAM while the display is being drawn to avoid tearing
        waitForVBlank(); // from gfx.h

        // erase old sprite
        drawRect(xpos-5, SCREEN_HEIGHT-(ypos+10), 10, 10, 0,0,0);

        // get input
        if (isPressedLeft() && !isPressedRight())
            xacc = -1;
        else if (isPressedRight() && !isPressedLeft())
            xacc = 1;
        // decelerate if neither pressed
        else 
            if (xvel > 0)
                xacc = -1;
            else if (xvel < 0)
                xacc = 1;
            else
                xacc = 0;
        // only jump if touching floor
        if ((isPressedUp() || isPressedA()) && ypos == FLOOR) {
            yvel = JUMP;
        }

        // -- SIMULATE -- //
        // apply gravity
        yvel -= GRAV;
        // apply acceleration
        xvel += xacc;
        yvel += yacc;
        // enfore maximum velocities
        if (xvel > SPEED)
            xvel = SPEED;
        if (xvel < -1*SPEED)
           xvel = -1*SPEED;
        // apply velocity
        if (ypos > FLOOR)
            xpos += AIR_RES*xvel; // (horizontal) air resistance
        else
            xpos += xvel; 
        ypos += yvel;
        // wrap x-coordinate
        if (xpos > SCREEN_WIDTH)
            xpos -= SCREEN_WIDTH;
        if (xpos < 0)
            xpos += SCREEN_WIDTH;
        // pop out of floor
        if (ypos < FLOOR) {
            ypos = FLOOR;
            yvel = 0;
        }

        // draw sprite
        drawRect(xpos-5, SCREEN_HEIGHT-(ypos+10), 10, 10, 31,31,31);

        // pause
        ShortSleep(12);

    }

}
