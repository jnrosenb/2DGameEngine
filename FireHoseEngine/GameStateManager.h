#ifndef GAMESTATEMANAGER_H
#define GAMESTATEMANAGER_H

class GameObjectFactory;


enum class GameState 
{
	NONE = -1,
	MAIN_MENU,
	INSTRUCTIONS,
	CREDITS,
	PAUSE,
	RESTART,
	LEVEL_1,
	LEVEL_2,
	LEVEL_3,
	LEVEL_4
};


class GameStateManager 
{
public:
	GameStateManager();
	~GameStateManager();

	void init();
	void checkStateChangeCondition();
	void switchState();
	void SetNextState(GameState next);
	void loadState();
	void unloadState();

	void handleRestart();
	void RestartCurrentLevel();
	void TogglePause();

private:
	GameObjectFactory *factory;

	GameState currentState;
	GameState nextState;
	GameState readyToPopState;
};

#endif