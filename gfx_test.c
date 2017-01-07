/* 
 * gfx_test written by Seth Tenembaum beginning Jan 7, 2017
 * 
 * The purpose of this document is to test the four graphics drawing helper functions I added to gfx.c.
 * 
 */

#include "gba.h" // memory addresses and type shortcuts
#include "screenmode.h" // video mode bits and SetMode macro
#include "keypad.h" // buttons and input register
#include "gfx.h" // RGB macro, sleep, random
                 // also now has drawing functions and display timing helpers

int main(void) {

    SetMode(MODE_4 | BG2_ENABLE); //screenmode.h

    // initialize palette
    // from GBATEK: Under normal circumstances (light source/viewing angle), the intensities 0-14 are practically all black
    *(BGPaletteMem+0x00) = RGB(10,10,10); // make background a light gray for clarity of what's actually been changed
    *(BGPaletteMem+0x01) = RGB(31,0,0); // consider BGPaletteMem+0x01 or similar
    *(BGPaletteMem+0x02) = RGB(0,31,0);
    *(BGPaletteMem+0x03) = RGB(0,0,31);

    drawPixel_mode4(0,0, 1);
    drawPixel_mode4(1,0, 1);
    drawPixel_mode4(0,1, 1);
    drawPixel_mode4(1,1, 2);
    drawPixel_mode4(10,10, 3);
    drawPixel_mode4(5,10, 2);
    //drawPixel_mode4(0,0, 1);
    //drawPixel_mode4(0,0, 1);
    //drawPixel_mode4(0,0, 1);
    //drawPixel_mode4(0,0, 1);
    // flip page
    REG_DISPCNT ^= BACKBUFFER;

}
