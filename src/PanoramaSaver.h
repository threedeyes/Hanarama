/*
 * Copyright 2013, Gerasim Troeglazov (3dEyes**), 3dEyes@gmail.com.
 * All rights reserved.
 * Distributed under the terms of the MIT License.
 */
 
#include <ScreenSaver.h>
#include <StringView.h>
#include <String.h>
#include <Slider.h>
#include <TextView.h>
#include <Font.h>
#include <CheckBox.h>
#include <TranslationUtils.h>
#include <Resources.h>

#include "FBView.h"
#include "BitmapUtils.h"

#define PREVIEW_WIDTH 800
#define PREVIEW_HEIGHT (PREVIEW_WIDTH / 2)


class PanoramaSaver : public BScreenSaver, public BHandler 
{
public:
						PanoramaSaver(BMessage *message, image_id id);
	virtual				~PanoramaSaver();				
	virtual void 		StartConfig(BView *view);
	status_t 			StartSaver(BView *v, bool preview);
	virtual	void 		StopSaver();
	status_t			StartSaver();
	void				ReloadImages(void);
	virtual void		Draw(BView *v, int32 frame);
	virtual	status_t	SaveState(BMessage* into) const;
	virtual	void		MessageReceived(BMessage* message);
private:
	FBView			*frameBuffer;
	thread_id 		rendererThread;
	BResources		*fSaverRes;
	bool			fPreview;
	BView 			*fSaverView;
};
