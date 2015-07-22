/*
 * Copyright 2013, Gerasim Troeglazov (3dEyes**), 3dEyes@gmail.com.
 * All rights reserved.
 * Distributed under the terms of the MIT License.
 */

#ifndef IZMENA_RESIZER_H
#define IZMENA_RESIZER_H

#include <Bitmap.h>
#include <TranslationUtils.h>
#include <Screen.h>
#include <StorageDefs.h>
#include <string.h>

#define SColor(x,y,w,h) (y>=h||x>=w?empty_color : map->color_list[ *((uchar *)src+ (uint32)((y)*w)+(uint32)(x)) ])
#define SAlpha(x,y,w,h) (y>=h||x>=w?0 : (*((uchar *)src+ (uint32)((y)*w)+(uint32)(x)) )==B_TRANSPARENT_8_BIT?0:255)
#define SColor32(x,y,w,h) (y>=h||x>=w?empty_color : (*( (rgb_color*)src + ((uint32)((y)*(w))+(uint32)((x)) ))))

void 	ResizeBitmap(BBitmap *inBitmap, BBitmap *outBitmap);
void	IncreaseBitmap(BBitmap *Src, BBitmap *Dst);
void 	DecreaseBitmap(BBitmap *Src,BBitmap *Dst);

#endif 
