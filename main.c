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
 * - try out the tile graphics modes
 * 
 */

#include "gba.h" //memory addresses and type shortcuts
#include "screenmode.h" //video mode bits and SetMode macro
#include "keypad.h" //buttons and input register
#include "gfx.h" //RGB macro, sleep, random

int main(void) {

    SetMode(MODE_4 | BG2_ENABLE); //screenmode.h
    // 240 px wide, 160 px tall
    // 8bpp (bits per pixel), palette of 256 15-bit colors

    // initialize palette
    // index 0 is the transparency index
    (BGPaletteMem)[1] = RGB(0,0,0); // background
    (BGPaletteMem)[2] = RGB(0,10,31); // floor
    (BGPaletteMem)[11] = RGB(31,31,31); // sprite color 1

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
        drawPixelMode4(i, 160-FLOOR, 2);

    // draw floor on the other page too
    REG_DISPCNT = REG_DISPCNT ^ BACKBUFFER;
    for (i = 0; i < 240; i++)
        drawPixelMode4(i, 160-FLOOR, 2);

    while (1) {
        while(REG_DISPSTAT ^ 0x01) ; // wait for VBlank

        // erase old sprite
        drawRect(xpos-5, 160-(ypos+10), 10, 10, 0);

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
        if (KEYS & (KEY_UP | KEY_A) && ypos == FLOOR) // only jump if on floor
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

        // flip page
        REG_DISPCNT = REG_DISPCNT ^ BACKBUFFER;
        
        // draw sprite
        drawRect(xpos-5, 160-(ypos+10), 10, 10, 11);

        while(REG_DISPSTAT & 0x01) ; // wait for VBlank to be over (don't run game logic twice before it's drawn)

    }

}
