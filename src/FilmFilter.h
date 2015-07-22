/*
 * Copyright 2015, Gerasim Troeglazov (3dEyes**), 3dEyes@gmail.com.
 * All rights reserved.
 * Distributed under the terms of the MIT License.
 */

#ifndef _H_P_FILM_FILTER_
#define _H_P_FILM_FILTER_

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <Bitmap.h>

#include "Filter.h"

class PFilmFilter : public PFilter
{
 public:
			PFilmFilter(BBitmap *bitmap);
			PFilmFilter(uint32 *buffer, uint32 size,
				uint32 width, uint32 height);
			~PFilmFilter();
	void	Apply(void);
	void	SetLevel(int level);
	int		Level(void);
private:
	int		fLevel;
};

#endif
