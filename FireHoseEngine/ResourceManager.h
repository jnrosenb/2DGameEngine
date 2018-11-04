/* Start Header -------------------------------------------------------
*  Copyright (C) 2018 DigiPen Institute of Technology.
*  Reproduction or disclosure of this file or its contents without the prior
*  written consent of DigiPen Institute of Technology is prohibited.

*  File Name: ResourceManager.h
*  Purpose: ResourceManager.cpp header file.
*  Project: CS529_JRosenbluth_3
*  Author: Jose Rosenbluth Chiu, J.Rosenbluth, 60003418
*  Creation date: 10/14/2018
-  End Header --------------------------------------------------------*/

#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_image.h>
#include <unordered_map>
#include <string>

class ResourceManager 
{
public:
	ResourceManager();
	~ResourceManager();

	SDL_Surface *loadSurface(std::string path);

private:
	std::unordered_map<std::string, SDL_Surface *> mSurfaces;
};


#endif