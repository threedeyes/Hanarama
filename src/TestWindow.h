/*
 * Copyright 2013, Gerasim Troeglazov (3dEyes**), 3dEyes@gmail.com.
 * All rights reserved.
 * Distributed under the terms of the MIT License.
 */

#ifndef _TEST_WINDOW_H
#define _TEST_WINDOW_H

#include <Window.h>
#include <View.h>
#include <OS.h>
#include <TranslationUtils.h>

#include "TestApplication.h"
#include "TestWindow.h"
#include "FBView.h"
#include "Camera.h"

class TestWindow : public BWindow {
	public:
						TestWindow(BRect rect, const char* name, BPath path);
		virtual			~TestWindow();

		virtual void 	MessageReceived(BMessage *message);
		bool			QuitRequested();
		

	private:		
		FBView			*fb;
		thread_id 		renderer_thread;
};

#endif


