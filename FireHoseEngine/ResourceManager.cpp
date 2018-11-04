/* Start Header -------------------------------------------------------
*  Copyright (C) 2018 DigiPen Institute of Technology.
*  Reproduction or disclosure of this file or its contents without the prior
*  written consent of DigiPen Institute of Technology is prohibited.

*  File Name: ResourceManager.cpp
*  Purpose: Manages the creation, storage, and freeing of resources during game.
*  Project: CS529_JRosenbluth_3
*  Author: Jose Rosenbluth Chiu, J.Rosenbluth, 60003418
*  Creation date: 10/14/2018
-  End Header --------------------------------------------------------*/

#include "ResourceManager.h"
#include <iostream>

#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_image.h>

using namespace std;

ResourceManager::ResourceManager() 
{
	cout << "Resource manager constructor." << endl;
}

ResourceManager::~ResourceManager() 
{

	for (auto &node : mSurfaces)
	{
		cout << "Releasing surface resource." << endl;
		SDL_FreeSurface(node.second);
		cout << "--OK--." << endl;

	}
	mSurfaces.clear();
	
	cout << "Resource manager destructor." << endl;
}

SDL_Surface *ResourceManager::loadSurface(std::string path) 
{
	SDL_Surface *pSurface = mSurfaces[path];

	if (pSurface == NULL) 
	{
		cout << "Image not loaded yet. Proceeding to load." << endl;

		pSurface = IMG_Load(path.c_str());
		if (pSurface) 
		{
			cout << "Image loaded succesfully. Adding to mSurfaces dictionary." << endl;
			mSurfaces[path] = pSurface;
		}
		else 
		{
			cout << "Load failed." << endl;
		}
	}
	else 
	{
		cout << "Image was already loaded. Proceeding to return surface." << endl;
	}

	return pSurface;
}