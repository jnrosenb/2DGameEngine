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

private:
	std::unordered_map<std::string, Component*> componentMap;
};

#endif