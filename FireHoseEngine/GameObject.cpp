#include <iostream>
#include "GameObject.h"
#include "Components/Component.h"
#include "Components/Transform.h"
#include "Components/controller.h"
#include "Components/Sprite.h"
#include "Components/Physics.h"
#include "Components/Camera.h"
#include "Components/Renderer.h"
#include "Components/Trigger.h"
#include "Components/RigidBody2D.h"
#include "Components/UpDown.h"

using namespace std;

GameObject::GameObject() 
{
}

GameObject::~GameObject() 
{
	//Delete all the components, then clear the vector
	for (Component *c : goComponents) 
	{
		delete c;
	}
	goComponents.clear();
}

void GameObject::Update(unsigned int deltaTime) 
{
	//Call Update in every component
	for (Component *c : goComponents)
	{
		c->Update(deltaTime);
	}
}

void GameObject::Draw()
{
	Renderer *R = static_cast<Renderer*>(this->GetComponent(COMPONENT_TYPE::RENDERER));
	if (R != 0) 
	{
		R->Draw();
	}
}

void GameObject::AddComponent(unsigned int type) 
{
	//Check first if go already has that component
	//TODO
	
	Component *newComponent;

	switch (type) 
	{
	case COMPONENT_TYPE::TRANSFORM:
		newComponent = new Transform(this, COMPONENT_TYPE::TRANSFORM);
		break;
	case COMPONENT_TYPE::CONTROLLER:
		newComponent = new Controller(this, COMPONENT_TYPE::CONTROLLER);
		break;
	case COMPONENT_TYPE::SPRITE:
		newComponent = new Sprite(this, COMPONENT_TYPE::SPRITE);
		break;
	case COMPONENT_TYPE::CAMERA:
		newComponent = new Camera(this, COMPONENT_TYPE::CAMERA);
		break;
	case COMPONENT_TYPE::RENDERER:
		newComponent = new Renderer(this, COMPONENT_TYPE::RENDERER);
		break;
	case COMPONENT_TYPE::TRIGGER:
		newComponent = new Trigger(this, COMPONENT_TYPE::TRIGGER);
		break;
	case COMPONENT_TYPE::RIGIDBODY2D:
		newComponent = new RigidBody2D(this, COMPONENT_TYPE::RIGIDBODY2D);
		break;
	case COMPONENT_TYPE::UPDOWN:
		newComponent = new UpDown(this, COMPONENT_TYPE::UPDOWN);
		break;
	}

	goComponents.push_back(newComponent);
}

void GameObject::AddComponent(Component *newC)
{
	if (!newC) 
	{
		std::cout << "(GameObject::AddComponent)- NULL REFERENCE" << std::endl;
		return;
	}

	std::cout << "Adding component to GO (TODO, SEE WHERE TO PASS SOME COMPONENTS POINTERS TO MANAGERS)" << std::endl;
	goComponents.push_back(newC);
}

Component *GameObject::GetComponent(unsigned int type) 
{
	//Call Update in every component
	for (Component *c : goComponents)
	{
		if (c->getType() == type) 
		{
			return c;
		}
	}
	return 0;
}

//event handler from go to components
void GameObject::handleEvent(Event *pEvent)
{
	for (Component *c : goComponents) 
	{
		//What if I have components that
		//I don't wanna send events to?
		c->handleEvent(pEvent);
	}
}

//Experiment
void GameObject::addEventKey(EventType type, std::string key)
{
	//TODO*************
	//Check that this is the right way to do this

	std::vector<std::string> keys = EventTypeKeyMap[type];

	keys.push_back(key);

	EventTypeKeyMap[type] = keys;//IUGH

}

//Experiment
std::vector<std::string> const& GameObject::getEventKeys(EventType type)
{
	return EventTypeKeyMap[type];
}

// Passes down the flag to all components
// (overwrites individual enabled state of components)
void GameObject::setEnabled(bool flag)
{
	enabled = flag;

	for (Component *c : goComponents)
	{
		c->setEnabled(flag);
	}
}

// returns if this go is enabled or disabled
bool GameObject::isEnabled()
{
	return enabled;
}