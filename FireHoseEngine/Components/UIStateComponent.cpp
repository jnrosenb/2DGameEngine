#include "UIStateComponent.h"
#include "Animator.h"
#include "Button.h"
#include "../GameObject.h"
#include "../EventManager.h"
#include "../GameStateManager.h"
#include <string>

extern GameStateManager *gamestateMgr; //REPLACE IN FUTURE FOR DELEGATE


UIStateComponent::UIStateComponent(GameObject *owner, COMPONENT_TYPE type) :
	Component(owner, type) 
{
}


UIStateComponent::~UIStateComponent()
{
	buttons.clear();
}


void UIStateComponent::initButtonData(Button *buttonGO, std::string key, float x, float y, float z)
{
	buttonGO->initButton(key, x, y, z);
	buttons.push_back(buttonGO);
	SetCurrentButtonTo("selected");
}


void UIStateComponent::Update(unsigned int deltaTime)
{
}


void UIStateComponent::pressCurrentSelection()
{
	callbackEvent *onPressAnimationEnd = new UIStateCallback(this, 
		&UIStateComponent::onUIStateCallback);
	
	//Set current button back to idle
	SetCurrentButtonTo("pressed", onPressAnimationEnd);
}


void UIStateComponent::SetCurrentButtonTo(std::string clip) 
{
	//Set current button back to idle
	Button *b = buttons[currentButton];
	if (b)
	{
		Animator *A = static_cast<Animator*>(b->getOwner()->GetComponent(COMPONENT_TYPE::ANIMATOR));
		if (A)
			A->Play(clip);
	}
}


void UIStateComponent::SetCurrentButtonTo(std::string clip, callbackEvent *ev)
{
	//Set current button back to idle
	Button *b = buttons[currentButton];
	if (b)
	{
		Animator *A = static_cast<Animator*>(b->getOwner()->GetComponent(COMPONENT_TYPE::ANIMATOR));
		if (A)
			A->Play(clip, ev);
	}
}


void UIStateComponent::MoveSelection(int dir)
{
	SetCurrentButtonTo("idle");

	currentButton -= dir;
	if (currentButton <= -1)
		currentButton = maxButtons - 1;
	else if (currentButton >= maxButtons)
		currentButton = 0;

	SetCurrentButtonTo("selected");
}


void UIStateComponent::SetNumberOfButtons(int num)
{
	maxButtons = num;
	buttons.reserve(num);
}


Component *UIStateComponent::createNew(GameObject *owner)
{
	return new UIStateComponent(owner, COMPONENT_TYPE::UI_STATE_COMPONENT);
}


void UIStateComponent::serialize(std::fstream& stream)
{
}


void UIStateComponent::deserialize(std::fstream& stream)
{
	std::cout << "DESERIALIZING UI_STATE_COMPONENT BEGIN" << std::endl;

	currentButton = 0;

	std::cout << "DESERIALIZING UI_STATE_COMPONENT END" << std::endl;
}


void UIStateComponent::handleEvent(Event *pEvent)
{
}


void UIStateComponent::onUIStateCallback()
{
	//Get current button
	Button *b = buttons[currentButton];
	if (b)
	{
		if (b->getButtonKey() == "play") 
		{
			gamestateMgr->SetNextState(GameState::LEVEL_1);
		}
		else if (b->getButtonKey() == "controls")
		{
			gamestateMgr->SetNextState(GameState::INSTRUCTIONS);
		}
		else if (b->getButtonKey() == "credits")
		{
			gamestateMgr->SetNextState(GameState::CREDITS);
		}
		else if (b->getButtonKey() == "unpause")
		{
			gamestateMgr->TogglePause();
		}
		else if (b->getButtonKey() == "menu")
		{
			gamestateMgr->SetNextState(GameState::MAIN_MENU);
		}
		else if (b->getButtonKey() == "restart")
		{
			gamestateMgr->RestartCurrentLevel();
		}
	}
	
	//The animation is finished, change to selected
	//SetCurrentButtonTo("selected");
}