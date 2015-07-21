#ifndef __T_SCALEBITMAP__
#define __T_SCALEBITMAP__

#include <Application.h>
#include <Alert.h>
#include <Roster.h>
#include <Entry.h>
#include <storage/Path.h>
#include <Node.h>
#include <View.h>
#include <Directory.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <DataIO.h>
#include <BitmapStream.h>
#include <TranslationUtils.h>
#include <TranslatorRoster.h>
#include <Bitmap.h>

#define SColor32(x,y,w,h) (y>=h||x>=w?empty_color : (*( (rgb_color*)src + ((uint32)((y)*(w))+(uint32)((x)) ))))

BBitmap *ConvertBitmapToRGBA( BBitmap *src );
void ScaleBitmap(BBitmap *Src,BBitmap *Dst);

#endif