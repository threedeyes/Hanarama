/*
 * Copyright 2013, Gerasim Troeglazov (3dEyes**), 3dEyes@gmail.com.
 * All rights reserved.
 * Distributed under the terms of the MIT License.
 */

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

BPath			fFilename;

bool			fNoiseEnabled;
int32			fNoiseLevel;

bool			fFilmEnabled;
int32			fFilmLevel;

bool			fSepiaEnabled;
int32			fSepiaLevel;

bool			fFPSEnabled;

int32			fTabSelection;

float 			fLastDelay = 0;

PRender 		*fRender = NULL;
BBitmap			*fSrcBitmap = NULL;
BBitmap			*fPreviwBitmap = NULL;
BBitmap			*fDstBitmap = NULL;

PanoramaSaver	*This = NULL;
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
	fCam = NULL;
	This = this;
	
	if (message) {
		if (message->FindInt32("Last Tab Selection", &fTabSelection) != B_OK)
			fTabSelection = 0;
		if (message->FindInt32("FPS Limit", &fFPSLimit) != B_OK)
			fFPSLimit = 30;
		if (message->FindInt32("CPU Limit", &fCPULimit) != B_OK)
			fCPULimit = 1;
		if (message->FindInt32("Quality", &fQuality) != B_OK)
			fQuality = 100;
		if (message->FindInt32("Noise Level", &fNoiseLevel) != B_OK)
			fNoiseLevel = 0;
		if (message->FindInt32("Film Level", &fFilmLevel) != B_OK)
			fFilmLevel = 0;
		if (message->FindInt32("Sepia Level", &fSepiaLevel) != B_OK)
			fSepiaLevel = 0;
		if (message->FindBool("Noise Enabled", &fNoiseEnabled) != B_OK)
			fNoiseEnabled = false;
		if (message->FindBool("Film Enabled", &fFilmEnabled) != B_OK)
			fFilmEnabled = false;
		if (message->FindBool("Sepia Enabled", &fSepiaEnabled) != B_OK)
			fSepiaEnabled = false;
		if (message->FindBool("FPS Enabled", &fFPSEnabled) != B_OK)
			fFPSEnabled = false;				
		const char *filename;
		if (message->FindString("Filename", &filename) != B_OK)
			fFilename.Unset();
		else
			fFilename.SetTo(filename);
		
		const void *data;
		ssize_t size;
		if (message->FindData("CachedBitmap", '#BMP', &data, &size) == B_OK) {
			BRect	prevRect = BRect(0, 0, PREVIEW_WIDTH - 1, PREVIEW_HEIGHT - 1);	
			fPreviwBitmap = new BBitmap(prevRect, B_RGB32, true);
			memcpy(fPreviwBitmap->Bits(), (char*)data, size);
		} else
			fPreviwBitmap = NULL;
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
	if ((status = into->AddInt32("Last Tab Selection", fTabSelection)) != B_OK)
		return status;
	if ((status = into->AddInt32("FPS Limit", fFPSLimit)) != B_OK)
		return status;
	if ((status = into->AddInt32("CPU Limit", fCPULimit)) != B_OK)
		return status;
	if ((status = into->AddInt32("Quality", fQuality)) != B_OK)
		return status;
	if ((status = into->AddInt32("Noise Level", fNoiseLevel)) != B_OK)
		return status;
	if ((status = into->AddInt32("Film Level", fFilmLevel)) != B_OK)
		return status;
	if ((status = into->AddInt32("Sepia Level", fSepiaLevel)) != B_OK)
		return status;
	if ((status = into->AddBool("Noise Enabled", fNoiseEnabled)) != B_OK)
		return status;
	if ((status = into->AddBool("Film Enabled", fFilmEnabled)) != B_OK)
		return status;
	if ((status = into->AddBool("Sepia Enabled", fSepiaEnabled)) != B_OK)
		return status;
	if ((status = into->AddBool("FPS Enabled", fFPSEnabled)) != B_OK)
		return status;
	if(fFilename.InitCheck()==B_OK) {
		if ((status = into->AddString("Filename", fFilename.Path())) != B_OK)
			return status;
	}
	if(fPreviwBitmap!=NULL) {
		if ((status = into->AddData("CachedBitmap", '#BMP', fPreviwBitmap->Bits(), fPreviwBitmap->BitsLength())) != B_OK)
			return status;
	}
	
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
  	
	fRender->InitMultiRenders(fCPULimit);
		
	float time;

	_starttime = _lasttime = getTime();

  	for(;;counter++) {
		fRender->Render();

    	if(fNoiseEnabled && fNoiseLevel > 0) {
    			noise.SetDispersion(fNoiseLevel);
   				noise.Apply();
    	}
    	if(fFilmEnabled) {
    			film.SetLevel(fFilmLevel);
   				film.Apply();
    	}
    	if(fSepiaEnabled) {
    			sepia.SetDepth(fSepiaLevel);
   				sepia.Apply();
    	}

		if(!view->fPreview)
			fader.Apply();

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
			
			if(fFPSEnabled) {
				sprintf(temp,"FPS: %d",(int)fps_int);
				view->SetOSD(temp);
			} else {
				view->SetOSD("");
			}
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
	delete fRender;

	if(fPreviwBitmap!=NULL) {
		delete fPreviwBitmap;
		fPreviwBitmap = NULL;
	}

	fSaverView->RemoveChild(frameBuffer);
	delete frameBuffer;
}

status_t 
PanoramaSaver::StartSaver(void)
{
	StartSaver(fSaverView, fPreview);
}

status_t 
PanoramaSaver::StartSaver(BView *view, bool preview)
{
	fSaverView = view;
	fPreview = preview;

	if(fFilename.InitCheck()==B_OK) {
		if(preview && fPreviwBitmap != NULL) {
			fSrcBitmap = fPreviwBitmap;
		} else {
			fSrcBitmap = BTranslationUtils::GetBitmapFile(fFilename.Path());
			BRect rect(0, 0, PREVIEW_WIDTH - 1, PREVIEW_HEIGHT - 1);
			BBitmap *newPreviewBitmap = new BBitmap(rect, B_RGB32);
			ResizeBitmap(fSrcBitmap, newPreviewBitmap);
			fPreviwBitmap = newPreviewBitmap;
		}
	} else {
		size_t size = 0;
		const void *data = fSaverRes->LoadResource('JPEG',"default.jpg" ,&size);
		BMemoryIO stream(data, size);
		stream.Seek(0, SEEK_SET);
		fSrcBitmap = BTranslationUtils::GetBitmap(&stream);
	}

	if(fCam==NULL)
		fCam = new PCamera();
	else
		fCam->startThread();
	fCam->SetMode(CAM_MODE_AUTO_PANNIG);
	
	frameBuffer = new FBView(view->Bounds(),
		view->Bounds().Width() * fQuality / 100.0 , view->Bounds().Height()* fQuality / 100.0, preview);
	view->AddChild(frameBuffer);
	
	rendererThread = spawn_thread(renderer,"rendererThread", B_DISPLAY_PRIORITY, (void*)frameBuffer);
	resume_thread(rendererThread);		
	
	return B_OK;
}

void PanoramaSaver::Draw(BView *view, int32)
{		
}
