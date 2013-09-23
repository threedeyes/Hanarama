/*
 * Copyright 2013, Gerasim Troeglazov (3dEyes**), 3dEyes@gmail.com.
 * All rights reserved.
 * Distributed under the terms of the MIT License.
 */

#include <math.h>
#include "FastMath.h"

int32    ATAN2_TABLE_PPY[ATAN2_TABLE_SIZE + 1];
int32    ATAN2_TABLE_PPX[ATAN2_TABLE_SIZE + 1];
int32    ATAN2_TABLE_PNY[ATAN2_TABLE_SIZE + 1];
int32    ATAN2_TABLE_PNX[ATAN2_TABLE_SIZE + 1];
int32    ATAN2_TABLE_NPY[ATAN2_TABLE_SIZE + 1];
int32    ATAN2_TABLE_NPX[ATAN2_TABLE_SIZE + 1];
int32    ATAN2_TABLE_NNY[ATAN2_TABLE_SIZE + 1];
int32    ATAN2_TABLE_NNX[ATAN2_TABLE_SIZE + 1];
int32	 ACOS_TABLE[ACOS_TABLE_SIZE + 1];

uint32	 FM_RND_SEED = 2345345;

void
FmRecalcTables(float width, float height)
{
	for (int i = 0; i <= ACOS_TABLE_SIZE; i++) {
		float x = (((float)i / (float)ACOS_TABLE_SIZE) * 2.0) - 1.0;
		int32 y = (int32)((acos(x) * height) / M_PI) * (int32)width;
		ACOS_TABLE[i] = y;
	}
	
	for (int i = 0; i <= ATAN2_TABLE_SIZE; i++) {
		float f = (float)i / ATAN2_TABLE_SIZE;
		
		float ATAN2_TABLE_PPYi = atan(f);
		float ATAN2_TABLE_PPXi = M_PI * 0.5f - ATAN2_TABLE_PPYi;
		float ATAN2_TABLE_PNYi = -ATAN2_TABLE_PPYi;
		float ATAN2_TABLE_PNXi = ATAN2_TABLE_PPYi - M_PI * 0.5f;
		float ATAN2_TABLE_NPYi = M_PI - ATAN2_TABLE_PPYi;
		float ATAN2_TABLE_NPXi = ATAN2_TABLE_PPYi + M_PI * 0.5f;
		float ATAN2_TABLE_NNYi = ATAN2_TABLE_PPYi - M_PI;
		float ATAN2_TABLE_NNXi = -M_PI * 0.5f - ATAN2_TABLE_PPYi;
				
		ATAN2_TABLE_PPY[i] = (int32)(ATAN2_TABLE_PPYi * (width * 0.5 / M_PI));
		ATAN2_TABLE_PPX[i] = (int32)(ATAN2_TABLE_PPXi * (width * 0.5 / M_PI));
		ATAN2_TABLE_PNY[i] = (int32)(ATAN2_TABLE_PNYi * (width * 0.5 / M_PI));
		ATAN2_TABLE_PNX[i] = (int32)(ATAN2_TABLE_PNXi * (width * 0.5 / M_PI));
		ATAN2_TABLE_NPY[i] = (int32)(ATAN2_TABLE_NPYi * (width * 0.5 / M_PI));
		ATAN2_TABLE_NPX[i] = (int32)(ATAN2_TABLE_NPXi * (width * 0.5 / M_PI));
		ATAN2_TABLE_NNY[i] = (int32)(ATAN2_TABLE_NNYi * (width * 0.5 / M_PI));
		ATAN2_TABLE_NNX[i] = (int32)(ATAN2_TABLE_NNXi * (width * 0.5 / M_PI));
	}	
}
