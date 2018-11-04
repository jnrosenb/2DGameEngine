/* Start Header -------------------------------------------------------
*  Copyright (C) 2018 DigiPen Institute of Technology.
*  Reproduction or disclosure of this file or its contents without the prior
*  written consent of DigiPen Institute of Technology is prohibited.

*  File Name: FrameRateController.cpp
*  Purpose: Controls Framerate so it has a defined cap.
*  Project: CS529_JRosenbluth_3
*  Author: Jose Rosenbluth Chiu, J.Rosenbluth, 60003418
*  Creation date: 10/14/2018
-  End Header --------------------------------------------------------*/

#ifndef FRAMERATECONTROLLER_H
#define FRAMERATECONTROLLER_H

#include "SDL2/SDL_stdinc.h"

class FrameRateController 
{
public:
	FrameRateController(int framesPerSecond);
	~FrameRateController();

	void FrameStart();
	void FrameEnd(); 
	Uint32 getFrameTime();

private:
	Uint32 pTicksBegin;
	Uint32 pTicksEnd;
	Uint32 pTicksNeededPerFrame;

	Uint32 pFrameTime;
};

#endif