/*
 * Copyright 2013, Gerasim Troeglazov (3dEyes**), 3dEyes@gmail.com.
 * All rights reserved.
 * Distributed under the terms of the MIT License.
 */

#ifndef _H_P_SEPIA_FILTER_
#define _H_P_SEPIA_FILTER_

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <Bitmap.h>

#include "Filter.h"

class PSepiaFilter : public PFilter
{
 public:
			PSepiaFilter(BBitmap *bitmap);
			PSepiaFilter(uint32 *buffer, uint32 size,
				uint32 width, uint32 height);
			~PSepiaFilter();
	void	Apply(void);
	void	SetDepth(int depth);
	int		Depth(void);
private:
	int		fDepth;
};

#endif
