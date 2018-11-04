/* Start Header -------------------------------------------------------
*  Copyright (C) 2018 DigiPen Institute of Technology.
*  Reproduction or disclosure of this file or its contents without the prior
*  written consent of DigiPen Institute of Technology is prohibited.

*  File Name: InputManager.cpp
*  Purpose: Catches and manages the user input.
*  Project: CS529_JRosenbluth_3
*  Author: Jose Rosenbluth Chiu, J.Rosenbluth, 60003418
*  Creation date: 10/14/2018
-  End Header --------------------------------------------------------*/

#include "InputManager.h"
#include "SDL2/SDL_stdinc.h"
#include "SDL2/SDL_keyboard.h"
#include <iostream>

using namespace std;

InputManager::InputManager() 
{
	Uint32 keyboardLen = 512;

	this->currKeyboardState = (Uint8*)malloc(keyboardLen * sizeof(Uint8));
	this->prevKeyboardState = (Uint8*)malloc(keyboardLen * sizeof(Uint8));

	SDL_memset(this->currKeyboardState, 0, keyboardLen * sizeof(Uint8));
	SDL_memset(this->prevKeyboardState, 0, keyboardLen * sizeof(Uint8));

	cout << "Input manager constructor." << endl;
}

InputManager::~InputManager() 
{
	free(currKeyboardState);
	free(prevKeyboardState);

	cout << "Input manager destructor." << endl;
}

void InputManager::update() 
{
	int keyboardLen;
	Uint8 const *keyboardState = SDL_GetKeyboardState(&keyboardLen);

	if (keyboardLen > 512)
		keyboardLen = 512;

	SDL_memcpy(this->prevKeyboardState, this->currKeyboardState, keyboardLen);
	SDL_memcpy(this->currKeyboardState, keyboardState, keyboardLen);
}

bool InputManager::getKeyPress(unsigned int keyscancode) 
{
	if (keyscancode > 512)
		return false;

	if (currKeyboardState[keyscancode])
		return true;
	return false;
}

bool InputManager::getKeyTrigger(unsigned int keyscancode)
{
	if (keyscancode > 512)
		return false;

	if (currKeyboardState[keyscancode] && !prevKeyboardState[keyscancode])
		return true;
	return false;
}

bool InputManager::getKeyReleased(unsigned int keyscancode)
{
	if (keyscancode > 512)
		return false;

	if (!currKeyboardState[keyscancode] && prevKeyboardState[keyscancode])
		return true;
	return false;
}