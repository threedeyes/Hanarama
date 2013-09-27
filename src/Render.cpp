/*
 * Copyright 2013, Gerasim Troeglazov (3dEyes**), 3dEyes@gmail.com.
 * All rights reserved.
 * Distributed under the terms of the MIT License.
 */
 
#include "Render.h"


PRender::PRender()
{
	fPanoramaBitmap = NULL;
	fFrameBufferBitmap = NULL;
	fCamera = NULL;
	frameWidth = 0;
	frameHeight = 0;	
	threadsCount = 1;
}


PRender::PRender(BBitmap *src, BBitmap *dst, PCamera *camera)
{
	SetPanoramaBitmap(src);

	SetFrameBufferBitmap(dst);
	threadsCount = 1;		

	SetCamera(camera);
}


PRender::PRender(BBitmap *src, uint32 *dst_buffer, int32 w, int32 h, PCamera *camera)
{
	SetPanoramaBitmap(src);
	
	fFrameBufferBitmap = NULL;
	frameBuffer = dst_buffer;
	frameWidth = w;
	frameHeight = h;
	frameBufferSize = w * h;
	threadsCount = 1;
	
	SetCamera(camera);
}


PRender::~PRender()
{
	if(threadsCount > 1) {
		for(int i = 0; i < threadsCount; i++)
			kill_thread(renderThreads[i].threadId);
		for(int i = 0; i < threadsCount; i++)
			delete_sem(renderThreads[i].threadLocker);
		delete_sem(readyLocker);
	}
}

	
void
PRender::SetPanoramaBitmap(BBitmap *bitmap)
{
	fPanoramaBitmap = bitmap;
	panoramaWidth = (int32)(fPanoramaBitmap->Bounds().Width() + 1.0);
	panoramaHeight = (int32)(fPanoramaBitmap->Bounds().Height() + 1.0);
	panoramaBuffer = (uint32*)fPanoramaBitmap->Bits();
	panoramaBufferSize = fPanoramaBitmap->BitsLength() / sizeof(uint32);
	FmRecalcTables(panoramaWidth, panoramaHeight);
}


void
PRender::SetFrameBufferBitmap(BBitmap *bitmap)
{
	fFrameBufferBitmap = bitmap;
	frameWidth = (int32)(fFrameBufferBitmap->Bounds().Width() + 1.0);
	frameHeight = (int32)(fFrameBufferBitmap->Bounds().Height() + 1.0);
	frameBuffer = (uint32*)fFrameBufferBitmap->Bits();
	frameBufferSize = fFrameBufferBitmap->BitsLength() / sizeof(uint32);
}


void
PRender::SetCamera(PCamera *camera)
{
	fCamera = camera;
}

int32 
MultiRenderThread(void *data)
{
	RenderThreadInfo *threadInfo = (RenderThreadInfo*)data;
	printf("RenderThread #%ld (%ld-%ld) started\n", threadInfo->threadId, threadInfo->fromLine, threadInfo->toLine);
	for(;;) {		
		acquire_sem(threadInfo->threadLocker);
		threadInfo->renderObject->RenderSegment(threadInfo->fromLine, threadInfo->toLine);
		release_sem(threadInfo->renderObject->readyLocker);	
	}
}

void
PRender::InitMultiRenders(int32 threads)
{
	if(threadsCount == 1 && threads <= 1)
		return;
	if(frameBuffer == NULL || frameWidth <= 0 || frameHeight <= 0)
		return;
		
	int32 blockHeight = (frameHeight / threads) - 1;
	int32 from = 0, to = 0;
		
	for(int i = 0; i < threads; i++, from = to + 1) {
		char threadName[] = {"Render #0"};
		threadName[8] = '1' + i;
		if(i < threads -1) {			
			to = from + blockHeight;	
		} else {
			to = frameHeight - 1;
		}		
		renderThreads[i].renderObject = this;
		renderThreads[i].fromLine = from;
		renderThreads[i].toLine = to;
		renderThreads[i].threadLocker = create_sem(0, threadName);
		renderThreads[i].threadId = spawn_thread(MultiRenderThread, 
				threadName, B_URGENT_DISPLAY_PRIORITY, (void*)&renderThreads[i]);
		resume_thread(renderThreads[i].threadId);
	}
	readyLocker =  create_sem( 0, "readyLocker");
	threadsCount = threads;
	snooze(100000);
}


void
PRender::RenderSegment(int32 from, int32 to)
{
	int32	i, j;
	uint32	pixelShift;

	float	rayY;
	float	rayYY;		
	float 	fx, fy;
	float 	rayX,rayZ;
	float 	fx_camX;
	float	dfx_camX;
	float 	fx_camZ;
	float	dfx_camZ;

	float 	dfx = 1.0 / frameWidth;
	float 	dfy = 1.0 / frameHeight;

	uint32	*frameOffset = frameBuffer + from * frameWidth;
						
	for(i = from, fy = dfy * from; i <= to; i++, fy+=dfy) {
		fx_camX = fCamCoeffs.PlaneOriginX - fy*fCamCoeffs.UpX;
		fx_camZ = fCamCoeffs.PlaneOriginZ - fy*fCamCoeffs.UpZ;
		rayY = fCamCoeffs.PlaneOriginY - fy*fCamCoeffs.UpY;
		rayYY = rayY * rayY;
		dfx_camX = dfx*fCamCoeffs.RightX;
		dfx_camZ = dfx*fCamCoeffs.RightZ;
		
		for(j = 0; j < frameWidth; j++, fx_camX += dfx_camX, fx_camZ += dfx_camZ) {
			pixelShift = FmAcos(rayY * FmRsqrtPrec(fx_camX * fx_camX + rayYY + fx_camZ * fx_camZ))
						 + FmAtan2(fx_camZ, fx_camX);
			if(pixelShift < panoramaBufferSize)
				*frameOffset++ = panoramaBuffer[pixelShift];
			else 
				*frameOffset++ = 0;
		}
	}
}


void
PRender::Render(void)
{
	fCamera->GetCoeffs(&fCamCoeffs);
	if(threadsCount == 1)
		RenderSegment(0, frameHeight - 1);
	else {
		for(int i = 0; i < threadsCount; i++ )
			release_sem(renderThreads[i].threadLocker);		
		acquire_sem_etc(readyLocker, threadsCount, 0, 0);
	}
}
