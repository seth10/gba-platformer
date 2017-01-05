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

void drawRect(int x, int y, int w, int h, u8 c)
{
    u16 *here = (~REG_DISPCNT & BACKBUFFER) ? (VideoBuffer) : ((u16*)0x600A000);
    int ix, iy;
    for (ix = x/2; ix < (x+w+1)/2; ix++)
        for (iy = y; iy < y+h; iy++)
            if (ix*2 == x-1) // odd left edge, only draw right (most-significant) pixel
                (here)[ ix%120 + iy*120 ] = c<<8;
            else if (ix*2 == x+w-1) // odd right edge, only draw left (least-significant) pixel
                (here)[ ix%120 + iy*120 ] = c;
            else
                (here)[ ix%120 + iy*120 ] = c + (c<<8); // a cheat implementation just used this statement but rounded down (well, left) odd rectangles to every second pixel (rounded down odd coordinates)
            // when compared to just using two calls to drawPixelMode4, this method saves writes to vram but adds more comparisons (is cpu or vram bandwidth the bottleneck?)
}

void drawPixelMode4(int x, int y, u8 color)
{
    // using 120 instead of 240 because each word, every 16 bits, stores two colors
    u16 *here = ((~REG_DISPCNT & BACKBUFFER) ? (VideoBuffer) : ((u16*)0x600A000)) + (x%240)/2 + (y*240)/2;
    if (x % 2 == 0) // even, left, less-significant pixel
        *here = color + (u8)(*here<<8);
    else // odd, right, more-significant pixel
        *here = (u8)*here + (color<<8);
}
