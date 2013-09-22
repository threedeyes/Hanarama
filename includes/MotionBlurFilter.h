/*
 * Copyright 2013, Gerasim Troeglazov (3dEyes**), 3dEyes@gmail.com.
 * All rights reserved.
 * Distributed under the terms of the MIT License.
 */

#ifndef _H_P_MBLUR_FILTER_
#define _H_P_MBLUR_FILTER_

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <Bitmap.h>

#include "Filter.h"

class PMotionBlurFilter : public PFilter
{
 public:
			PMotionBlurFilter(BBitmap *bitmap);
			PMotionBlurFilter(uint32 *buffer, uint32 size, uint32 width, uint32 height);
			~PMotionBlurFilter();
	void	Apply(void);
};

#endif
