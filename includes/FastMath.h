/*
 * Copyright 2013, Gerasim Troeglazov (3dEyes**), 3dEyes@gmail.com.
 * All rights reserved.
 * Distributed under the terms of the MIT License.
 */

#ifndef _H_FAST_MATH_
#define _H_FAST_MATH_

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <SupportDefs.h>

#define ATAN2_TABLE_SIZE 		4096
#define ATAN2_TABLE_SIZE_NEG 	(-ATAN2_TABLE_SIZE)
#define ACOS_TABLE_SIZE			256000
#define ACOS_TABLE_SIZE_HALF	(ACOS_TABLE_SIZE/2)

extern	int32    ATAN2_TABLE_PPY[];
extern	int32    ATAN2_TABLE_PPX[];
extern	int32    ATAN2_TABLE_PNY[];
extern	int32    ATAN2_TABLE_PNX[];
extern	int32    ATAN2_TABLE_NPY[];
extern	int32    ATAN2_TABLE_NPX[];
extern	int32    ATAN2_TABLE_NNY[];
extern	int32    ATAN2_TABLE_NNX[];
extern	int32	 ACOS_TABLE[];

void FmRecalcTables(float width, float height);

inline int32
FmAtan2(float y, float x)
{
	if (x >= 0) {
		if (y >= 0) {
        	if (x >= y)
            	return ATAN2_TABLE_PPY[(int32)(ATAN2_TABLE_SIZE * y / x)];
            else
                return ATAN2_TABLE_PPX[(int32)(ATAN2_TABLE_SIZE * x / y)];
		} else {
			if (x >= -y)
				return ATAN2_TABLE_PNY[(int32)(ATAN2_TABLE_SIZE_NEG * y / x)];
			else
				return ATAN2_TABLE_PNX[(int32)(ATAN2_TABLE_SIZE_NEG * x / y)];
		}
	} else {
		if (y >= 0) {
			if (-x >= y)
				return ATAN2_TABLE_NPY[(int32)(ATAN2_TABLE_SIZE_NEG * y / x)];
            else
				return ATAN2_TABLE_NPX[(int32)(ATAN2_TABLE_SIZE_NEG * x / y)];
        } else {
			if (x <= y)
				return ATAN2_TABLE_NNY[(int32)(ATAN2_TABLE_SIZE * y / x)];
			else
				return ATAN2_TABLE_NNX[(int32)(ATAN2_TABLE_SIZE * x / y)];
       }
    }
}


inline	int32
FmAcos(float x)
{
	return ACOS_TABLE[ (int32)(x*ACOS_TABLE_SIZE_HALF)+(int32)ACOS_TABLE_SIZE_HALF];
}


inline  float
FmRsqrt(float x)
{
  float xhalf = 0.5f * x;
  int i = *(int*)&x;
  i = 0x5f375a86 - (i>>1);
  x = *(float*)&i;
  x = x * (1.5f - xhalf * x * x);
 // x = x * (1.5f - xhalf * x * x);
  return x;	
}

inline  float
FmRsqrtPrec(float x)
{
  float xhalf = 0.5f * x;
  int i = *(int*)&x;
  i = 0x5f375a86 - (i>>1);
  x = *(float*)&i;
  x = x * (1.5f - xhalf * x * x);
  x = x * (1.5f - xhalf * x * x);
  return x;	
}

#endif
