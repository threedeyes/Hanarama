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

// message constants
enum {
	MSG_SET_FPS_LIMIT		= 'fpsl',
	MSG_SET_CPU_LIMIT		= 'cpul',
	MSG_SET_QUALITY			= 'qual',
	MSG_SET_NOISE_LEVEL		= 'nois',
};

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
	FBView			*frameBuffer;
	thread_id 		rendererThread;
	BResources		*fSaverRes;
};
