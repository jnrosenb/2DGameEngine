#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

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