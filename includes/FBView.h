#ifndef _H_FBVIEW_
#define _H_FBVIEW_

#include <SupportDefs.h>
#include <Bitmap.h>
#include <Cursor.h>
#include <View.h>
#include <Rect.h>
#include <Path.h>

class FBView : public BView 
{
 public:
		FBView(BRect rect, BPath path);
		FBView(BRect rect, int width, int height, BPath path);
		~FBView();			
		
		void 	Paint();
		BBitmap	*GetBitmap();
		uint32	*GetBuffer();
		uint32	GetBufferSize();
		
		int		Width();
		int		Height();
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
		BPath 	filename;
};

#endif
