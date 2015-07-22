#include <string.h>

#include "BitmapUtils.h"

void 	ResizeBitmap(BBitmap *inBitmap, BBitmap *outBitmap)
{
	if ( inBitmap->Bounds().Width() < outBitmap->Bounds().Width() || inBitmap->Bounds().Height() < outBitmap->Bounds().Height() )
		IncreaseBitmap(inBitmap,outBitmap);
	else
		DecreaseBitmap(inBitmap,outBitmap);
			
}

void	IncreaseBitmap(BBitmap *Src, BBitmap *Dst)
{
	rgb_color empty_color={0,0,0,0};
	if(Src->Bounds()==Dst->Bounds() && Src->ColorSpace() == Dst->ColorSpace())
	 {
	  memcpy(Src->Bits(),Dst->Bits(),Src->BitsLength());
	  return;
	 }
	if(Src->ColorSpace()==B_COLOR_8_BIT)
	 {
	   BScreen screen( B_MAIN_SCREEN_ID );
 	   const color_map *map = screen.ColorMap();
	   if( map == NULL )return;

	   uchar *src=(uchar*)Src->Bits();
	   uchar *bits=(uchar*)Dst->Bits();

	   float src_size_h=Src->Bounds().bottom+1;
	   float src_size_w=Src->Bounds().right+1;  
	   
	   float dst_size_h=Dst->Bounds().bottom+1;
	   float dst_size_w=Dst->Bounds().right+1;
	   
	   float kx=(Src->Bounds().right+1)/dst_size_w;
	   float ky=(Src->Bounds().bottom+1)/dst_size_h;
		for(float y=0;y<dst_size_h;y++)
		 for(float x=0;x<dst_size_w;x++)
		 	{
	 			float x1=x*kx;
	 			float y1=y*ky;
		 		float xi=floor(x1);
		 		float yi=floor(y1);
	 			float xd=x1-xi;
	 			float yd=y1-yi;
		 		rgb_color c;
		 		c.red=(uchar) (0.5 + (((float)(SColor(xi,yi,src_size_w,src_size_h).red) * (1-xd) * (1-yd))
	 							   + ((float)(SColor(xi+1,yi+1,src_size_w,src_size_h).red) * xd * yd)
	 							   + ((float)(SColor(xi+1,yi,src_size_w,src_size_h).red) * xd * (1-yd)) 
		 						   + ((float)(SColor(xi,yi+1,src_size_w,src_size_h).red) * (1-xd) * yd)));
		 		c.green=(uchar) (0.5 + (((float)(SColor(xi,yi,src_size_w,src_size_h).green) * (1-xd) * (1-yd))
	 							   + ((float)(SColor(xi+1,yi+1,src_size_w,src_size_h).green) * xd * yd)
	 							   + ((float)(SColor(xi+1,yi,src_size_w,src_size_h).green) * xd * (1-yd)) 
		 						   + ((float)(SColor(xi,yi+1,src_size_w,src_size_h).green) * (1-xd) * yd)));
		 		c.blue=(uchar) (0.5 + (((float)(SColor(xi,yi,src_size_w,src_size_h).blue) * (1-xd) * (1-yd))
	 							   + ((float)(SColor(xi+1,yi+1,src_size_w,src_size_h).blue) * xd * yd)
	 							   + ((float)(SColor(xi+1,yi,src_size_w,src_size_h).blue) * xd * (1-yd)) 
	 							   + ((float)(SColor(xi,yi+1,src_size_w,src_size_h).blue) * (1-xd) * yd)));
		 						  
		 		c.alpha = (uchar) (0.5 + (((SAlpha(xi,yi,src_size_w,src_size_h)) * (1-xd) * (1-yd))
	 							   + ((SAlpha(xi+1,yi+1,src_size_w,src_size_h)) * xd * yd)
	 							   + ((SAlpha(xi+1,yi,src_size_w,src_size_h)) * xd * (1-yd)) 
	 							   + ((SAlpha(xi,yi+1,src_size_w,src_size_h)) * (1-xd) * yd)));
				*bits++ = c.blue;
				*bits++ = c.green;
				*bits++ = c.red;
				*bits++ = c.alpha;
	 		}
	 	return;
	 }
	if(Src->ColorSpace()==B_RGBA32 || Src->ColorSpace()==B_RGB32)
	 {
	   uchar *src=(uchar*)Src->Bits();
	   uchar *bits=(uchar*)Dst->Bits();

	   float src_size_h=Src->Bounds().Height()+1.0;
	   float src_size_w=Src->Bounds().Width()+1.0;

	   float dst_size_h=Dst->Bounds().Height()+1.0;
	   float dst_size_w=Dst->Bounds().Width()+1.0;
	   
	   float kx=(Src->Bounds().right+1)/dst_size_w;
	   float ky=(Src->Bounds().bottom+1)/dst_size_h;
		for(float y=0;y<dst_size_h;y++)
		 for(float x=0;x<dst_size_w;x++)
		 	{
	 			float x1=x*kx;
	 			float y1=y*ky;
		 		float xi=floor(x1);
		 		float yi=floor(y1);
	 			float xd=x1-xi;
	 			float yd=y1-yi;
		 		rgb_color c;
		 		c.red=(uchar) (0.5 + (((float)(SColor32(xi,yi,src_size_w,src_size_h).red) * (1-xd) * (1-yd))
	 							   + ((float)(SColor32(xi+1,yi+1,src_size_w,src_size_h).red) * xd * yd)
	 							   + ((float)(SColor32(xi+1,yi,src_size_w,src_size_h).red) * xd * (1-yd)) 
		 						   + ((float)(SColor32(xi,yi+1,src_size_w,src_size_h).red) * (1-xd) * yd)));
		 		c.green=(uchar) (0.5 + (((float)(SColor32(xi,yi,src_size_w,src_size_h).green) * (1-xd) * (1-yd))
	 							   + ((float)(SColor32(xi+1,yi+1,src_size_w,src_size_h).green) * xd * yd)
	 							   + ((float)(SColor32(xi+1,yi,src_size_w,src_size_h).green) * xd * (1-yd)) 
		 						   + ((float)(SColor32(xi,yi+1,src_size_w,src_size_h).green) * (1-xd) * yd)));
		 		c.blue=(uchar) (0.5 + (((float)(SColor32(xi,yi,src_size_w,src_size_h).blue) * (1-xd) * (1-yd))
	 							   + ((float)(SColor32(xi+1,yi+1,src_size_w,src_size_h).blue) * xd * yd)
	 							   + ((float)(SColor32(xi+1,yi,src_size_w,src_size_h).blue) * xd * (1-yd)) 
	 							   + ((float)(SColor32(xi,yi+1,src_size_w,src_size_h).blue) * (1-xd) * yd)));
		 		c.alpha=(uchar) (0.5 + (((float)(SColor32(xi,yi,src_size_w,src_size_h).alpha) * (1-xd) * (1-yd))
	 							   + ((float)(SColor32(xi+1,yi+1,src_size_w,src_size_h).alpha) * xd * yd)
	 							   + ((float)(SColor32(xi+1,yi,src_size_w,src_size_h).alpha) * xd * (1-yd)) 
	 							   + ((float)(SColor32(xi,yi+1,src_size_w,src_size_h).alpha) * (1-xd) * yd)));
				*bits++ = c.red;
				*bits++ = c.green;
				*bits++ = c.blue;
				*bits++ = c.alpha;
	 		}
	 	return;
	 }
}

void DecreaseBitmap(BBitmap *Src,BBitmap *Dst)
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


