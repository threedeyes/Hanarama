/*
 * Copyright 2013, Gerasim Troeglazov (3dEyes**), 3dEyes@gmail.com.
 * All rights reserved.
 * Distributed under the terms of the MIT License.
 */

#ifndef _H_P_FILTER_
#define _H_P_FILTER_

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <Bitmap.h>

#define __USE_ASM_FILTERS__ 1

class PFilter
{
 public:
				PFilter(BBitmap *bitmap);
				PFilter(uint32 *buffer, uint32 size, uint32 width, uint32 height);
				~PFilter();
		void	Init(BBitmap *bitmap);
		void	Init(uint32 *buffer, uint32 size, uint32 width, uint32 height);
virtual void	Apply(void);

 protected:
 	uint32	*fBuffer;
 	uint32	fSize;
 	uint32	fWidth;
 	uint32	fHeight;	
};

#endif
