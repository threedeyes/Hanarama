/*
 * Copyright 2010 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
 
#include <stdio.h>
#include <math.h>

#include "TestApplication.h"
#include "TestWindow.h"
#include "FBView.h"

extern PCamera *fCam;

FBView::FBView(BRect rect, BPath path) : 
	BView(rect, "FBView", B_FOLLOW_ALL, B_WILL_DRAW|B_PULSE_NEEDED|B_FRAME_EVENTS)
{
	FBView(rect, rect.IntegerWidth(), rect.IntegerHeight(), path);
}

FBView::FBView(BRect rect, int width, int height, BPath path) : 
	BView(rect, "FBView", B_FOLLOW_ALL, B_WILL_DRAW|B_PULSE_NEEDED|B_FRAME_EVENTS)
{
	buffer_width = width;
	buffer_height = height;
	
	filename = path;
	
	BRect	fbRect = BRect(0,0,buffer_width-1,buffer_height-1);	
	bufferBitmap = new BBitmap(fbRect, B_RGB32, true);	

	mouseIsDown = false;
	mouseDownPosLastX=0;
	mouseDownPosLastY=0;	
	time=-12;
}

FBView::~FBView()
{
	
}

void 
FBView::Paint()
{
  if(LockLooper())	{
 	 //bufferView->LockLooper();
 	 SetDrawingMode(B_OP_COPY);
	 DrawBitmap(bufferBitmap, bufferBitmap->Bounds(), Bounds(), B_FILTER_BITMAP_BILINEAR|B_WAIT_FOR_RETRACE);
	 //bufferView->UnlockLooper();
	 UnlockLooper();
	}
}

BBitmap	*
FBView::GetBitmap()
{
	return bufferBitmap;
}

uint32 *
FBView::GetBuffer()
{
	if(bufferBitmap) {
		return (uint32*)bufferBitmap->Bits();
	}
	return NULL;
}

uint32	
FBView::GetBufferSize()
{
	if(bufferBitmap) {
		return bufferBitmap->BitsLength()/4;
	}
	return 0;
}

int
FBView::Width() 
{
	return buffer_width;
}

int
FBView::Height()
{
	return buffer_height;
}

void 
FBView::MouseMoved(BPoint p, uint32 transit,const BMessage *message)
{
	if(fCam!=NULL) 
	if(mouseIsDown==true && fCam->Mode()==CAM_MODE_MANUAL) {
		fCam->MoveHeadingBy(-(p.x-mouseDownPosLastX)*M_PI/180.0);
		fCam->MovePitchBy(0.5*(p.y-mouseDownPosLastY)*M_PI/180.0);
		mouseDownPosLastX=p.x;
		mouseDownPosLastY=p.y;	
	}
}

void
FBView::MouseDown(BPoint p)
{
	if(fCam!=NULL)
	if(fCam->Mode()==CAM_MODE_MANUAL) {
		mouseIsDown=true;
		mouseDownPosLastX=p.x;
		mouseDownPosLastY=p.y;
	}
}

void
FBView::MouseUp(BPoint p)
{
	if(fCam!=NULL)
	if(fCam->Mode()==CAM_MODE_MANUAL)
		mouseIsDown=false;
}

void
FBView::MouseWheelChanged(BMessage *msg)
{
	float dy = 0, dx = 0;
	if (msg->FindFloat("be:wheel_delta_x", &dx) != B_OK)
		dx = 0;
	if (msg->FindFloat("be:wheel_delta_y", &dy) != B_OK)
		dy = 0;

	if(fCam->Mode()==CAM_MODE_MANUAL && dy != 0) {
		fCam->MoveFOVBy(dy * 5 * M_PI / 180.0);
	}
	
}

void
FBView::MessageReceived(BMessage *pmsg)
{
	switch (pmsg->what) {	
		case B_MOUSE_WHEEL_CHANGED:
			MouseWheelChanged(pmsg);
			break;
		default:
			BView::MessageReceived(pmsg);
			break;
	}
}
