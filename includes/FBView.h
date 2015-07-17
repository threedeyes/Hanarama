#ifndef _H_FBVIEW_
#define _H_FBVIEW_

#include <SupportDefs.h>
#include <Bitmap.h>
#include <Cursor.h>
#include <String.h>
#include <View.h>
#include <Rect.h>
#include <Path.h>

class FBView : public BView 
{
 public:
		FBView(BRect rect, bool preview);
		FBView(BRect rect, int width, int height, bool preview);
		~FBView();			
		
		void 	Paint();
		BBitmap	*GetBitmap();
		uint32	*GetBuffer();
		uint32	GetBufferSize();
		
		int		Width();
		int		Height();

		void	SetOSD(const char *str);

 private:
 		int		buffer_width;
 		int		buffer_height;
		BView	*bufferView;
		BBitmap *bufferBitmap; 		

		void MouseWheelChanged(BMessage *msg);
		
		virtual void MessageReceived(BMessage *pmsg);
		virtual void MouseDown(BPoint p);
		virtual void MouseUp(BPoint p);
 		virtual void MouseMoved(BPoint point, uint32 transit, const BMessage *message);

		//Event state
		bool	mouseIsDown;
		float	mouseDownPosLastX;
		float	mouseDownPosLastY;
		float 	cam_fov_speed;		
		float 	time;
		
		BCursor *fMoveCursor;
public:		
		bool fPreview;
		BString	fOSDText;
};

#endif
