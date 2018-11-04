/* Start Header -------------------------------------------------------
*  Copyright (C) 2018 DigiPen Institute of Technology.
*  Reproduction or disclosure of this file or its contents without the prior
*  written consent of DigiPen Institute of Technology is prohibited.

*  File Name: FrameRateController.h
*  Purpose: FrameRateController.cpp header file.
*  Project: CS529_JRosenbluth_3
*  Author: Jose Rosenbluth Chiu, J.Rosenbluth, 60003418
*  Creation date: 10/14/2018
-  End Header --------------------------------------------------------*/

#include "FrameRateController.h"
#include <iostream>
#include <SDL2/SDL_timer.h>

using namespace std;

FrameRateController::FrameRateController(int framesPerSecond) 
{
	pTicksBegin = 0, pTicksEnd = 0, pFrameTime;

	if (framesPerSecond > 0)
	{
		//Because ticks are in milliseconds
		pTicksNeededPerFrame = 1000 / framesPerSecond;
	}
	else
		pTicksNeededPerFrame = 0;

	//This is just for the first frame
	pFrameTime = pTicksNeededPerFrame;

	cout << "FramerateController constructor." << endl;
}

FrameRateController::~FrameRateController() 
{
	cout << "FramerateController destructor." << endl;
}

void FrameRateController::FrameStart()
{
	pTicksBegin = SDL_GetTicks();
}

Uint32 FrameRateController::getFrameTime()
{
	return pFrameTime;
}

void FrameRateController::FrameEnd() 
{
	pTicksEnd = SDL_GetTicks();

	while (pTicksEnd - pTicksBegin < pTicksNeededPerFrame) 
	{
		pTicksEnd = SDL_GetTicks();
	}

	pFrameTime = pTicksEnd - pTicksBegin;
}