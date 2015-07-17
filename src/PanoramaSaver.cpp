#include <StringView.h>
#include <TranslationUtils.h>
#include <ScreenSaver.h>
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

namespace BPrivate {
	void BuildScreenSaverDefaultSettingsView(BView* view, const char* moduleName, const char* info);
}

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
	BPrivate::BuildScreenSaverDefaultSettingsView(view, "Hanarama 360",
		"(c) 2013-2015 Gerasim Troeglazov");
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

float getTime(void)
{
	return system_time()/1000000.0;
}

bool DIE = false;

int32 renderer(void *data)
{
	float delay = 0;
	int counter=0;
	char temp[16];
	float _starttime, _lasttime;

	FBView *view = (FBView*)data;
	
  	BBitmap *dstBmp = view->GetBitmap();
  	BBitmap *srcBmp;
	if(view->fPreview)
		srcBmp = BTranslationUtils::GetBitmapFile("/boot/home/Project/Hanarama/samples/small.jpg");
	else
		srcBmp = BTranslationUtils::GetBitmapFile("/boot/home/Project/Hanarama/samples/test.jpg");

	PRender *render = new PRender(srcBmp, dstBmp, fCam);
	
	PFadeFilter fader(dstBmp);
	PBlurFilter blurer(dstBmp);
	PNoiseFilter noise(dstBmp);
		
	fader.SetFade(255);
  	noise.SetDispersion(50);
  	
	render->InitMultiRenders(2);
		
	float time;
	float fps_limit = 30.0;

	_starttime = _lasttime = getTime();

  	for(;!DIE;counter++) {
		if(!DIE)
			render->Render();
		if(!DIE)
			fader.Apply();
    	//noise.Apply();
		if(!DIE)
			view->Paint();

		time = getTime();
		if(time - _lasttime > 1.0) {
			float fps = 1.0/((time - _lasttime)/counter);
			_lasttime = time;
			int fps_int = (int)fps;
			if(fps_int<0)fps_int=0;

			float znooze_frame = ((1/fps_limit) - (1/fps)) * 1000000;		

			if(znooze_frame > 0)
				delay = znooze_frame;

			//sprintf(temp,"FPS: %d %d",(int)fps_int, (int)delay);
			//view->SetOSD(temp);			
			counter = -1;
		}
		
		if(time - _starttime <= 3.0) {
			int fade = ((time - _starttime)*255) / 3.0;
			if(fade>255)
				fade=255;
  			fader.SetFade(255 - fade);
		}

		
		if(delay>0 && delay < 1000000)
			snooze((int)delay);
  	}
  	render->LeaveMultiRender();  	
  	return 0;
}

void PanoramaSaver::StopSaver(void)
{
	status_t ret = 0;
	fCam->stopThread();
	DIE = true;
	wait_for_thread(rendererThread, &ret);
}

status_t 
PanoramaSaver::StartSaver(BView *view, bool preview)
{	
	DIE = false;
	fCam = new PCamera();
	fCam->SetMode(CAM_MODE_AUTO_PANNIG);
	
	if(preview) {
		frameBuffer = new FBView(view->Bounds(), view->Bounds().Width(), view->Bounds().Height(), preview);
	} else {
		frameBuffer = new FBView(view->Bounds(), (view->Bounds().Width() + 1) / 2, (view->Bounds().Height() + 1) / 2, preview);
	}
	view->AddChild(frameBuffer);
	
	rendererThread = spawn_thread(renderer,"rendererThread", B_DISPLAY_PRIORITY, (void*)frameBuffer);
	resume_thread(rendererThread);		
	
	return B_OK;
}

void PanoramaSaver::Draw(BView *view, int32)
{		
}
