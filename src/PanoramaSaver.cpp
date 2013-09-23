#include <StringView.h>
#include <TranslationUtils.h>
#include <Slider.h>
#include <stdlib.h>
#include <View.h>
#include <Polygon.h>
#include <Bitmap.h>
#include <Screen.h>
#include <stdio.h>
#include <time.h>

#include "Camera.h"
#include "Render.h"
#include "PanoramaSaver.h"

PCamera *fCam = NULL;

extern "C" _EXPORT BScreenSaver *instantiate_screen_saver(BMessage *message, image_id image)
{
	return new PanoramaSaver(message, image);
}

PanoramaSaver::PanoramaSaver(BMessage *message, image_id image)
 : BScreenSaver(message, image), BHandler("HanaramaSaver")
{
}

PanoramaSaver::~PanoramaSaver()
{
	if (Looper()) {
		Looper()->RemoveHandler(this);
	}
}

void PanoramaSaver::StartConfig(BView *view)
{
	BRect bounds = view->Bounds();
	bounds.InsetBy(10.0, 10.0);
	BRect frame(0.0, 0.0, bounds.Width(), bounds.Height());

	if (BWindow* window = view->Window())
		window->AddHandler(this);

	tView = new BStringView(frame, B_EMPTY_STRING, "Hanarama ScreenSaver");
	tView->SetFont(be_bold_font);
	tView->SetFontSize(15);
	view->AddChild(tView);
	tView->ResizeToPreferred();
	frame.top = tView->Frame().bottom+15;
	view->AddChild(new BStringView(frame, B_EMPTY_STRING, " Ver 0.1, (c) 2013 3dEyes**"));
	
}


status_t
PanoramaSaver::SaveState(BMessage* into) const
{
	return B_BAD_VALUE;
}


void
PanoramaSaver::MessageReceived(BMessage* message)
{
}


int32 renderer(void *data)
{
	FBView *view = (FBView*)data;	
	
  	BBitmap *dstBmp = view->GetBitmap();
	BBitmap *srcBmp = BTranslationUtils::GetBitmapFile("/Storage/pano/Samples/01.jpg");

	PRender *render = new PRender(srcBmp, dstBmp, fCam);
	render->InitMultiRenders(2);
	  	
  	for(;;) {    	
    	render->Render();    	
		view->Paint();
  	}
  	return 0;
}


status_t 
PanoramaSaver::StartSaver(BView *view, bool preview)
{	
	fCam = new PCamera();
	fCam->SetMode(CAM_MODE_AUTO_PANNIG);
	
	frameBuffer = new FBView(view->Bounds(), (view->Bounds().Width() + 1) / 2, (view->Bounds().Height() + 1) / 2, BPath());
	view->AddChild(frameBuffer);
	
	rendererThread = spawn_thread(renderer,"rendererThread", B_DISPLAY_PRIORITY, (void*)frameBuffer);
	resume_thread(rendererThread);		
	
	return B_OK;
}

void PanoramaSaver::Draw(BView *view, int32)
{		
}
