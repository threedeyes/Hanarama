/*
 * Copyright 2013, Gerasim Troeglazov (3dEyes**), 3dEyes@gmail.com.
 * All rights reserved.
 * Distributed under the terms of the MIT License.
 */
 
#include "TestApplication.h"
#include "TestWindow.h"

const char *APPSIGNATURE = "application/x-vnd.Hanarama-viewer";

TestApplication::TestApplication()
		  :BApplication(APPSIGNATURE)
{

}


TestApplication::~TestApplication()
{
}

void
TestApplication::RefsReceived(BMessage *pmsg)
{
	if (pmsg->HasMessenger("TrackerViewToken")) {
		pmsg->FindMessenger("TrackerViewToken", &fTrackerMessenger);
	}

	uint32 type;
	int32 count;
	status_t ret = pmsg->GetInfo("refs", &type, &count);
	if (ret != B_OK || type != B_REF_TYPE)
		return;

	entry_ref ref;
	for (int32 i = 0; i < count; i++) {
   		if (pmsg->FindRef("refs", i, &ref) == B_OK)
   			OpenFile(&ref);
   	}
}


void
TestApplication::ArgvReceived(int32 argc, char **argv)
{
	BMessage *pmsg = NULL;
	for (int32 i = 1; i < argc; i++) {
		entry_ref ref;
		status_t err = get_ref_for_path(argv[i], &ref);
		if (err == B_OK) {
			if (!pmsg) {
				pmsg = new BMessage;
				pmsg->what = B_REFS_RECEIVED;
			}
			pmsg->AddRef("refs", &ref);
		}
	}
	if (pmsg) {
		RefsReceived(pmsg);
		delete pmsg;
	}
}

void
TestApplication::OpenFile(const entry_ref *pref)
{
	BPath path;
	BEntry entry=BEntry(pref);
	entry.GetPath(&path);
	
	BRect windowRect(50,50,50+WIDTH,50+HEIGHT);
	testWindow = new TestWindow(windowRect, "Hanarama", path);
	testWindow->Show();	
}

int
main(int argc, char* argv[])
{
	TestApplication app;
	app.Run();
	
	return 0;
}
