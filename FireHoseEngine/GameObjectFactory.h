#ifndef GAMEOBJECTFACTORY_H
#define GAMEOBJECTFACTORY_H

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