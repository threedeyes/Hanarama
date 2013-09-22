/*
 * Copyright 2013, Gerasim Troeglazov (3dEyes**), 3dEyes@gmail.com.
 * All rights reserved.
 * Distributed under the terms of the MIT License.
 */
 
#include "Filter.h"

PFilter::PFilter(BBitmap *bitmap)
{
	Init(bitmap);
}


PFilter::PFilter(uint32 *buffer, uint32 size, uint32 width, uint32 height)
{
	Init(buffer, size, width, height);
}

PFilter::~PFilter()
{
}

void
PFilter::Init(BBitmap *bitmap)
{
	fBuffer = (uint32*)bitmap->Bits();
	fSize = bitmap->BitsLength() / sizeof(uint32);
	fWidth = bitmap->Bounds().Width() + 1;
	fHeight = bitmap->Bounds().Height() + 1;
}


void
PFilter::Init(uint32 *buffer, uint32 size, uint32 width, uint32 height)
{
	fBuffer = buffer;
	fSize = size;
	fWidth = width;
	fHeight = height;
}


void
PFilter::Apply(void)
{	
}
