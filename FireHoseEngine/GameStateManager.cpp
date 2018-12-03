#include "GameStateManager.h"
#include "GameObjectFactory.h"
#include "Managers.h"
#include <iostream>

extern Manager *pManager;


GameStateManager::GameStateManager()
{
}

GameStateManager::~GameStateManager()
{
	delete factory;
}

void GameStateManager::init() 
{
	factory = new GameObjectFactory();

	readyToPopState = GameState::NONE;
	currentState = GameState::NONE;
	nextState = GameState::MAIN_MENU;
}

void GameStateManager::SetNextState(GameState next) 
{
	nextState = next;
}

void GameStateManager::switchState()
{
	if (currentState != GameState::NONE)
	{
		//Delete every gameobject and their components. 
		//Every animationClip, and I dont know what else
		unloadState();
	}

	loadState();

	/*if (nextState == GameState::RESTART || nextState == GameState::PAUSE) 
	{
		loadState();
	}
	else 
	{
		if (currentState != GameState::NONE) 
		{
			//Delete every gameobject and their components. 
			//Every animationClip, and I dont know what else
			unloadState();
		}

		loadState();
	}//*/
}

void GameStateManager::loadState()
{
	switch (nextState) 
	{
	case GameState::MAIN_MENU:
		factory->LoadLevel("Resources/MainMenu.txt");
		currentState = GameState::MAIN_MENU;
		break;
	case GameState::INSTRUCTIONS:
		break;
	case GameState::CREDITS:
		break;
	case GameState::RESTART:
		handleRestart();
		break;
	case GameState::PAUSE:
		factory->LoadLevel("Resources/Pause.txt");
		currentState = GameState::PAUSE;
		break;
	case GameState::LEVEL_1:
		factory->LoadLevel("Resources/level1_.txt");
		currentState = GameState::LEVEL_1;
		break;
	case GameState::LEVEL_2:
		break;
	case GameState::LEVEL_3:
		break;
	case GameState::LEVEL_4:
		break;
	};
}

void GameStateManager::handleRestart() 
{
	currentState = GameState::RESTART;
	nextState = readyToPopState;
	loadState();
	readyToPopState = GameState::NONE;
}

void GameStateManager::RestartCurrentLevel()
{
	//SAVE PREV STATE, THEN JUMP TO PAUSE
	readyToPopState = currentState;
	nextState = GameState::RESTART;
}

void GameStateManager::TogglePause()
{
	if (currentState == GameState::PAUSE) 
	{
		//RETURN TO PREV STATE
		nextState = readyToPopState;
		readyToPopState = GameState::NONE;
	}
	else 
	{
		//SAVE PREV STATE, THEN JUMP TO PAUSE
		readyToPopState = currentState;
		nextState = GameState::PAUSE;
	}
}

void GameStateManager::unloadState()
{
	pManager->UnloadManagers();
}

void GameStateManager::checkStateChangeCondition() 
{
	if (currentState != nextState) 
		switchState();
}