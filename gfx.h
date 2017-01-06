// Gfx.h created 30 Nov 2001
// Desc: Store some of the main graphics routines used by the game


#ifndef GFX_H
#define GFX_H

#include "gba.h"

// GBA stores 15 bit color in blue green red format meaing first 5 
// bits are red, next five green, next blue. What is the 16bit?
#define RGB(r,g,b) ((r)+((g)<<5)+((b)<<10))


// do some looping so the gba will wait
// 125 is a good delay for displaying something
// 1 is a good delay for a game (like 30/60 fps) or something like that
void Sleep(int i);

// about 1ms, ShortSleep(1000) will take ~1 second
void ShortSleep(int i);

// The following code for random was taken from Mines10.zip of gbadev.org, that author
// had the following comment in it:
// random stuff extracted from an email from 'ninge1'
#define RAND_MAX 32767
volatile s32 RAND_RandomData;

void SeedRandom(void);

s32 RAND(s32 Value);

//usage somwhere in program SeedRandom();
//then xrand = RAND(MAX_X); where MAX_X is range upper bound for value you want returned!


// display timing details
// 240 pixels wide, 160 pixels tall, 68 "pixels" of blanking in each direction
// useful to wait for VBlank:  while(REG_VCOUNT < VDraw) ; // wait for VBlank
// if not using the display status register instead:  while(REG_DISPSTAT ^ 0x01) ;
#define HDraw 240
#define HBlank 68
//#define scanline (HDraw+HBlank)
#define VDraw 160
#define VBlank 68
//#define refresh (VDraw+VBlank)

// define names from tonc
#define DSTAT_IN_VBL 0x01
#define DSTAT_IN_HBL 0x02
void waitForHDraw();
void waitForHBlank();
void waitForVDraw();
void waitForVBlank();

// same definition but names more meaningful when not directly dealing with timing and the display buffers
#define SCREEN_WIDTH HDraw
#define SCREEN_HEIGHT VDraw

void flipPage();


// "_mode#" appended because no function overloading in vanilla C :C
void drawPixel_mode3(int x, int y, int red, int green, int blue);

// colors are 0-31 (5 bits per channel), not 0-255!
void drawRect_mode3(int x, int y, int width, int height, int red, int green, int blue); 

// for graphics mode 4, this writes to whichever video buffer is *not* currently being displayed (hidden)!
// because you can't write to VRAM in 1-byte chunks(!) this function will
// read the data from the adjacent pixel and add it to this one for a 2-byte write
void drawPixel_mode4(int x, int y, u8 colorIndex);

// again for mode 4, this writes to whichever video buffer is *not* currently being displayed (hidden)!
// the single color parameter is what index in the color palette to use
void drawRect_mode4(int x, int y, int width, int height, u8 colorIndex);

#endif

