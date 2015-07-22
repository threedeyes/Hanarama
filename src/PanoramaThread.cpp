/*
 * Copyright 2013, Gerasim Troeglazov (3dEyes**), 3dEyes@gmail.com.
 * All rights reserved.
 * Distributed under the terms of the MIT License.
 */

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <String.h>

#include "TestApplication.h"
#include "TestWindow.h"
#include "FBView.h"
#include "Camera.h"
#include "FastMath.h"
#include "Render.h"
#include "FadeFilter.h"
#include "BlurFilter.h"
#include "NoiseFilter.h"

PCamera *fCam = NULL;
BBitmap *srcBmp = NULL;

float _starttime, _lasttime;

float getTime(void)
{ 
	return system_time()/1000000.0;
}

int32 renderer(void *data)
{
	char temp[16];
	int counter=0;
		
	FBView *view=(FBView*)data;
  	BBitmap *dstBmp = view->GetBitmap();

	fCam = new PCamera();
	fCam->SetMode(CAM_MODE_MANUAL);	

	PRender *render = new PRender(srcBmp, dstBmp, fCam);
	render->InitMultiRenders(1);
	
	PFadeFilter fader(dstBmp);
	PBlurFilter blurer(dstBmp);
	PNoiseFilter noise(dstBmp);
	
  	_starttime = _lasttime = getTime();
  	
  	float time;
  	
  	fader.SetFade(80);
  	noise.SetDispersion(20);
  	
  	for(;; counter++) {    	
    	render->Render();
    	//fader.Apply();
    	//blurer.Apply();
    	//noise.Apply();

		view->Paint();

  		time = getTime();
		if(time - _lasttime > 1.0) {
			float fps = 1.0/((time - _lasttime)/counter);
			_lasttime = time;
			int fps_int = (int)fps;
			if(fps_int<0)fps_int=0;
			const char *mode[]={"Manual","Auto panning","Random","Left scrolling","Right scrolling"};
			sprintf(temp,"FPS:%d  %s",(int)fps_int, mode[fCam->Mode()]);
			view->Window()->SetTitle(temp);
			counter = -1;
		}		    	
  	}
  	return 0;
}
