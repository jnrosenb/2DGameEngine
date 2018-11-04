/* Start Header -------------------------------------------------------
*  Copyright (C) 2018 DigiPen Institute of Technology.
*  Reproduction or disclosure of this file or its contents without the prior
*  written consent of DigiPen Institute of Technology is prohibited.

*  File Name: InputManager.h
*  Purpose: InputManager.cpp header file.
*  Project: CS529_JRosenbluth_3
*  Author: Jose Rosenbluth Chiu, J.Rosenbluth, 60003418
*  Creation date: 10/14/2018
-  End Header --------------------------------------------------------*/

#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include "SDL2/SDL_stdinc.h"

class InputManager 
{
public:
	InputManager();
	~InputManager();

	void update();

	bool getKeyPress(unsigned int keyscancode);
	bool getKeyTrigger(unsigned int keyscancode);
	bool getKeyReleased(unsigned int keyscancode);

private:

	Uint8 *currKeyboardState;
	Uint8 *prevKeyboardState;
};

#endif