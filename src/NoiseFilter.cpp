/*
 * Copyright 2013, Gerasim Troeglazov (3dEyes**), 3dEyes@gmail.com.
 * All rights reserved.
 * Distributed under the terms of the MIT License.
 */
 
#include "NoiseFilter.h"
#include "FastMath.h"


#ifdef __USE_ASM_FILTERS__
extern "C" {
	uint32 noise_asm(uint32 *ptr, uint32 size, uint32 disp);
}
#endif


PNoiseFilter::PNoiseFilter(BBitmap *bitmap)
	:PFilter(bitmap), fDispersion(0)
{	
}


PNoiseFilter::PNoiseFilter(uint32 *buffer, uint32 size, uint32 width, uint32 height)
	:PFilter(buffer, size, width, height), fDispersion(0)
{
}


PNoiseFilter::~PNoiseFilter()
{
}


void
PNoiseFilter::Apply(void)
{
#ifdef __USE_ASM_FILTERS__
	noise_asm(fBuffer, fSize, (int32)fDispersion);
#else
	uint32 ptr;
	uint32 color, color1, color2;

	uint8 *c = (uint8*)&color;	
	uint8 *c1 = (uint8*)&color1;
	uint8 *c2 = (uint8*)&color2;
	
	for(uint32 i = 0 ; i < fSize; i++) {
		color1 = fBuffer[i];
		int y = FMRand() % fDispersion - fDispersion / 2;
		int r = c1[0] + y;
		int g = c1[1] + y;
		int b = c1[2] + y;
		if(r>255)r=255;
		if(g>255)g=255;
		if(b>255)b=255;
		if(r<0)r=0;
		if(g<0)g=0;
		if(b<0)b=0;
		c[0] = r;	
		c[1] = g;
		c[2] = b;
		
		fBuffer[i] = color;
	}
#endif
}

void
PNoiseFilter::SetDispersion(uint8 disp)
{
	fDispersion = disp;
}

uint8
PNoiseFilter::Dispersion(void)
{
	return fDispersion;
}
