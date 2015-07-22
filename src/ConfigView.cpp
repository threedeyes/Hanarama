/*
 * Copyright 2015, Gerasim Troeglazov (3dEyes**), 3dEyes@gmail.com.
 * All rights reserved.
 * Distributed under the terms of the MIT License.
 */

#include "ConfigView.h"
#include "PanoramaSaver.h"

#include <LayoutBuilder.h>
#include <GroupLayoutBuilder.h>
#include <GridLayoutBuilder.h>
#include <ListView.h>
#include <ScrollView.h>
#include <CheckBox.h>
#include <Button.h>
#include <Slider.h>
#include <StringView.h>
#include <View.h>
#include <Path.h>
#include <Entry.h>
#include <TabView.h>
#include <ControlLook.h>
#include <FilePanel.h>

#include <stdio.h>

extern int32	fFPSLimit;
extern int32	fCPULimit;
extern int32	fQuality;
extern bool		fNoiseEnabled;
extern int32	fNoiseLevel;
extern bool		fFilmEnabled;
extern int32	fFilmLevel;
extern bool		fSepiaEnabled;
extern int32	fSepiaLevel;
extern bool		fFPSEnabled;
extern BPath	fFilename;
extern float	fLastDelay;
extern int32	fTabSelection;
extern BBitmap*	fPreviwBitmap;
extern PanoramaSaver* This;

MainTabView::MainTabView(BRect rect, const char *name)
	:
	BGroupView(name, B_VERTICAL, 0)
{
	fOpenPanel = new BFilePanel(B_OPEN_PANEL);	
	
	BStringView* titleString = new BStringView(B_EMPTY_STRING, "Hanarama 360");
	titleString->SetFont(be_bold_font);

	fDefaultButton = new BButton("default", "Default",
		new BMessage(MSG_BTN_DEFAULT),
		B_WILL_DRAW | B_FRAME_EVENTS | B_NAVIGABLE);

	fFickrButton = new BButton("flickr", "Get Panoramas",
		new BMessage(MSG_BTN_OPEN_FLICKR),
		B_WILL_DRAW | B_FRAME_EVENTS | B_NAVIGABLE);

	fSelectButton = new BButton("select", "Select",
		new BMessage(MSG_BTN_OPEN_FILE),
		B_WILL_DRAW | B_FRAME_EVENTS | B_NAVIGABLE);

	BStringView* copyrightString = new BStringView(B_EMPTY_STRING,
		"© 2013-2015 Gerasim Troeglazov");


	fImageView = new ImageView(BRect(0,0,0,0));
	
	BLayoutBuilder::Group<>(this, B_VERTICAL, 0)
		.SetInsets(B_USE_DEFAULT_SPACING)
		.Add(titleString)		
		.Add(copyrightString)
		.AddStrut(10)
		.Add(fImageView)
		.AddStrut(10)
		.AddGroup(B_HORIZONTAL, 0)
			.Add(fDefaultButton)
			.AddGlue()
			.Add(fFickrButton)
			.AddGlue()
			.Add(fSelectButton)
			.End()		
		.End();
}

MainTabView::~MainTabView()
{
	delete fOpenPanel;
}


void
MainTabView::AttachedToWindow()
{
	SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	fDefaultButton->SetTarget(this);
	fFickrButton->SetTarget(this);
	fSelectButton->SetTarget(this);
	fOpenPanel->SetTarget(this);
}

void
MainTabView::MessageReceived(BMessage* message)
{
	switch(message->what) {
		case B_REFS_RECEIVED:
		case B_SIMPLE_DATA:
		{
			entry_ref ref;
			if (message->FindRef("refs", &ref) == B_OK) {
				fFilename.SetTo(&ref);
			} else
				fFilename.Unset();
			break;
		}
		case MSG_BTN_DEFAULT:
			fFilename.Unset();
//			This->StopSaver();			
//			This->StartSaver();
//			fImageView->Invalidate();
			if(fPreviwBitmap!=NULL) {
				delete fPreviwBitmap;
				fPreviwBitmap = NULL;
			}
			fTabSelection = 0;
			break;

		case MSG_BTN_OPEN_FILE:
			fOpenPanel->Show();
			fTabSelection = 0;
			break;

		case MSG_BTN_OPEN_FLICKR:
			fTabSelection = 0;
			break;

		default:
			BView::MessageReceived(message);				
	}
}


PerformanceTabView::PerformanceTabView(BRect rect, const char *name)
	:
	BGroupView(name, B_VERTICAL, 0)
{
	fFPSSlider = new SimpleSlider("FPS Limit", 1, 120, new BMessage(MSG_SET_FPS_LIMIT));
	fFPSSlider->SetValue(fFPSLimit);

	fCPUSlider = new SimpleSlider("Threads", 1, 32,	new BMessage(MSG_SET_CPU_LIMIT));
	fCPUSlider->SetValue(fCPULimit);

	fQualitySlider = new SimpleSlider("Quality", 10, 100, new BMessage(MSG_SET_QUALITY));
	fQualitySlider->SetValue(fQuality);
	
	fFPSCheckBox = new BCheckBox("fpsEnabled", "Show FPS", new BMessage(MSG_SET_FPS_LIMIT));
	fFPSCheckBox->SetValue(fFPSEnabled);

	BLayoutBuilder::Group<>(this, B_VERTICAL, B_USE_HALF_ITEM_SPACING)
		.SetInsets(B_USE_HALF_ITEM_INSETS, B_USE_HALF_ITEM_INSETS,
			B_USE_BIG_INSETS, B_USE_HALF_ITEM_INSETS)
		.AddStrut(roundf(be_control_look->DefaultItemSpacing() / 2))
		.Add(fFPSSlider)
		.Add(fCPUSlider)
		.Add(fQualitySlider)
		.Add(fFPSCheckBox)
		.AddGlue();
}


