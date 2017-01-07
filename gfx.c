// Implementation of gfx.h stuff

#include "screenmode.h"
#include "gfx.h"

// used to slow down the gba between frames, and for displaying stuff pretty cool eh?
void Sleep(int i)
{
	int x, y;
	int c;
	for (y = 0; y < i; y++)
	{
		for (x = 0; x < 30000; x++)
			c = c + 2; // do something to slow things down
	}
}

void ShortSleep(int i)
{
  int dummy = 0;
  while (dummy < 138*i) dummy++;
}

void SeedRandom(void)
{
   RAND_RandomData = REG_VCOUNT;
}


s32 RAND(s32 Value)
{
   RAND_RandomData *= 20077;
   RAND_RandomData += 12345;

   return ((((RAND_RandomData >> 16) & RAND_MAX) * Value) >> 15);
}


// using the display status register
void waitForHDraw()  { while(REG_DISPSTAT & DSTAT_IN_HBL) ; }
void waitForHBlank() { while(~REG_DISPSTAT & DSTAT_IN_HBL) ; }
void waitForVDraw()  { while(REG_DISPSTAT & DSTAT_IN_VBL) ; }
void waitForVBlank() { while(~REG_DISPSTAT & DSTAT_IN_VBL) ; }
// using the display control register
void flipPage() { REG_DISPCNT ^= BACKBUFFER; }


void drawPixel_mode3(int x, int y, int r, int g, int b)
{
    (VideoBuffer)[ x%SCREEN_WIDTH + y*SCREEN_WIDTH ] = RGB(r,g,b);
}

void drawRect_mode3(int x, int y, int w, int h, int r, int g, int b)
{
    int ix, iy;
    for (ix = x; ix < x+w; ix++)
        for (iy = y; iy < y+h; iy++)
            drawPixel_mode3(ix, iy, r,g,b);
}

void drawPixel_mode4(int x, int y, u8 color)
{
    // using 120 (dividing by 2) instead of 240 because each word, every 16 bits, stores two pixels
    u16 *here = (REG_DISPCNT & BACKBUFFER ? VideoBuffer : BackBuffer) + x%(SCREEN_WIDTH/2) + y*(SCREEN_WIDTH/2);
    if (x % 2 == 0) // even, left, less-significant pixel
        *here = color + (*here & 0xFF00);
    else // odd, right, more-significant pixel
        *here = (*here & 0x00FF) + (color<<8);
}

void drawRect_mode4(int x, int y, int w, int h, u8 c)
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
