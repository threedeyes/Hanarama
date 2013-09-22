/*
 * Copyright 2013, Gerasim Troeglazov (3dEyes**), 3dEyes@gmail.com.
 * All rights reserved.
 * Distributed under the terms of the MIT License.
 */

#include "Camera.h"


PCamera::PCamera()
{
	initCamera(M_PI_2, M_PI_2, M_PI_2);
}


PCamera::PCamera(float heading, float pitch, float fov)
{
	initCamera(heading, pitch, fov);
}


PCamera::~PCamera()
{	
	stopThread();
	delete_sem(fCamLocker);
}

void
PCamera::initCamera(float heading, float pitch, float fov)
{
	fCamLocker = create_sem( 1, "CamLocker");
	fCameraMode = CAM_MODE_MANUAL;
	fTimeFactor = 1.0;
	SetCamera(heading, pitch, fov);
	startThread();	
}

void
PCamera::SetCamera(float heading, float pitch, float fov)
{
	acquire_sem(fCamLocker);
	fHeading = heading;
	fPitch = pitch;
	fFOV = fov;
	release_sem(fCamLocker);
	recalcCamera();	
}


float
PCamera::SetHeading(float heading)
{
	acquire_sem(fCamLocker);
	fHeading = heading;	
	release_sem(fCamLocker);
	recalcCamera();
	return fHeading;
}


float
PCamera::SetPitch(float pitch)
{
	acquire_sem(fCamLocker);
	fPitch = pitch;
	release_sem(fCamLocker);
	recalcCamera();
	return fPitch;
}


float
PCamera::SetFOV(float fov)
{
	acquire_sem(fCamLocker);
	fFOV = fov;
	release_sem(fCamLocker);
	recalcCamera();
	return fFOV;
}


float
PCamera::MoveHeadingBy(float dheading)
{
	acquire_sem(fCamLocker);
	fHeading += dheading;	
	release_sem(fCamLocker);
	recalcCamera();
	return fHeading;
}


float
PCamera::MovePitchBy(float dpitch)
{
	acquire_sem(fCamLocker);
	fPitch += dpitch;
	release_sem(fCamLocker);
	recalcCamera();
	return fPitch;
}


float
PCamera::MoveFOVBy(float dfov)
{
	acquire_sem(fCamLocker);
	fFOV += dfov;
	release_sem(fCamLocker);
	recalcCamera();
	return fFOV;
}		

float
PCamera::Heading(void)
{
	return fHeading;
}


float
PCamera::Pitch(void)
{
	return fPitch;
}


float
PCamera::FOV(void)
{
	return fFOV;
}


int
PCamera::SetMode(int mode)
{
	fCameraMode = mode;
	return fCameraMode;
}


int
PCamera::Mode(void)
{
	return fCameraMode;
}

void
PCamera::GetCoeffs(CameraCoeffs *coeffs)
{
	acquire_sem(fCamLocker);
	memcpy(coeffs, &fCoeffs, sizeof(CameraCoeffs));
	release_sem(fCamLocker);
}

void
PCamera::recalcCamera(void)
{
	acquire_sem(fCamLocker);
	//fPitch = fmin(M_PI, fmax(0,fPitch));
	fFOV = fmin(M_PI_2, fmax(0.5,fFOV));
	
	fCoeffs.RatioUp = 2.0 * tan(fFOV / 2.0);
	fCoeffs.RatioRight = fCoeffs.RatioUp * 1.33;
	fCoeffs.DirX = sin(fPitch) * sin(fHeading);
	fCoeffs.DirY = cos(fPitch);
	fCoeffs.DirZ = sin(fPitch) * cos(fHeading);
	fCoeffs.UpX = fCoeffs.RatioUp * sin(fPitch - M_PI_2) * sin(fHeading);
	fCoeffs.UpY = fCoeffs.RatioUp * cos(fPitch - M_PI_2);
	fCoeffs.UpZ = fCoeffs.RatioUp * sin(fPitch - M_PI_2) * cos(fHeading);
	fCoeffs.RightX = fCoeffs.RatioRight * sin(fHeading - M_PI_2);
	fCoeffs.RightZ = fCoeffs.RatioRight * cos(fHeading - M_PI_2);
	fCoeffs.PlaneOriginX = fCoeffs.DirX + 0.5 * fCoeffs.UpX - 0.5 * fCoeffs.RightX;
	fCoeffs.PlaneOriginY = fCoeffs.DirY + 0.5 * fCoeffs.UpY;
	fCoeffs.PlaneOriginZ = fCoeffs.DirZ + 0.5 * fCoeffs.UpZ - 0.5 * fCoeffs.RightZ;
	release_sem(fCamLocker);
}

int32
PCamera::AutoCamThread(void *data)
{
	PCamera *fCamera = (PCamera*)data;
	
	float lastTime = system_time()/1000000.0;
	float randPitch = fCamera->Pitch(),
		  randFOV = fCamera->FOV(),
		  randHeading = fCamera->Heading();
	float lastPitch = randPitch,
		  lastFOV = randFOV,
		  lastHeading = randHeading;
	float dPitch = 0,
		  dFOV = 0,
		  dHeading = 0;
	float step = 0;
	
	for(;;) {
		float tFactor = fCamera->fTimeFactor;
		float time = system_time()/1000000.0;
		
		if(fCamera->Mode() == CAM_MODE_AUTO_PANNIG) {
			float fov = (30+(0.5+sin(tFactor * time/8)*cos(tFactor * time/11.7)*sin(tFactor * time/7.1))*60)*M_PI/180.0;	
			float heading = ((sin(tFactor * time/19.2)*sin(tFactor * time/16.34)+1)/2)*4*M_PI;
			float pitch = (20+((sin(tFactor * time/5.1)*cos(tFactor * time/7.2)+1)/2)*150)*M_PI/180.0;
			fCamera->SetCamera(heading, pitch, fov);
		} else if(fCamera->Mode() == CAM_MODE_AUTO_RANDOM) {
			if(time - lastTime > 3.0) {
				lastPitch = randPitch;
		  		lastFOV = randFOV;
		  		lastHeading = randHeading;
			
				randPitch = (rand() % 314)/100.0;
				randFOV = (rand() % 314)/100.0;
				randHeading = (rand() % 628)/100.0;
				dPitch = -(lastPitch - randPitch)/200.0;
				dFOV = -(lastFOV - randFOV)/200.0;
				dHeading = -(lastHeading - randHeading)/200.0;
				lastTime = time;
				step = 0;
			}
			fCamera->SetCamera(lastHeading + dHeading * step,
							   lastPitch + dPitch * step,
							   lastFOV + dFOV * step);
							   			
			step = step<200?(step+1):step;
		} else if(fCamera->Mode() == CAM_MODE_AUTO_ROTATE_LEFT) {
			float heading = (time * M_PI * 2)	* tFactor * 0.02;
			float fov = 1.3;
			float pitch = M_PI_2;
			fCamera->SetCamera(heading, pitch, fov);
		} else if(fCamera->Mode() == CAM_MODE_AUTO_ROTATE_RIGHT) {
			float heading = - (time * M_PI * 2)	* tFactor * 0.02;
			float fov = 1.3;
			float pitch = M_PI_2;
			fCamera->SetCamera(heading, pitch, fov);
		}

		snooze(5000);
	}
}

void
PCamera::startThread(void)
{
	fCamThreadId = spawn_thread(PCamera::AutoCamThread, "AutoCamThread", B_NORMAL_PRIORITY, (void*)this);
	resume_thread(fCamThreadId);
}

void
PCamera::stopThread(void)
{
	kill_thread(fCamThreadId);
}
