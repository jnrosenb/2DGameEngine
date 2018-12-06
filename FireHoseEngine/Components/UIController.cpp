#include "UIController.h"
#include "UIStateComponent.h"
#include "ParticleEmitter.h"
#include "Animator.h"
#include "Camera.h"
#include "../GameObject.h"
#include "../EventManager.h"
#include "../GameStateManager.h"
#include "../Events.h"
#include <iostream>

extern GameStateManager *gamestateMgr;
extern Manager *pManager;


UIController::UIController(GameObject *owner, COMPONENT_TYPE type) :
	Controller(owner, type)
{
}


UIController::~UIController()
{
}


void UIController::Update(unsigned int deltaTime)
{
	//Deltatime in seconds
	float dt = deltaTime / 1000.f;
	
	//If controller is not active, skip <<-- TODO - switch for enable -->>
	if (!active) return;
	
	UIStateComponent *uiMgr = static_cast<UIStateComponent*>(getOwner()->GetComponent(COMPONENT_TYPE::UI_STATE_COMPONENT));
	if (uiMgr == 0)
		return;

	//Navigation through menu
	if (pManager->GetInputManager()->getKeyTrigger(SDL_SCANCODE_UP) ||
		pManager->GetInputManager()->getKeyTrigger(SDL_SCANCODE_W))
	{
		uiMgr->MoveSelection(1);
	}
	else if (pManager->GetInputManager()->getKeyTrigger(SDL_SCANCODE_DOWN) ||
		pManager->GetInputManager()->getKeyTrigger(SDL_SCANCODE_S))
	{
		uiMgr->MoveSelection(-1);
	}

	//Selection
	if (pManager->GetInputManager()->getKeyTrigger(SDL_SCANCODE_RETURN))
	{
		//Send info to UIManager to select current button and press it
		uiMgr->pressCurrentSelection();
	}

	//If in pause, then do this
	if (pManager->GetInputManager()->getKeyTrigger(SDL_SCANCODE_P))
	{
		if (gamestateMgr->GetCurrentState() == GameState::PAUSE)
			gamestateMgr->TogglePause();
	}
}

Component *UIController::createNew(GameObject *owner)
{
	return new UIController(owner, COMPONENT_TYPE::CONTROLLER);
}

void UIController::deserialize(std::fstream& stream)
{
	std::cout << "DESERIALIZING CONTROLLER BEGIN" << std::endl;

	active = true; //For now this is hardcoded

	std::cout << "DESERIALIZING CONTROLLER END" << std::endl;
}


void UIController::handleEvent(Event *pEvent)
{
}