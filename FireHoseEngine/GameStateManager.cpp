#include "GameStateManager.h"
#include "GameObjectFactory.h"
#include "Managers.h"
#include <iostream>

extern Manager *pManager;


GameStateManager::GameStateManager()
{
	backFromPause = false;
}


GameStateManager::~GameStateManager()
{
	delete factory;
}


void GameStateManager::init(int width, int height) 
{
	//Factory in charge of creating all GGOO
	factory = new GameObjectFactory();

	//Create a gral manager for each state
	for (int i = 0; i < static_cast<int>(GameState::NUM); ++i) 
	{
		Manager *mgr = new Manager();
		
		pManager = mgr;
		mgr->GetGraphicManager()->init(width, height);
		stateManagers.push_back(mgr); 
		pManager = 0;
	}

	readyToPopState = GameState::NONE;
	currentState = GameState::NONE;
	nextState = GameState::MAIN_MENU;
	loadState();
}


Manager *GameStateManager::GetStateManager(GameState state)
{
	//TODO check if this could cause error
	return stateManagers[static_cast<int>(state)];
}


void GameStateManager::SetNextState(GameState next) 
{
	nextState = next;
}


void GameStateManager::switchState()
{
	if (nextState == GameState::PAUSE)
	{
		loadState();
	}
	else
	{
		//Delete every gameobject and their components. 
		//Every animationClip, and I dont know what else
		if (currentState != GameState::NONE)
		{
			unloadState();
		}

		if (readyToPopState != GameState::NONE) 
		{
			pManager = GetStateManager(readyToPopState);
			unloadState();
		}

		loadState();
	}
}


void GameStateManager::loadState()
{
	switch (nextState) 
	{
	case GameState::MAIN_MENU:
		pManager = GetStateManager(GameState::MAIN_MENU);
		factory->LoadLevel("Resources/UI_MainMenu.txt");
		currentState = GameState::MAIN_MENU;
		break;
	case GameState::INSTRUCTIONS:
		pManager = GetStateManager(GameState::INSTRUCTIONS);
		factory->LoadLevel("Resources/UI_Controls.txt");
		currentState = GameState::INSTRUCTIONS;
		break;
	case GameState::CREDITS:
		pManager = GetStateManager(GameState::CREDITS);
		factory->LoadLevel("Resources/UI_Credits.txt");
		currentState = GameState::CREDITS;
		break;
	case GameState::GAMEOVER:
		pManager = GetStateManager(GameState::GAMEOVER);
		factory->LoadLevel("Resources/UI_Gameover.txt");
		currentState = GameState::GAMEOVER;
		break;
	case GameState::RESTART:
		handleRestart();
		break;
	case GameState::PAUSE:
		pManager = GetStateManager(GameState::PAUSE);
		factory->LoadLevel("Resources/UI_Pause.txt");
		currentState = GameState::PAUSE;
		break;
	case GameState::LEVEL_1:
		pManager = GetStateManager(GameState::LEVEL_1);
		if (!backFromPause)
			factory->LoadLevel("Resources/level1_.txt");
		backFromPause = false;
		currentState = GameState::LEVEL_1;
		break;
	/*
	case GameState::LEVEL_2:
		pManager = GetStateManager(GameState::LEVEL_2);
		if (!backFromPause)
			factory->LoadLevel("Resources/level2.txt");
		backFromPause = false;
		pManager = GetStateManager(GameState::LEVEL_2);
		break;
	case GameState::LEVEL_3:
		pManager = GetStateManager(GameState::LEVEL_3);
		if (!backFromPause)
			factory->LoadLevel("Resources/level3.txt");
		backFromPause = false;
		pManager = GetStateManager(GameState::LEVEL_3);
		break;
	case GameState::LEVEL_4:
		pManager = GetStateManager(GameState::LEVEL_4);
		if (!backFromPause)
			factory->LoadLevel("Resources/level4.txt");
		backFromPause = false;
		pManager = GetStateManager(GameState::LEVEL_4);
		break;
	//*/
	};
}


void GameStateManager::handleRestart() 
{
	currentState = GameState::RESTART;
	
	nextState = readyToPopState;
	readyToPopState = GameState::NONE;
	loadState();
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
		///Unload the current pause state
		//unloadState();
		
		///Inmediately assign level as state (skip load)
		//nextState = readyToPopState;
		//readyToPopState = GameState::NONE;
		//currentState = nextState;
		//pManager = GetStateManager(currentState);

		backFromPause = true;
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


GameState GameStateManager::GetCurrentState()
{
	return currentState;
}



