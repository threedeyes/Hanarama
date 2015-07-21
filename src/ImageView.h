#ifndef _H_IMAGE_VIEW_
#define _H_IMAGE_VIEW_

#include <SupportDefs.h>
#include <Bitmap.h>
#include <Cursor.h>
#include <String.h>
#include <View.h>
#include <Rect.h>
#include <Path.h>

class ImageView : public BView 
{
public:
		ImageView(BRect rect);
		~ImageView();
private:
		virtual void Draw(BRect rect);
		virtual void FrameResized(float width, float height);
};

#endif
