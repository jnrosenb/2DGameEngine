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
	GAMEWON,
	LEVEL_1,
	NUM
};


class GameStateManager 
{
public:
	GameStateManager();
	~GameStateManager();

	void init(ResourceManager *RM, GraphicsManager *GM, int w, int h);
	void checkStateChangeCondition();
	void switchState();
	void SetNextState(GameState next);
	void loadState();
	void unloadState();
	void unloadState(int numOfRenderersToPop);

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
	int deltaPauseObjs;
};

#endif