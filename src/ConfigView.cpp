/*
 * Copyright 2012-2013 Tri-Edge AI <triedgeai@gmail.com>
 * Copyright 2014 Haiku, Inc. All rights reserved.
 *
 * Distributed under the terms of the MIT license.
 *
 * Authors:
 *		Tri-Edge AI
 *		John Scipione, jscipione@gmail.com
 */


#include "ConfigView.h"

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
#include <TabView.h>
#include <ControlLook.h>

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


MainTabView::MainTabView(BRect rect, const char *name)
	:
	BGroupView(name, B_VERTICAL, 0)
{
	BStringView* titleString = new BStringView(B_EMPTY_STRING, "Hanarama 360");
	titleString->SetFont(be_bold_font);

	BButton* defaultButton = new BButton("default", "Default",
		new BMessage('defl'),
		B_WILL_DRAW | B_FRAME_EVENTS | B_NAVIGABLE);

	BButton* selectButton = new BButton("delect", "Select",
		new BMessage('open'),
		B_WILL_DRAW | B_FRAME_EVENTS | B_NAVIGABLE);

	BStringView* copyrightString = new BStringView(B_EMPTY_STRING,
		"© 2013-2015 Gerasim Troeglazov");
	BLayoutBuilder::Group<>(this, B_VERTICAL, 0)
		.SetInsets(B_USE_DEFAULT_SPACING)
		.Add(titleString)		
		.Add(copyrightString)
		.AddGlue()
		.AddGroup(B_HORIZONTAL, 0)
			.Add(defaultButton)
			.AddGlue()
			.Add(selectButton)
			.End()		
		.End();
}


void
MainTabView::AttachedToWindow()
{
	SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
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
			break;

		case MSG_SET_CPU_LIMIT:
			fCPULimit = fCPUSlider->Value();
			break;

		case MSG_SET_QUALITY:
			fQuality = fQualitySlider->Value();
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
			break;
		case MSG_SET_SEPIA_LEVEL:
			fSepiaEnabled = fSepiaCheckBox->Value() == B_CONTROL_ON;
			fSepiaLevel = fSepiaSlider->Value();
			break;
		case MSG_SET_FILM_LEVEL:
			fFilmEnabled = fFilmCheckBox->Value() == B_CONTROL_ON;
			fFilmLevel = fFilmSlider->Value();
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
