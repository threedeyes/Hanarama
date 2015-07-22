/*
 * Copyright 2015, Gerasim Troeglazov (3dEyes**), 3dEyes@gmail.com.
 * All rights reserved.
 * Distributed under the terms of the MIT License.
 */
 
#include "FilmFilter.h"
#include "FastMath.h"

PFilmFilter::PFilmFilter(BBitmap *bitmap)
	:PFilter(bitmap), fLevel(0)
{
}


PFilmFilter::PFilmFilter(uint32 *buffer, uint32 size, uint32 width, uint32 height)
	:PFilter(buffer, size, width, height), fLevel(0)
{
}


PFilmFilter::~PFilmFilter()
{
}


void
PFilmFilter::Apply(void)
{
	int levelReversed = 100 - fLevel;
	for(int j=0;j < fLevel/8 ;j++) {
		int x0 = FMRand() % fWidth;
		int y0 = FMRand() % fHeight;
		int len = FMRand() % (fWidth/8 + 1);
		for(int l=0;l<len;l++) {
			fBuffer[y0*fWidth + x0] = 0x00ffffff;
			int dx = 1 - FMRand() % 3;
			int dy = 1 - FMRand() % 3;
			x0 = (x0+dx) % fWidth;
			y0 = (y0+dy) % fHeight;
		}
	}
	for(int j=0;j < fLevel/6;j++) {
		int x0 = FMRand() % fWidth;
		int y0 = FMRand() % fHeight;
		int len = FMRand() % (fWidth/16 + 1);
		for(int l=0;l<len;l++) {
			fBuffer[y0*fWidth + x0] = 0x00ffffff;
			int dx = 1 - FMRand() % 3;
			int dy = 1 - FMRand() % 3;
			x0 = (x0+dx) % fWidth;
			y0 = (y0+dy) % fHeight;
		}
	}
	int lines = FMRand() % 100;
	if(lines > 70 + levelReversed/3) {
		int x0 = FMRand() % fWidth;
		int y0 = FMRand() % fHeight;
		int len = FMRand() % fHeight;
		for(int l=0;l<len;l++) {
			fBuffer[ ((y0+l)%fHeight) * fWidth + x0] = 0x00ffffff;
		}
	}
	
	uint32 ptr;
	uint32 color, color1, color2;

	uint8 *c = (uint8*)&color;	
	uint8 *c1 = (uint8*)&color1;
	uint8 *c2 = (uint8*)&color2;

	if(fLevel>0) {
		int y = FMRand() % (fLevel / 4 + 1);
		
		for(uint32 i = 0 ; i < fSize; i++) {
			color1 = fBuffer[i];
			int r = c1[0] - y;
			int g = c1[1] - y;
			int b = c1[2] - y;
			if(r<0)r=0;
			if(g<0)g=0;
			if(b<0)b=0;
			c[0] = r;	
			c[1] = g;
			c[2] = b;
			
			fBuffer[i] = color;
		}
	}
}

void
PFilmFilter::SetLevel(int level)
{
	fLevel = level;
}

int
PFilmFilter::Level(void)
{
	return fLevel;
}
