/*
 * Copyright 2013, Gerasim Troeglazov (3dEyes**), 3dEyes@gmail.com.
 * All rights reserved.
 * Distributed under the terms of the MIT License.
 */
 
#include "FadeFilter.h"

extern "C" {
	uint32 shade_asm(uint32 *ptr, uint32 size, uint8 shade);
}

PFadeFilter::PFadeFilter(BBitmap *bitmap)
	:PFilter(bitmap)
{
}


PFadeFilter::PFadeFilter(uint32 *buffer, uint32 size, uint32 width, uint32 height)
	:PFilter(buffer, size, width, height) 
{
}


PFadeFilter::~PFadeFilter()
{
}


void
PFadeFilter::Apply(void)
{
	shade_asm(fBuffer, fSize, fFade);
}


void
PFadeFilter::SetFade(uint8 fade)
{
	fFade = fade;
}


uint8
PFadeFilter::Fade(void)
{
	return fFade;
}
