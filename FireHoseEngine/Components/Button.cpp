#include "Button.h"
#include "Transform.h"
#include "../GameObject.h"
#include "../EventManager.h"
#include "../GameStateManager.h"
#include "../Events.h"
#include <string>


Button::Button(GameObject *owner, COMPONENT_TYPE type) :
	Component(owner, type) 
{
}

Button::~Button() 
{
}

void Button::Update(unsigned int deltaTime) 
{
}


void Button::initButton(std::string key, float x, float y, float z)
{
	Transform *T = static_cast<Transform*>(getOwner()->GetComponent(COMPONENT_TYPE::TRANSFORM));
	if (T)
	{
		this->key = key;

		T->Translate(x, y, z);
	}
}


Component *Button::createNew(GameObject *owner) 
{
	return new Button(owner, COMPONENT_TYPE::BUTTON);
}


void Button::serialize(std::fstream& stream)
{
}


void Button::deserialize(std::fstream& stream)
{
}


void Button::handleEvent(Event *pEvent) 
{
}


void Button::ClickButton()
{
}


std::string Button::getButtonKey()
{
	return key;
}