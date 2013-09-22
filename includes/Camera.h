/*
 * Copyright 2013, Gerasim Troeglazov (3dEyes**), 3dEyes@gmail.com.
 * All rights reserved.
 * Distributed under the terms of the MIT License.
 */

#ifndef _H_P_CAMERA_
#define _H_P_CAMERA_

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <OS.h>

#define CAM_MODE_MANUAL				0
#define CAM_MODE_AUTO_PANNIG		1
#define CAM_MODE_AUTO_RANDOM		2
#define CAM_MODE_AUTO_ROTATE_LEFT	3
#define CAM_MODE_AUTO_ROTATE_RIGHT	4

typedef struct {
	float 	RatioUp;
	float 	RatioRight;
	float 	DirX;
	float 	DirY;
	float 	DirZ;
	float 	UpX;
	float 	UpY;
	float 	UpZ;
	float 	RightX;
	float 	RightZ;
	float 	PlaneOriginX;
	float 	PlaneOriginY;
	float 	PlaneOriginZ;
} CameraCoeffs;

class PCamera
{
 public:
			PCamera();
			PCamera(float heading, float pitch, float fov);
			~PCamera();
	
	void	SetCamera(float heading, float pitch, float fov);
	float	SetHeading(float heading);
	float	SetPitch(float pitch);
	float	SetFOV(float fov);

	float	MoveHeadingBy(float dheading);
	float	MovePitchBy(float dpitch);
	float	MoveFOVBy(float dfov);
		
	float	Heading(void);
	float	Pitch(void);
	float	FOV(void);
	
	int		SetMode(int mode);
	int		Mode(void);
	
	void	GetCoeffs(CameraCoeffs *coeffs);
	
 private:
 
 	void	recalcCamera(void);
 	void	startThread(void);
 	void	stopThread(void);
 	
 	float	fHeading;
 	float	fPitch;
 	float	fFOV;
 	
	int		fCameraMode;
 	CameraCoeffs	fCoeffs;
 	
	thread_id	CamThreadId;

 public:	
 	float	fTimeFactor;
};

#endif
