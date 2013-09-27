/*
 * Copyright 2013, Gerasim Troeglazov (3dEyes**), 3dEyes@gmail.com.
 * All rights reserved.
 * Distributed under the terms of the MIT License.
 */
 
#include "BlurFilter.h"

PBlurFilter::PBlurFilter(BBitmap *bitmap)
	:PFilter(bitmap)
{
}


PBlurFilter::PBlurFilter(uint32 *buffer, uint32 size, uint32 width, uint32 height)
	:PFilter(buffer, size, width, height)
{
}


PBlurFilter::~PBlurFilter()
{
}


void
PBlurFilter::Apply(void)
{
	uint32 ptr;
	int32 r,g,b;
	uint32 color, color1, color2, color3, color4, color5;

	uint8 *c = (uint8*)&color;
	
	uint8 *c1 = (uint8*)&color1;
	uint8 *c2 = (uint8*)&color2;
	uint8 *c3 = (uint8*)&color3;
	uint8 *c4 = (uint8*)&color4;
	uint8 *c5 = (uint8*)&color5;
	
	for(uint32 y = 1; y < fHeight - 1; y++) {
		for(uint32 x = 1; x< fWidth - 1; x++) {
			ptr = y * fWidth + x;
			color1 = fBuffer[ptr-fWidth];
			color2 = fBuffer[ptr-1];
			color3 = fBuffer[ptr];
			color4 = fBuffer[ptr+1];
			color5 = fBuffer[ptr+fWidth];

	        c[0] = (c1[0]+c2[0]+c3[0]+c4[0]+c5[0])/5;
	        c[1] = (c1[1]+c2[1]+c3[1]+c4[1]+c5[1])/5;
	        c[2] = (c1[2]+c2[2]+c3[2]+c4[2]+c5[2])/5;

			fBuffer[ptr] = color;
		}
	}
}
