/*
 * Copyright 2013, Gerasim Troeglazov (3dEyes**), 3dEyes@gmail.com.
 * All rights reserved.
 * Distributed under the terms of the MIT License.
 */
 
#include "SepiaFilter.h"
#include "FastMath.h"

PSepiaFilter::PSepiaFilter(BBitmap *bitmap)
	:PFilter(bitmap), fDepth(0)
{
}


PSepiaFilter::PSepiaFilter(uint32 *buffer, uint32 size, uint32 width, uint32 height)
	:PFilter(buffer, size, width, height), fDepth(0)
{
}


PSepiaFilter::~PSepiaFilter()
{
}


void
PSepiaFilter::Apply(void)
{
	uint32 ptr;
	uint32 color, color1, color2;

	uint8 *c = (uint8*)&color;	
	uint8 *c1 = (uint8*)&color1;
	uint8 *c2 = (uint8*)&color2;
	
	int p = (FMRand() % 20);
	
	for(uint32 i = 0 ; i < fSize; i++) {
		color1 = fBuffer[i];
		
		int y = (c1[0] + c1[1] + c1[2] ) / 3;
		y -= p;
		if(y<0)y=0;
		//if(y>255)y=255;
		
		int r = y + fDepth * 2;
		int g = y + fDepth;
		int b = y;
		
		if (r <= ((fDepth * 2) -1))
			r = 255;
		if (g <= (fDepth-1))
			g = 255;

		if(r>255)r=255;
		if(g>255)g=255;
		if(b>255)b=255;
		if(r<0)r=0;
		if(g<0)g=0;
		if(b<0)b=0;
		c[0] = b;	
		c[1] = g;
		c[2] = r;
		
		fBuffer[i] = color;
	}
}

void
PSepiaFilter::SetDepth(int depth)
{
	fDepth = depth;
}

int
PSepiaFilter::Depth(void)
{
	return fDepth;
}
