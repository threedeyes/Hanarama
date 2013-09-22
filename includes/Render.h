/*
 * Copyright 2013, Gerasim Troeglazov (3dEyes**), 3dEyes@gmail.com.
 * All rights reserved.
 * Distributed under the terms of the MIT License.
 */

#ifndef _H_P_RENDER_
#define _H_P_RENDER_

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <OS.h>
#include <Bitmap.h>
#include <View.h>
#include <OS.h>

#include "FastMath.h"
#include "Camera.h"

class PRender;

#define MAX_RENDER_THREADS	16

typedef struct {
	thread_id		threadId;
	int32			fromLine;
	int32			toLine;
	sem_id			threadLocker;
	PRender			*renderObject;
} RenderThreadInfo;

class PRender
{
 public:
			PRender();
			PRender(BBitmap *src, BBitmap *dst, PCamera *camera);
			PRender(BBitmap *src, uint32 *dst_buffer, int32 w, int32 h, PCamera *camera);
			~PRender();
	
	void	SetPanoramaBitmap(BBitmap *bitmap);
	void	SetFrameBufferBitmap(BBitmap *bitmap);
	void	SetCamera(PCamera *camera);
	void	InitMultiRenders(int32 threads);
	void	LeaveMultiRender(void);
	void	RenderSegment(int32 from, int32 to);
	void	Render(void);
 
 private:
	CameraCoeffs		fCamCoeffs;
 	PCamera				*fCamera;

	BBitmap 			*fPanoramaBitmap;
	BBitmap 			*fFrameBufferBitmap;	
	
	uint32				*panoramaBuffer;
	int32				panoramaWidth;
	int32				panoramaHeight;
	int32				panoramaBufferSize;
	
	uint32				*frameBuffer;
	int32				frameWidth;
	int32				frameHeight;
	int32				frameBufferSize;
	
	int32				threadsCount;
	RenderThreadInfo	renderThreads[MAX_RENDER_THREADS];

public:
	sem_id				readyLocker;
};

#endif
