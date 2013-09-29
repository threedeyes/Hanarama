/*
 * Copyright 2013, Gerasim Troeglazov (3dEyes**), 3dEyes@gmail.com.
 * All rights reserved.
 * Distributed under the terms of the MIT License.
 */

#ifndef _H_P_NOISE_FILTER_
#define _H_P_NOISE_FILTER_

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <Bitmap.h>

#include "Filter.h"

class PNoiseFilter : public PFilter
{
 public:
			PNoiseFilter(BBitmap *bitmap);
			PNoiseFilter(uint32 *buffer, uint32 size,
				uint32 width, uint32 height);
			~PNoiseFilter();
	void	Apply(void);
	void	SetDispersion(uint8 disp);
	uint8	Dispersion(void);
private:
	uint8	fDispersion;
};

#endif
