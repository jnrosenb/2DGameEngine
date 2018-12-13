#ifndef GAMEOBJECTFACTORY_H
#define GAMEOBJECTFACTORY_H

/* Start Header -------------------------------------------------------
*  Copyright (C) 2018 DigiPen Institute of Technology.
*  Reproduction or disclosure of this file or its contents without the prior
*  written consent of DigiPen Institute of Technology is prohibited.

*  File Name: InputManager.h
*  Purpose: Main source file. Contains the main loop, and is in charge of
*  creating and destroying all the managers used in the engine.
*  Project: CS529_JRosenbluth_3
*  Author: Jose Rosenbluth Chiu, J.Rosenbluth, 60003418
*  Creation date: 10/14/2018
-  End Header --------------------------------------------------------*/

#include <string>
#include <fstream>
#include <unordered_map>

class Component;
class GameObject;

class GameObjectFactory 
{
public:
	GameObjectFactory(); 
	~GameObjectFactory();

	void LoadLevel(char const *path); /*TAKE THIS OUT LATER*/
	GameObject *BuildGameObject(std::string goPath);
	void BuildComponent(std::fstream& stream, std::string component, GameObject *owner, Component **newComp);

	void SuscribeEvent(std::string line, GameObject *go);

private:
	std::unordered_map<std::string, Component*> componentMap;

	//Experiment
	void sendCorrectEventKey(std::string currentEvent, std::string key, GameObject *go);
};

#endif