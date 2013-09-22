/*
 * Copyright 2013, Gerasim Troeglazov (3dEyes**), 3dEyes@gmail.com.
 * All rights reserved.
 * Distributed under the terms of the MIT License.
 */

#ifndef _H_P_FADE_FILTER_
#define _H_P_FADE_FILTER_

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <Bitmap.h>

#include "Filter.h"

class PFadeFilter : public PFilter
{
 public:
			PFadeFilter(BBitmap *bitmap);
			PFadeFilter(uint32 *buffer, uint32 size, uint32 width, uint32 height);
			~PFadeFilter();
	void	Apply(void);
	void	SetFade(uint8 fade);
	uint8	Fade(void);
 private:
 	uint8	fFade;
};

#endif