void
PerformanceTabView::AttachedToWindow()
{
	SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	fFPSSlider->SetTarget(this);
	fCPUSlider->SetTarget(this);
	fQualitySlider->SetTarget(this);
	fFPSCheckBox->SetTarget(this);
}


void
PerformanceTabView::MessageReceived(BMessage* message)
{
	switch(message->what) {
		case MSG_SET_FPS_LIMIT:
			fFPSLimit = fFPSSlider->Value();
			fFPSEnabled = fFPSCheckBox->Value() == B_CONTROL_ON;
			fLastDelay = 0;
			fTabSelection = 1;
			break;

		case MSG_SET_CPU_LIMIT:
			fCPULimit = fCPUSlider->Value();
			fTabSelection = 1;
			break;

		case MSG_SET_QUALITY:
			fQuality = fQualitySlider->Value();
			fTabSelection = 1;
			break;

		default:
			BView::MessageReceived(message);				
	}
}


FXTabView::FXTabView(BRect rect, const char *name)
	:
	BGroupView(name, B_VERTICAL, 0)
{	
	fNoiseCheckBox = new BCheckBox("noiseEnabled", "Noise", new BMessage(MSG_SET_NOISE_LEVEL));
	fNoiseCheckBox->SetValue(fNoiseEnabled);
		
	fNoiseSlider = new SimpleSlider("Value", 0, 100, new BMessage(MSG_SET_NOISE_LEVEL));
	fNoiseSlider->SetValue(fNoiseLevel);

	fSepiaCheckBox = new BCheckBox("sepiaEnabled", "Sepia", new BMessage(MSG_SET_SEPIA_LEVEL));
	fSepiaCheckBox->SetValue(fSepiaEnabled);
	
	fSepiaSlider = new SimpleSlider("Value", 0, 100, new BMessage(MSG_SET_SEPIA_LEVEL));
	fSepiaSlider->SetValue(fSepiaLevel);	

	fFilmCheckBox = new BCheckBox("filmEnabled", "Film effect", new BMessage(MSG_SET_FILM_LEVEL));
	fFilmCheckBox->SetValue(fFilmEnabled);

	fFilmSlider = new SimpleSlider("Value", 0, 100, new BMessage(MSG_SET_FILM_LEVEL));
	fFilmSlider->SetValue(fFilmLevel);

	BLayoutBuilder::Group<>(this, B_VERTICAL, B_USE_HALF_ITEM_SPACING)
		.SetInsets(B_USE_HALF_ITEM_INSETS, B_USE_HALF_ITEM_INSETS,
			B_USE_BIG_INSETS, B_USE_HALF_ITEM_INSETS)
		.AddStrut(roundf(be_control_look->DefaultItemSpacing() / 2))
		
		.Add(BGridLayoutBuilder(B_USE_DEFAULT_SPACING, B_USE_HALF_ITEM_SPACING)
			.Add(fSepiaCheckBox, 0, 0)
			.Add(fSepiaSlider, 1, 0)	
			.Add(fFilmCheckBox, 0, 1)
			.Add(fFilmSlider, 1, 1)	
			.Add(fNoiseCheckBox, 0, 2)
			.Add(fNoiseSlider, 1, 2)
		)
		.AddGlue();
}


void
FXTabView::AttachedToWindow()
{
	SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	fNoiseSlider->SetTarget(this);
	fNoiseCheckBox->SetTarget(this);
	fSepiaSlider->SetTarget(this);
	fSepiaCheckBox->SetTarget(this);
	fFilmSlider->SetTarget(this);
	fFilmCheckBox->SetTarget(this);
}


void
FXTabView::MessageReceived(BMessage* message)
{
	switch(message->what) {
		case MSG_SET_NOISE_LEVEL:
			fNoiseEnabled = fNoiseCheckBox->Value() == B_CONTROL_ON;
			fNoiseLevel = fNoiseSlider->Value();
			fTabSelection = 2;
			break;
		case MSG_SET_SEPIA_LEVEL:
			fSepiaEnabled = fSepiaCheckBox->Value() == B_CONTROL_ON;
			fSepiaLevel = fSepiaSlider->Value();
			fTabSelection = 2;
			break;
		case MSG_SET_FILM_LEVEL:
			fFilmEnabled = fFilmCheckBox->Value() == B_CONTROL_ON;
			fFilmLevel = fFilmSlider->Value();
			fTabSelection = 2;
			break;
		default:
			BView::MessageReceived(message);		
	}
}

ConfigView::ConfigView(BRect frame)
	:
	BView(frame, B_EMPTY_STRING, B_FOLLOW_ALL_SIDES, B_WILL_DRAW)
{
	SetLayout(new BGroupLayout(B_HORIZONTAL));
	SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));

	BTabView* tabView = new BTabView(B_EMPTY_STRING, B_WIDTH_FROM_LABEL);

	MainTabView *mainTabView = new MainTabView(Bounds(), "Main");
	tabView->AddTab(mainTabView);

	PerformanceTabView *perTabfView = new PerformanceTabView(Bounds(), "Performance");
	tabView->AddTab(perTabfView);

	FXTabView *fxTabfView = new FXTabView(Bounds(), "FX");
	tabView->AddTab(fxTabfView);

	tabView->Select(fTabSelection);

	AddChild(BLayoutBuilder::Group<>(B_VERTICAL, B_USE_DEFAULT_SPACING)
		.Add(tabView)
		.SetInsets(B_USE_DEFAULT_SPACING));
}


void
ConfigView::AttachedToWindow()
{
}


void
ConfigView::MessageReceived(BMessage* message)
{
	switch (message->what) {
		default:
			BView::MessageReceived(message);
	}
}
