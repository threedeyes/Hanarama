/*
 * Copyright 2013, Gerasim Troeglazov (3dEyes**), 3dEyes@gmail.com.
 * All rights reserved.
 * Distributed under the terms of the MIT License.
 */

#ifndef _H_P_BLUR_FILTER_
#define _H_P_BLUR_FILTER_

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <Bitmap.h>

#include "Filter.h"

class PBlurFilter : public PFilter
{
 public:
			PBlurFilter(BBitmap *bitmap);
			PBlurFilter(uint32 *buffer, uint32 size, uint32 width, uint32 height);
			~PBlurFilter();
	void	Apply(void);
};

#endif
