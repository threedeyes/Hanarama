#include <StringView.h>
#include <TranslationUtils.h>
#include <Slider.h>
#include <stdlib.h>
#include <View.h>
#include <Polygon.h>
#include <Bitmap.h>
#include <Screen.h>
#include <stdio.h>
#include <sys/time.h>

#include "Camera.h"
#include "Render.h"
#include "PanoramaSaver.h"

#include "FadeFilter.h"
#include "BlurFilter.h"
#include "NoiseFilter.h"

PCamera *fCam = NULL;
bool DIE = false;

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
  	BBitmap *srcBmp;
  	//if(view->Bounds().Width() < 256)
		srcBmp = BTranslationUtils::GetBitmapFile("/HaikuData/Projects/Hanarama/samples/small.jpg");
	//else
	//	srcBmp = BTranslationUtils::GetBitmapFile("/HaikuData/Projects/Hanarama/samples/test.jpg");

	PRender *render = new PRender(srcBmp, dstBmp, fCam);
	
	PFadeFilter fader(dstBmp);
	PBlurFilter blurer(dstBmp);
	PNoiseFilter noise(dstBmp);
		
	fader.SetFade(255);
  	noise.SetDispersion(50);
  	
	render->InitMultiRenders(1);
		
	bigtime_t start = real_time_clock_usecs();
	
  	for(;;) {
  		if(DIE)
  			break;
		//bigtime_t now = real_time_clock_usecs();
  		//bigtime_t cont = (now - start) / 1000;
  		//if(cont>3000)cont=3000;
  		//fader.SetFade(255 - (255 * cont)/3000);
  		
    	render->Render();
    	//fader.Apply();
		view->Paint();
  	}
  	render->LeaveMultiRender();  	
  	return 0;
}

void PanoramaSaver::StopSaver(void)
{
	status_t ret = 0;
	DIE = true;
	fCam->stopThread();
	wait_for_thread(rendererThread, &ret);
}

status_t 
PanoramaSaver::StartSaver(BView *view, bool preview)
{	
	DIE = false;
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
