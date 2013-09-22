/*
 * Copyright 2013, Gerasim Troeglazov (3dEyes**), 3dEyes@gmail.com.
 * All rights reserved.
 * Distributed under the terms of the MIT License.
 */

#include "Camera.h"

int32 AutoCamThread(void *data)
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
		
		if(fCamera->Mode() == CAM_MODE_AUTO_PANNIG) {
			float time = system_time()/1000000.0;
			float fov = (30+(0.5+sin(tFactor * time/8)*cos(tFactor * time/11.7)*sin(tFactor * time/7.1))*60)*M_PI/180.0;	
			float heading = ((sin(tFactor * time/19.2)*sin(tFactor * time/16.34)+1)/2)*4*M_PI;
			float pitch = (20+((sin(tFactor * time/5.1)*cos(tFactor * time/7.2)+1)/2)*150)*M_PI/180.0;
			fCamera->SetCamera(heading, pitch, fov);
		} else if(fCamera->Mode() == CAM_MODE_AUTO_RANDOM) {
			float time = system_time()/1000000.0;
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
			fCamera->SetPitch(lastPitch + dPitch*step);
			fCamera->SetFOV(lastFOV + dFOV*step);
			fCamera->SetHeading(lastHeading + dHeading*step);
			step = step<200?(step+1):step;
		} else if(fCamera->Mode() == CAM_MODE_AUTO_ROTATE_LEFT) {
			float time = system_time()/1000000.0;
			float heading = (time * M_PI * 2)	* tFactor * 0.02;
			float fov = 1.3;
			float pitch = M_PI_2;
			fCamera->SetCamera(heading, pitch, fov);
		} else if(fCamera->Mode() == CAM_MODE_AUTO_ROTATE_RIGHT) {
			float time = system_time()/1000000.0;
			float heading = - (time * M_PI * 2)	* tFactor * 0.02;
			float fov = 1.3;
			float pitch = M_PI_2;
			fCamera->SetCamera(heading, pitch, fov);
		}		
				
		snooze(5000);
	}
}

PCamera::PCamera()
{
	fCameraMode = CAM_MODE_MANUAL;
	fTimeFactor = 1.0;
	SetCamera(M_PI_2, M_PI_2, M_PI_2);
	startThread();
}


PCamera::PCamera(float heading, float pitch, float fov)
{
	fCameraMode = CAM_MODE_MANUAL;
	fTimeFactor = 1.0;
	SetCamera(heading, pitch, fov);
	startThread();
}


PCamera::~PCamera()
{
	stopThread();
}


void
PCamera::SetCamera(float heading, float pitch, float fov)
{
	fHeading = heading;
	fPitch = pitch;
	fFOV = fov;
	recalcCamera();	
}


float
PCamera::SetHeading(float heading)
{
	fHeading = heading;	
	recalcCamera();
	return fHeading;
}


float
PCamera::SetPitch(float pitch)
{
	fPitch = pitch;
	recalcCamera();
	return fPitch;
}


float
PCamera::SetFOV(float fov)
{
	fFOV = fov;
	recalcCamera();
	return fFOV;
}


float
PCamera::MoveHeadingBy(float dheading)
{
	fHeading += dheading;	
	recalcCamera();
	return fHeading;
}


float
PCamera::MovePitchBy(float dpitch)
{
	fPitch += dpitch;
	recalcCamera();
	return fPitch;
}


float
PCamera::MoveFOVBy(float dfov)
{
	fFOV += dfov;
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
	memcpy(coeffs, &fCoeffs, sizeof(CameraCoeffs));
}

void
PCamera::recalcCamera(void)
{
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
}

void
PCamera::startThread(void)
{
	CamThreadId = spawn_thread(AutoCamThread, "AutoCamThread", 20, (void*)this);
	resume_thread(CamThreadId);
}

void
PCamera::stopThread(void)
{
	kill_thread(CamThreadId);
}
