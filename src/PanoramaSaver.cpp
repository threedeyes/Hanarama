#include <ControlLook.h>
#include <InterfaceKit.h>
#include <LayoutBuilder.h>
#include <StringView.h>
#include <TabView.h>
#include <TranslationUtils.h>
#include <TranslatorFormats.h>
#include <ScreenSaver.h>
#include <Slider.h>
#include <stdlib.h>
#include <View.h>
#include <Polygon.h>
#include <Bitmap.h>
#include <Screen.h>
#include <stdio.h>
#include <sys/time.h>

#include "ConfigView.h"

#include "Camera.h"
#include "Render.h"
#include "PanoramaSaver.h"

#include "FadeFilter.h"
#include "BlurFilter.h"
#include "NoiseFilter.h"
#include "SepiaFilter.h"
#include "FilmFilter.h"

int32			fFPSLimit;
int32			fCPULimit;
int32			fQuality;
int32			fNoiseLevel;
BPath			fFilename;

float 			fLastDelay = 0;

PRender 		*fRender = NULL;
BBitmap			*fSrcBitmap = NULL;
BBitmap			*fDstBitmap = NULL;


PCamera *fCam = NULL;

extern "C" _EXPORT BScreenSaver *instantiate_screen_saver(BMessage *message, image_id image)
{
	return new PanoramaSaver(message, image);
}

PanoramaSaver::PanoramaSaver(BMessage *message, image_id image)
	:
	BScreenSaver(message, image),
 	BHandler("HanaramaSaver")
{
	if (message) {
		if (message->FindInt32("FPS Limit", &fFPSLimit) != B_OK)
			fFPSLimit = 30;
		if (message->FindInt32("CPU Limit", &fCPULimit) != B_OK)
			fCPULimit = 1;
		if (message->FindInt32("Quality", &fQuality) != B_OK)
			fQuality = 100;
		if (message->FindInt32("Noise level", &fNoiseLevel) != B_OK)
			fNoiseLevel = 0;
		const char *filename;
		if (message->FindString("Filename", &filename) != B_OK)
			fFilename.Unset();
		else
			fFilename.SetTo(filename);
	}

	image_info the_info;
	status_t retval = get_image_info(image, &the_info);
    fSaverRes = new BResources;
    retval = fSaverRes->SetTo(new BFile( the_info.name, O_RDONLY));
}

PanoramaSaver::~PanoramaSaver()
{
	if (Looper()) {
		Looper()->RemoveHandler(this);
	}
}

void PanoramaSaver::StartConfig(BView *view)
{	
	ConfigView *fConfigView = new ConfigView(view->Bounds());
	view->AddChild(fConfigView);	
}


status_t
PanoramaSaver::SaveState(BMessage* into) const
{
	status_t status;
	if ((status = into->AddInt32("FPS Limit", fFPSLimit)) != B_OK)
		return status;
	if ((status = into->AddInt32("CPU Limit", fCPULimit)) != B_OK)
		return status;
	if ((status = into->AddInt32("Quality", fQuality)) != B_OK)
		return status;
	if ((status = into->AddInt32("Noise level", fNoiseLevel)) != B_OK)
		return status;
	return B_OK;
}


void
PanoramaSaver::MessageReceived(BMessage* message)
{
}

float getTime(void)
{
	return system_time()/1000000.0;
}

int32 renderer(void *data)
{
	int counter=0;
	char temp[16];
	float _starttime, _lasttime;

	FBView *view = (FBView*)data;	
  	fDstBitmap = view->GetBitmap();

	fRender = new PRender(fSrcBitmap, fDstBitmap, fCam);
	
	PFadeFilter fader(fDstBitmap);
	PBlurFilter blurer(fDstBitmap);
	PNoiseFilter noise(fDstBitmap);
	PSepiaFilter sepia(fDstBitmap);
	PFilmFilter film(fDstBitmap);
		
	fader.SetFade(255);

	int lastDispersion = fNoiseLevel;
  	noise.SetDispersion(fNoiseLevel);
  	sepia.SetDepth(fNoiseLevel);
  	
	fRender->InitMultiRenders(fCPULimit);
		
	float time;

	_starttime = _lasttime = getTime();

  	for(;;counter++) {
		fRender->Render();
		fader.Apply();
    	if(fNoiseLevel>0) {
    		if(fNoiseLevel != lastDispersion) {
    			//noise.SetDispersion(fNoiseLevel);
    			sepia.SetDepth(fNoiseLevel);
    			lastDispersion = fNoiseLevel;
    		}
    		sepia.Apply();
    		film.Apply();
    		//noise.Apply();
    	}
		view->Paint();

		time = getTime();
		if(time - _lasttime > 1.0) {
			float fps = 1.0/((time - _lasttime)/counter);
			_lasttime = time;
			int fps_int = (int)fps;
			if(fps_int<0)fps_int=0;

			float znooze_frame = ((1/(float)fFPSLimit) - ((1/fps) - (fLastDelay/1000000))) * 1000000;			

			if(znooze_frame > 0)
				fLastDelay = znooze_frame;

			sprintf(temp,"FPS: %d",(int)fps_int);
			view->SetOSD(temp);			
			counter = -1;
		}
		
		if(time - _starttime <= 3.0) {
			int fade = ((time - _starttime)*255) / 3.0;
			if(fade>255)
				fade=255;
  			fader.SetFade(255 - fade);
		}

		
		if(fLastDelay>0 && fLastDelay < 1000000)
			snooze((int)fLastDelay);
  	}
  	fRender->LeaveMultiRender();
  	return 0;
}

void PanoramaSaver::StopSaver(void)
{
	status_t ret = 0;
	fCam->stopThread();
	fRender->LeaveMultiRender();
	kill_thread(rendererThread);

	if(fSrcBitmap!=NULL) {
		delete fSrcBitmap;
		fSrcBitmap = NULL;
	}
}

status_t 
PanoramaSaver::StartSaver(BView *view, bool preview)
{	
//	fSrcBitmap = BTranslationUtils::GetBitmapFile("/boot/home/Project/Hanarama/samples/test.jpg");
//	if(fSrcBitmap == NULL)
//		return B_ERROR;
	size_t size = 0;	
	const void *data = fSaverRes->LoadResource('JPEG',"default.jpg" ,&size);
	BMemoryIO stream(data, size);
	stream.Seek(0, SEEK_SET);
	fSrcBitmap = BTranslationUtils::GetBitmap(&stream);

	fCam = new PCamera();
	fCam->SetMode(CAM_MODE_AUTO_PANNIG);
	
	if(preview) {
		frameBuffer = new FBView(view->Bounds(), view->Bounds().Width(), view->Bounds().Height(), preview);
	} else {
		frameBuffer = new FBView(view->Bounds(), view->Bounds().Width() * fQuality / 100.0 , view->Bounds().Height()* fQuality / 100.0, preview);
	}
	view->AddChild(frameBuffer);
	
	rendererThread = spawn_thread(renderer,"rendererThread", B_DISPLAY_PRIORITY, (void*)frameBuffer);
	resume_thread(rendererThread);		
	
	return B_OK;
}

void PanoramaSaver::Draw(BView *view, int32)
{		
}
