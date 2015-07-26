/*
 * Copyright 2015, Gerasim Troeglazov (3dEyes**), 3dEyes@gmail.com.
 * All rights reserved.
 * Distributed under the terms of the MIT License.
 */

#include <stdio.h>
#include <math.h>

#include "ImageView.h"

extern BBitmap* fSrcBitmap;
extern BBitmap* fPreviewBitmap;

ImageView::ImageView(BRect rect) :
	BView(rect, "ImageView", B_FOLLOW_ALL, B_WILL_DRAW|B_FRAME_EVENTS)
{
	SetViewColor(B_TRANSPARENT_32_BIT);
}

ImageView::~ImageView()
{
}

void 
ImageView::FrameResized(float width, float height)
{
	Invalidate();
}

void 
ImageView::Draw(BRect rect)
{
	BBitmap* prevBitmap = fSrcBitmap;
	if(fPreviewBitmap!=NULL)
		prevBitmap = fPreviewBitmap;
	
	if(prevBitmap!=NULL) {
		float width, height, left, top;

		width = Bounds().Width() - 16.0;
		height = width / 2.0;
		left = (Bounds().Width() - width) / 2.0;
		top = (Bounds().Height() - height) / 2.0;
		
		if(height > Bounds().Height()) {
			height = Bounds().Height() - 16.0;
			width = height * 2;
			left = (Bounds().Width() - width) / 2.0;
			top = (Bounds().Height() - height) / 2.0;			
		}
		
		SetDrawingMode(B_OP_COPY);
		SetHighColor(ui_color(B_PANEL_BACKGROUND_COLOR));
		FillRect(Bounds());

		BRect imageRect(left, top, left + width, top + height);
		
		DrawBitmap(prevBitmap, prevBitmap->Bounds(), imageRect);

		SetHighColor(0,0,0);
		StrokeRect(imageRect);
	}
}
