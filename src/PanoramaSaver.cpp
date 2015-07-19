#include <ControlLook.h>
#include <InterfaceKit.h>
#include <LayoutBuilder.h>
#include <StringView.h>
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

#include "Camera.h"
#include "Render.h"
#include "PanoramaSaver.h"

#include "FadeFilter.h"
#include "BlurFilter.h"
#include "NoiseFilter.h"

int32			fFPSLimit;
int32			fCPULimit;
int32			fQuality;
int32			fNoiseLevel;
BPath			fFilename;
PRender 		*fRender;

class SimpleSlider : public BSlider {
public:
	SimpleSlider(const char* label, int32 min, int32 max, BMessage* message)
		:
		BSlider(B_EMPTY_STRING, B_EMPTY_STRING, message, min, max, B_HORIZONTAL)
	{
		BString minLabel;
		minLabel << min;
		BString maxLabel;
		maxLabel << max;
		SetLimitLabels(minLabel, maxLabel);
		SetHashMarks(B_HASH_MARKS_BOTTOM);
		SetHashMarkCount(11);
		fLabel = label;
	};

	const char* UpdateText() const
	{
		fText.SetToFormat("%s: %d", fLabel, Value());
		return fText.String();
	};

private:
	mutable BString fText;
	const char* fLabel;
};

class SettingsView : public BView {
public:
								SettingsView(BRect frame);

	virtual	void				AttachedToWindow();
	virtual	void				MessageReceived(BMessage* message);

	private:
			BSlider*			fFPSSlider;
			BSlider*			fCPUSlider;
			BSlider*			fQualitySlider;
			BSlider*			fNoiseSlider;
};


SettingsView::SettingsView(BRect frame)
	:
	BView(frame, "", B_FOLLOW_ALL, B_WILL_DRAW)
{
	BStringView* titleString = new BStringView(B_EMPTY_STRING, "Hanarama 360");
	titleString->SetFont(be_bold_font);

	BStringView* copyrightString = new BStringView(B_EMPTY_STRING,
		"© 2013-2015 Gerasim Troeglazov");

	fFPSSlider = new SimpleSlider("FPS Limit", 1, 120, new BMessage(MSG_SET_FPS_LIMIT));
	fFPSSlider->SetValue(fFPSLimit);

	fCPUSlider = new SimpleSlider("Threads", 1, 32,	new BMessage(MSG_SET_CPU_LIMIT));
	fCPUSlider->SetValue(fCPULimit);

	fQualitySlider = new SimpleSlider("Quality", 10, 100, new BMessage(MSG_SET_QUALITY));
	fQualitySlider->SetValue(fQuality);

	fNoiseSlider = new SimpleSlider("Noise", 0, 100, new BMessage(MSG_SET_NOISE_LEVEL));
	fNoiseSlider->SetValue(fNoiseLevel);


	BLayoutBuilder::Group<>(this, B_VERTICAL, B_USE_HALF_ITEM_SPACING)
		.SetInsets(B_USE_HALF_ITEM_INSETS, B_USE_HALF_ITEM_INSETS,
			B_USE_BIG_INSETS, B_USE_HALF_ITEM_INSETS)
		.Add(titleString)
		.Add(copyrightString)
		.AddStrut(roundf(be_control_look->DefaultItemSpacing() / 2))
		.AddGlue()
		.Add(fFPSSlider)
		.AddGlue()
		.Add(fCPUSlider)
		.AddGlue()
		.Add(fQualitySlider)
		.AddGlue()
		.Add(fNoiseSlider)
		.AddGlue()
	.End();

	MoveBy(0, -25); // The view is not where it should be.
}


void
SettingsView::AttachedToWindow()
{
	SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	fFPSSlider->SetTarget(this);
	fCPUSlider->SetTarget(this);
	fQualitySlider->SetTarget(this);
	fNoiseSlider->SetTarget(this);
}


void
SettingsView::MessageReceived(BMessage* message)
{
	switch(message->what) {
		case MSG_SET_FPS_LIMIT:
			fFPSLimit = fFPSSlider->Value();
			break;

		case MSG_SET_CPU_LIMIT:
			fCPULimit = fCPUSlider->Value();
			break;

		case MSG_SET_QUALITY:
			fQuality = fQualitySlider->Value();
			break;
			
		case MSG_SET_NOISE_LEVEL:
			fNoiseLevel = fNoiseSlider->Value();
			break;			
	}
}



namespace BPrivate {
	void BuildScreenSaverDefaultSettingsView(BView* view, const char* moduleName, const char* info);
}

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
}

PanoramaSaver::~PanoramaSaver()
{
	if (Looper()) {
		Looper()->RemoveHandler(this);
	}
}

void PanoramaSaver::StartConfig(BView *view)
{
	view->AddChild(new SettingsView(view->Frame()));
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
	float delay = 0;
	int counter=0;
	char temp[16];
	float _starttime, _lasttime;

	FBView *view = (FBView*)data;
	
  	BBitmap *dstBmp = view->GetBitmap();
  	BBitmap *srcBmp;
	//if(view->fPreview)
		//srcBmp = BTranslationUtils::GetBitmap(B_JPEG_FORMAT, "default.jpg");
		 srcBmp = BTranslationUtils::GetBitmapFile("/HaikuData/Projects/Hanarama/samples/test.jpg");
	//else
		//srcBmp = BTranslationUtils::GetBitmapFile("/HaikuData/Projects/Hanarama/samples/test.jpg");

	fRender = new PRender(srcBmp, dstBmp, fCam);
	
	PFadeFilter fader(dstBmp);
	PBlurFilter blurer(dstBmp);
	PNoiseFilter noise(dstBmp);
		
	fader.SetFade(255);

	int lastDispersion = fNoiseLevel;
  	noise.SetDispersion(fNoiseLevel);
  	
	fRender->InitMultiRenders(fCPULimit);
		
	float time;

	_starttime = _lasttime = getTime();

  	for(;;counter++) {
		fRender->Render();
		fader.Apply();
    	if(fNoiseLevel>0) {
    		if(fNoiseLevel != lastDispersion) {
    			noise.SetDispersion(fNoiseLevel);
    			lastDispersion = fNoiseLevel;
    		}
    		noise.Apply();
    	}
		view->Paint();

		time = getTime();
		if(time - _lasttime > 1.0) {
			float fps = 1.0/((time - _lasttime)/counter);
			_lasttime = time;
			int fps_int = (int)fps;
			if(fps_int<0)fps_int=0;

			float znooze_frame = ((1/(float)fFPSLimit) - ((1/fps) - (delay/1000000))) * 1000000;			

			if(znooze_frame > 0)
				delay = znooze_frame;

			//sprintf(temp,"FPS: %d",(int)fps_int);
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
  	fRender->LeaveMultiRender();
  	return 0;
}

void PanoramaSaver::StopSaver(void)
{
	status_t ret = 0;
	fCam->stopThread();
	fRender->LeaveMultiRender();
	kill_thread(rendererThread);
}

status_t 
PanoramaSaver::StartSaver(BView *view, bool preview)
{	
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
