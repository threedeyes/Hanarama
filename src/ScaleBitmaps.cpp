#include <Application.h>
#include <Alert.h>
#include <Roster.h>
#include <Entry.h>
#include <storage/Path.h>
#include <Node.h>
#include <View.h>
#include <Directory.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <DataIO.h>
#include <BitmapStream.h>
#include <TranslationUtils.h>
#include <TranslatorRoster.h>
#include <Bitmap.h>
#include <strings.h>

#include "ScaleBitmaps.h"

BBitmap *ConvertBitmapToRGBA(BBitmap *src)
{
	BBitmap*	tmp = new BBitmap(src->Bounds(), B_RGBA32, true);
	BBitmap*	final = new BBitmap(src->Bounds(), B_RGBA32);
	BView*		view = new BView(src->Bounds(), "", B_FOLLOW_ALL, B_WILL_DRAW);

	tmp->AddChild(view);
	tmp->Lock();

	view->DrawBitmap(src, BPoint(0,0));

	tmp->Unlock();
	memcpy(final->Bits(), tmp->Bits(), tmp->BitsLength());	
	delete tmp;

	return final;
}

void ScaleBitmap(BBitmap *Src,BBitmap *Dst)
{
	rgb_color empty_color={0,0,0,0};
	
	uchar	*src=(uchar*)Src->Bits();
	uchar	*bits=(uchar*)Dst->Bits();
	
	float src_w=Src->Bounds().Width()+1;
	float src_h=Src->Bounds().Height()+1;
	float dst_w=Dst->Bounds().Width()+1;
	float dst_h=Dst->Bounds().Height()+1;
	float eps=1e-7;
	
	float kx=src_w/dst_w;
	float ky=src_h/dst_h;
	
	for(float y=0;y<dst_h;y++)
		for(float x=0;x<dst_w;x++)
		{
			float x1,y1;
			float x2=kx+(x1=kx*x), 	y2=ky+(y1=ky*y);
			float x1i=floor(x1), 	y1i=floor(y1), 		x2i=floor(x2), 		y2i=floor(y2);
			float x1d=1-x1+x1i, 	y1d=1-y1+y1i,		x2d=x2-x2i,			y2d=y2-y2i;
			float xn=(x1d<=eps)?x1i:x1i+1,				yn=(y1d<=eps)?y1i:y1i+1;

			float	sum_r=0,sum_g=0,sum_b=0,sum_a=0;
			float	count=0;
			float xl,yl;
			for(xl=xn; xl<x2i; xl++)
				for(float yl=yn; yl<y2i; yl++)
				{
					rgb_color	col=SColor32(xl,yl,src_w,src_h);
					sum_r+=col.red;	sum_g+=col.green; sum_b+=col.blue; sum_a+=col.alpha;
					count++;
				}
				
			for(xl=xn;xl<x2i;xl++)
				{
					rgb_color	col1=SColor32(xl,y1i,src_w,src_h);
					rgb_color	col2=SColor32(xl,y2i,src_w,src_h);

					sum_r+=(y1d>eps)?(col1.red*y1d):0;
					sum_g+=(y1d>eps)?(col1.green*y1d):0;
					sum_b+=(y1d>eps)?(col1.blue*y1d):0;
					sum_a+=(y1d>eps)?(col1.alpha*y1d):0;

					sum_r+=(y2d>eps)?(col2.red*y2d):0;
					sum_g+=(y2d>eps)?(col2.green*y2d):0;
					sum_b+=(y2d>eps)?(col2.blue*y2d):0;
					sum_a+=(y2d>eps)?(col2.alpha*y2d):0;
					
					count+=y1d+y2d;;					
				}
			for(yl=yn;yl<y2i;yl++)
				{
					rgb_color	col1=SColor32(x1i,yl,src_w,src_h);
					rgb_color	col2=SColor32(x2i,yl,src_w,src_h);

					sum_r+=(x1d>eps)?(col1.red*x1d):0;
					sum_g+=(x1d>eps)?(col1.green*x1d):0;
					sum_b+=(x1d>eps)?(col1.blue*x1d):0;
					sum_a+=(x1d>eps)?(col1.alpha*x1d):0;

					sum_r+=(x2d>eps)?(col2.red*x2d):0;
					sum_g+=(x2d>eps)?(col2.green*x2d):0;
					sum_b+=(x2d>eps)?(col2.blue*x2d):0;
					sum_a+=(x2d>eps)?(col2.alpha*x2d):0;
					
					count+=x1d+x2d;;					
				}
			 
			rgb_color	col1=SColor32(x1i,y1i,src_w,src_h);
			rgb_color	col2=SColor32(x1i,y2i,src_w,src_h);
			rgb_color	col3=SColor32(x2i,y1i,src_w,src_h);
			rgb_color	col4=SColor32(x2i,y2i,src_w,src_h);
				
			sum_r+=((x1d>eps&&y1d>eps)?(col1.red*x1d*y1d):0)
				   +((x1d>eps&&y2d>eps)?(col2.red*x1d*y2d):0)
				   +((x2d>eps&&y1d>eps)?(col3.red*x2d*y1d):0)
				   +((x2d>eps&&y2d>eps)?(col4.red*x2d*y2d):0);
			sum_g+=((x1d>eps&&y1d>eps)?(col1.green*x1d*y1d):0)
				   +((x1d>eps&&y2d>eps)?(col2.green*x1d*y2d):0)
				   +((x2d>eps&&y1d>eps)?(col3.green*x2d*y1d):0)
				   +((x2d>eps&&y2d>eps)?(col4.green*x2d*y2d):0);
			sum_b+=((x1d>eps&&y1d>eps)?(col1.blue*x1d*y1d):0)
				   +((x1d>eps&&y2d>eps)?(col2.blue*x1d*y2d):0)
				   +((x2d>eps&&y1d>eps)?(col3.blue*x2d*y1d):0)
				   +((x2d>eps&&y2d>eps)?(col4.blue*x2d*y2d):0);
			sum_a+=((x1d>eps&&y1d>eps)?(col1.alpha*x1d*y1d):0)
				   +((x1d>eps&&y2d>eps)?(col2.alpha*x1d*y2d):0)
				   +((x2d>eps&&y1d>eps)?(col3.alpha*x2d*y1d):0)
				   +((x2d>eps&&y2d>eps)?(col4.alpha*x2d*y2d):0);
			count+=(x1d+x2d)*(y1d+y2d);			
			
			*bits++ = (uchar)(0.5 + sum_r/count);
			*bits++ = (uchar)(0.5 + sum_g/count);
			*bits++ = (uchar)(0.5 + sum_b/count);
			*bits++ = (uchar)(0.5 + sum_a/count);
		}	
}
