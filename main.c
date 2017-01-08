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

    s16 xpos = 10, // must be signed (negative (leftward) velocities etc. are possible)
        ypos = 100,
        xvel = 0,
        yvel = 0,
        xacc = 0,
        yacc = 0;

    const s16 GRAV  = 1,
              FLOOR = 30,
              SPEED = 7,
              JUMP  = 10;
    const double AIR_RES = 0.7; // air resistance

    const s16 PLATS[][3] = // platforms (x, y, w (,h?))
    {
        {  0, FLOOR    , SCREEN_WIDTH},
        {150, FLOOR+ 20, 50},
        {150, FLOOR+ 45, 50},
        { 20, FLOOR+ 35, 20},
        { 50, FLOOR+ 75, 25},
        { 80, FLOOR+125, 15},
    };

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

        s16 prey = ypos; // record y's previous position before simulation
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
        if (ypos > FLOOR) // how do I apply this to the platforms too, so there isn't air resistance when moving along a platform?
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
        // check for platform collisions
        u8 plati;
        for (plati = 0; plati < sizeof(PLATS)/sizeof(*PLATS); plati++) {
            const s16 *platform = PLATS[plati];
            // check if within x bounds of this platform
            if (xpos > platform[0] && xpos < platform[0]+platform[2]) {
                // if it was previously above the platform and now it is below it
                if (ypos < platform[1] && prey > platform[1]) {
                    // pop up to standing on the platform
                    //ypos = platform[1];
                    yvel = 0;
                    break; // no need to check other platforms
                }
            }
        }

        // draw platform(s)
        for (plati = 0; plati < sizeof(PLATS)/sizeof(*PLATS); plati++)
            drawRect(PLATS[plati][0], SCREEN_HEIGHT-PLATS[plati][1], PLATS[plati][2], 1, 31,10,0);
        // subtracting the y-value from SCREEN_HEIGHT because 0,0 is the top-left, not bottom-left

        // draw sprite
        drawRect(xpos-5, SCREEN_HEIGHT-(ypos+10), 10, 10, 31,31,31);

        // pause
        ShortSleep(16);

    } // end game loop

}
