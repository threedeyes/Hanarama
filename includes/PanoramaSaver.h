#include <ScreenSaver.h>
#include <StringView.h>
#include <String.h>
#include <Slider.h>
#include <Font.h>
#include <CheckBox.h>

#include "FBView.h"

class PanoramaSaver : public BScreenSaver, public BHandler 
{
public:
						PanoramaSaver(BMessage *message, image_id id);
	virtual				~PanoramaSaver();				
	virtual void 		StartConfig(BView *view);
	status_t 			StartSaver(BView *v, bool preview);
	virtual	void 		StopSaver();
	virtual void		Draw(BView *v, int32 frame);
	virtual	status_t	SaveState(BMessage* into) const;
	virtual	void		MessageReceived(BMessage* message);
private:
	BStringView		*tView;
	FBView			*frameBuffer;
	thread_id 		rendererThread;
};
