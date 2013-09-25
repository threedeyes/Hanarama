#include <stdio.h>

#include "TestApplication.h"
#include "TestWindow.h"

extern int32 renderer(void *data);
extern PCamera *fCam;

TestWindow::TestWindow(BRect frame, const char* title, BPath path)
	: BWindow(frame, title, B_TITLED_WINDOW_LOOK,B_NORMAL_WINDOW_FEEL,0)//B_NOT_RESIZABLE|B_NOT_ZOOMABLE)
{	

	fb = new FBView(Bounds(), WIDTH, HEIGHT, path);
	AddChild(fb); 	
 	 	
 	SetPulseRate(5000);
 	 	
 	renderer_thread = spawn_thread(renderer,"renderer_thread",B_NORMAL_PRIORITY,(void*)fb);
	resume_thread(renderer_thread);
}


TestWindow::~TestWindow()
{
	 kill_thread(renderer_thread);
}


void 
TestWindow::MessageReceived(BMessage *message)
{
	switch (message->what) {
		case B_UNMAPPED_KEY_DOWN:
		case B_KEY_DOWN:
		{
			const char *bytes;
			int32 key;
			message->FindInt32("key",&key);	
			//printf("%d\n",key);		
			if(key==18) {
				fCam->SetMode(CAM_MODE_MANUAL);
			}
			if(key==19) {
				fCam->SetMode(CAM_MODE_AUTO_PANNIG);
			}
			if(key==20) {
				fCam->SetMode(CAM_MODE_AUTO_RANDOM);
			}
			if(key==21) {
				fCam->SetMode(CAM_MODE_AUTO_ROTATE_LEFT);
			}
			if(key==22) {
				fCam->SetMode(CAM_MODE_AUTO_ROTATE_RIGHT);
			}			
			
			if(key==97) {
				fCam->MoveHeadingBy(5*M_PI/180.0);
			}
			if(key==99) {
				fCam->MoveHeadingBy(-5*M_PI/180.0);
			}
			
			if(key==87) {
				fCam->MovePitchBy(5*M_PI/180.0);
			}
			if(key==98) {
				fCam->MovePitchBy(-5*M_PI/180.0);
			}					
			
			if(key==37) {
				fCam->MoveFOVBy(5*M_PI/180.0);
			}
			if(key==58) {
				fCam->MoveFOVBy(-5*M_PI/180.0);
			}					

			break;
		}
		default:
			BWindow::MessageReceived(message);
			break;
	}
}

bool
TestWindow::QuitRequested()
{
	be_app->PostMessage(B_QUIT_REQUESTED);
	return true;
}
