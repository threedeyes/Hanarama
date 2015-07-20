/*
 * Copyright 2013, Gerasim Troeglazov (3dEyes**), 3dEyes@gmail.com.
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
	for(int j=0;j<15;j++) {
		int x0 = FMRand() % fWidth;
		int y0 = FMRand() % fHeight;
		int len = FMRand() % (fWidth/25);
		for(int l=0;l<len;l++) {
			fBuffer[y0*fWidth + x0] = 0;
			int dx = 1 - FMRand() % 3;
			int dy = 1 - FMRand() % 3;
			x0 = (x0+dx) % fWidth;
			y0 = (y0+dy) % fHeight;
		}
	}
	for(int j=0;j<120;j++) {
		int x0 = FMRand() % fWidth;
		int y0 = FMRand() % fHeight;
		int len = FMRand() % (fWidth/120);
		for(int l=0;l<len;l++) {
			fBuffer[y0*fWidth + x0] = 0;
			int dx = 1 - FMRand() % 3;
			int dy = 1 - FMRand() % 3;
			x0 = (x0+dx) % fWidth;
			y0 = (y0+dy) % fHeight;
		}
	}
	int lines = FMRand() % 100;
	if(lines > 90) {
		int x0 = FMRand() % fWidth;
		int y0 = FMRand() % fHeight;
		int len = FMRand() % fHeight;
		for(int l=0;l<len;l++) {
			fBuffer[ ((y0+l)%fHeight) * fWidth + x0] = 0;
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
