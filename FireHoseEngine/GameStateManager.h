#ifndef GAMESTATEMANAGER_H
#define GAMESTATEMANAGER_H

#include <vector>
#include "Events.h"

class Manager;
class GameObjectFactory;


enum class GameState 
{
	NONE = -1,
	MAIN_MENU,
	INSTRUCTIONS,
	CREDITS,
	PAUSE,
	RESTART,
	GAMEOVER,
	LEVEL_1,
	NUM
};


class GameStateManager 
{
public:
	GameStateManager();
	~GameStateManager();

	void init(int width, int height);
	void checkStateChangeCondition();
	void switchState();
	void SetNextState(GameState next);
	void loadState();
	void unloadState();

	void handleRestart();
	void RestartCurrentLevel();
	void TogglePause();
	GameState GetCurrentState();

	Manager* GetStateManager(GameState state);

private:
	//Container for the different states managers
	std::vector<Manager*> stateManagers;

private:
	GameObjectFactory *factory;

	GameState currentState;
	GameState nextState;
	GameState readyToPopState;
	bool backFromPause;
};

#endif