/* 
 * Main written by Seth Tenembaum beginning Jan 1, 1970
 * 
 * The purpose of this document is to...
 * 
 */

#include "gba.h" //memory addresses and type shortcuts
#include "screenmode.h" //video mode bits and SetMode func
#include "keypad.h" //buttons and input register
#include "gfx.h" //RGB macro, sleep, random

int main(void) {

    SetMode(MODE_3 | BG2_ENABLE); //screenmode.h

    // (VideoBuffer)[ x*240 + y ] = RGB(r,g,b);
    int x, y;
    for (x = 50; x < 100; x++)
        for (y = 0; y < 100; y++)
            (VideoBuffer)[ y*240 + x ] = RGB(31,y%31,0);
    
    while (1) {

        // if (KEYS & KEY_UP) {

    }

}
