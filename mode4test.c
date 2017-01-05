/* 
 * Main written by Seth Tenembaum beginning Jan 2, 2017
 * Credit to tonc (https://www.coranac.com/tonc/) for learning and some remarks used as comments.
 * 
 * The purpose of this document is to figure out how graphics mode 4 works through a minimal example.
 * 
 */

#include "gba.h" //memory addresses and type shortcuts
#include "screenmode.h" //video mode bits and SetMode macro
#include "keypad.h" //buttons and input register
#include "gfx.h" //RGB macro, sleep, random

int main(void) {

    SetMode(MODE_4 | BG2_ENABLE); //screenmode.h
    // 240 px wide, 160 px tall

    // graphics mode 4 uses a palette of 256 colors, not just 8-bit color depth

    // initialize palette
    // index 0 is the transparency index
    (BGPaletteMem)[1] = RGB(31,0,0); // consider BGPaletteMem+0x01 or similar
    (BGPaletteMem)[2] = RGB(0,31,0);
    (BGPaletteMem)[3] = RGB(0,0,31);

    // draw image
    // You cannot write to VRAM in byte-sized chunks! If you do the half-word will set two pixels at once, the other becoming black.
    (VideoBuffer)[0] = 0x01 + (0x02<<8);
    (VideoBuffer)[1] = 0x03 + (0x02<<8);
    (VideoBuffer)[2] = 0x01 + (0x00<<8);

    // this will result in the first scanline beginning with red, green, blue, green, red(, black)

}
