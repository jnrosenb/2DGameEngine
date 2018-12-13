#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

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
#include <vector>
#include <unordered_map>
#include "EventManager.h"

class Component;
class Event;

class GameObject 
{
public:
	GameObject();
	~GameObject();
	
	void Update(unsigned int deltaTime);
	void Draw();

	void AddComponent(unsigned int type);
	void AddComponent(Component *c);
	Component *GetComponent(unsigned int type);

	void handleEvent(Event *pEvent);

	void setEnabled(bool flag);
	bool isEnabled();

	//Experiment
	void addEventKey(EventType type, std::string key);
	std::vector<std::string> const& getEventKeys(EventType type);

private:
	std::vector<Component*> goComponents;
	bool enabled;

	//Experiment
	std::unordered_map< EventType, std::vector< std::string > > EventTypeKeyMap;
};

#endif