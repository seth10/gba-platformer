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

void drawRect(int x, int y, int w, int h, int r, int g, int b)
{
    int ix, iy;
    for (ix = x; ix < x+w; ix++)
        for (iy = y; iy < y+h; iy++)
            (VideoBuffer)[ ix + iy*240 ] = RGB(r, g, b);
}
