#ifndef _TEST_APPLICATION_H
#define _TEST_APPLICATION_H

#include <Application.h>
#include <FilePanel.h>
#include <Path.h>
#include <stdio.h>

#include "TestWindow.h"

#define WIDTH	800.0
#define HEIGHT	480.0

class TClockWindow;


class TestApplication : public BApplication {
	public:
						TestApplication();
		virtual			~TestApplication();

	private:
		void	ArgvReceived(int32 argc, char **argv);
		void	RefsReceived(BMessage *pmsg);	
		void	OpenFile(const entry_ref *pref);	

		TestWindow		*testWindow;
		BMessenger  fTrackerMessenger;
		BFilePanel  *fOpenPanel;
		
		
};

extern const char *APPSIGNATURE;

#endif

