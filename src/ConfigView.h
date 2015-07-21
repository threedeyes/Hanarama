/*
 * Copyright 2013, Gerasim Troeglazov (3dEyes**), 3dEyes@gmail.com.
 * All rights reserved.
 * Distributed under the terms of the MIT License.
 */
 
#ifndef _H_SAVER_CONFIG_VIEW_
#define _H_SAVER_CONFIG_VIEW_


#include <View.h>
#include <GroupView.h>
#include <Slider.h>
#include <String.h>

class BListView;
class BScrollView;
class BStringView;

// message constants
enum {
	MSG_SET_FPS_LIMIT		= 'fpsl',
	MSG_SET_CPU_LIMIT		= 'cpul',
	MSG_SET_QUALITY			= 'qual',
	MSG_SET_NOISE_LEVEL		= 'nois',
};


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



class MainTabView : public BGroupView {
public:
								MainTabView(BRect rect, const char *name);
	virtual	void				AttachedToWindow();
};


class PerformanceTabView : public BGroupView {
public:
								PerformanceTabView(BRect rect, const char *name);

	virtual	void				AttachedToWindow();
	virtual	void				MessageReceived(BMessage* message);

	private:
			BSlider*			fFPSSlider;
			BSlider*			fCPUSlider;
			BSlider*			fQualitySlider;
			BSlider*			fNoiseSlider;
};

class FXTabView : public BGroupView {
public:
								FXTabView(BRect rect, const char *name);

	virtual	void				AttachedToWindow();
	virtual	void				MessageReceived(BMessage* message);

	private:
			BSlider*			fNoiseSlider;
};

class ConfigView : public BView {
public:
								ConfigView(BRect frame);

			void	 			AttachedToWindow();
			void	 			MessageReceived(BMessage* message);
};

#endif	// _H_SAVER_CONFIG_VIEW_