#ifndef GAMEOBJECTMANAGER_H
#define GAMEOBJECTMANAGER_H

#include <vector>

class GameObject; /*Forward declaration*/

class GameObjectManager 
{
public:
	GameObjectManager();
	~GameObjectManager();

	void AddGameObject(GameObject *go);
	void Update(unsigned int deltaTime);
	unsigned int GetNumOfGO();
	GameObject *GetGOByIndex(unsigned int index);
	void Draw();

	std::vector<GameObject*> getGoList();

private:
	std::vector<GameObject*> mGameObjects;
};

#endif