/* 
 * mode4rect_test written by Seth Tenembaum beginning Jan 8, 2017
 * 
 * The purpose of this document is to determine the more efficient implementation of drawRect_mode4: the current set of conditionals (more cpu) or simply a call to drawPixel_mode4 for each pixel (more writes to VRAM).
 * 
 * Using moreComparisons took 33.5 seconds to fill a 30.8k pixel area 80 times over. moreComparisons completed the same task in 132.5 seconds, making it almost 4 times slower. This concludes that the current drawRect_mode4 implementation is more efficient than the naïve approach.
 * 
 */

#include "gba.h" // memory addresses and type shortcuts
#include "screenmode.h" // video mode bits and SetMode macro
#include "keypad.h" // buttons and input register
#include "gfx.h" // RGB macro, sleep, random
                 // also now has drawing functions and display timing helpers

// current implementation
void drawRect_mode4_moreComparisons(int x, int y, int w, int h, u8 c)
{
    u16 *currentBackBuffer = REG_DISPCNT & BACKBUFFER ? VideoBuffer : BackBuffer;
    int ix, iy;
    for (ix = x/2; ix < (x+w+1)/2; ix++)
        for (iy = y; iy < y+h; iy++)
            if (ix*2 == x-1) // odd left edge, only draw right (most-significant) pixel
                drawPixel_mode4(ix+1, iy, c);
            else if (ix*2 == x+w-1) // odd right edge, only draw left (least-significant) pixel
                drawPixel_mode4(ix, iy, c);
            else // fill both pixels
                (currentBackBuffer)[ ix%(SCREEN_WIDTH/2) + iy*(SCREEN_WIDTH/2) ] = c + (c<<8);
}

// same as drawRect_mode3, only difference is calling drawPixel_mode4.
void drawRect_mode4_moreWrites(int x, int y, int w, int h, u8 c)
{
    int ix, iy;
    for (ix = x; ix < x+w; ix++)
        for (iy = y; iy < y+h; iy++)
            drawPixel_mode4(ix, iy, c);
}

int main(void) {

    SetMode(MODE_4 | BG2_ENABLE); //screenmode.h

    // initialize palette
    *(BGPaletteMem+0x00) = RGB( 0, 0, 0); // black (background color, default)
    *(BGPaletteMem+0x01) = RGB(31, 0, 0); // red
    *(BGPaletteMem+0x02) = RGB( 0,31, 0); // green
    *(BGPaletteMem+0x03) = RGB( 0, 0,31); // blue
    *(BGPaletteMem+0x04) = RGB(31,31, 0); // yellow
    *(BGPaletteMem+0x05) = RGB( 0,31,31); // cyan
    *(BGPaletteMem+0x06) = RGB(31, 0,31); // magenta
    *(BGPaletteMem+0x07) = RGB(31,31,31); // white
    *(BGPaletteMem+0x08) = RGB(20,20,20); // light grey


    // push it to the limit (do enough writes to be able to time the difference)

    int i;
    for (i = 0; i < 10; i++) {
        drawRect_mode4_moreComparisons( 0, 0, 220,140, 1); flipPage(); // red
        drawRect_mode4_moreComparisons( 0, 0, 220,140, 4); flipPage(); // yellow
        drawRect_mode4_moreComparisons(20, 0, 220,140, 2); flipPage(); // green
        drawRect_mode4_moreComparisons(20, 0, 220,140, 5); flipPage(); // cyan
        drawRect_mode4_moreComparisons(20,20, 220,140, 3); flipPage(); // blue
        drawRect_mode4_moreComparisons(20,20, 220,140, 6); flipPage(); // magenta
        drawRect_mode4_moreComparisons( 0,20, 220,140, 8); flipPage(); // light grey
        drawRect_mode4_moreComparisons( 0,20, 220,140, 7); flipPage(); // white
    }
    drawRect_mode4_moreComparisons( 0, 0, 240,160, 0); flipPage(); // black
    drawRect_mode4_moreComparisons( 0, 0, 240,160, 0); flipPage(); // black (other page too)


    Sleep(10);


    for (i = 0; i < 10; i++) {
        drawRect_mode4_moreWrites( 0, 0, 220,140, 1); flipPage(); // red
        drawRect_mode4_moreWrites( 0, 0, 220,140, 4); flipPage(); // yellow
        drawRect_mode4_moreWrites(20, 0, 220,140, 2); flipPage(); // green
        drawRect_mode4_moreWrites(20, 0, 220,140, 5); flipPage(); // cyan
        drawRect_mode4_moreWrites(20,20, 220,140, 3); flipPage(); // blue
        drawRect_mode4_moreWrites(20,20, 220,140, 6); flipPage(); // magenta
        drawRect_mode4_moreWrites( 0,20, 220,140, 8); flipPage(); // light grey
        drawRect_mode4_moreWrites( 0,20, 220,140, 7); flipPage(); // white
    }
    drawRect_mode4_moreWrites( 0, 0, 240,160, 0); flipPage(); // black

    while (1) ; // done, don't let the GBA keep calling main
}
