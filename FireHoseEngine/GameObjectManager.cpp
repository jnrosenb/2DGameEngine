#include <iostream>
#include "GameObjectManager.h"
#include "GameObject.h"

GameObjectManager::GameObjectManager()
{
}

GameObjectManager::~GameObjectManager()
{
	for (GameObject *go : mGameObjects)
	{
		delete go;
	}
	mGameObjects.clear();
}

void GameObjectManager::AddGameObject(GameObject *go)
{
	if (go != 0)
	{		
		mGameObjects.push_back(go);
	}
	else 
	{
		std::cout << "Cannot add NULL gameObject" << std::endl;
	}
}

void GameObjectManager::Update(unsigned int deltaTime)
{
	for (GameObject *go : mGameObjects)
	{
		go->Update(deltaTime);
	}
}

void GameObjectManager::Draw()
{
	/*for (GameObject *go : mGameObjects)
	{
		go->Draw();
	}//*/

	std::cout << "(GameObjectManager::Draw)- NOT BEING USED RIGHT NOW" << std::endl;
}

unsigned int GameObjectManager::GetNumOfGO()
{
	return mGameObjects.size();
}

GameObject *GameObjectManager::GetGOByIndex(unsigned int index)
{
	if (index >= 0 && index < mGameObjects.size()) 
	{
		return mGameObjects[index];
	}
	else 
	{
		std::cout << "(GameObjectManager::GetGOByIndex)- No GameObject in this index. Returning NULL" << std::endl;
		return 0;
	}
}

std::vector<GameObject*> GameObjectManager::getGoList()
{
	return mGameObjects;
}