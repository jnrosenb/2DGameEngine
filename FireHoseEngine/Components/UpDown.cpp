#include "UpDown.h"
#include "Transform.h"
#include "../GameObject.h"
#include <iostream>
#include <cstdlib>
#include "../Managers.h"
#include <unordered_map>

extern Manager *pManager;

using namespace std;


UpDown::UpDown(GameObject *owner, COMPONENT_TYPE type) : 
	Component(owner, type)
{
}

UpDown::~UpDown() 
{
}

void UpDown::Update(unsigned int deltaTime) 
{
	if (!activated)
		return;

	float speed = 2.0f;
	float dt = deltaTime/1000.f;

	distanceDone += speed * dt * dir;

	Transform *T = static_cast<Transform*>(getOwner()->GetComponent(COMPONENT_TYPE::TRANSFORM));
	if (T != 0) 
	{
		T->Translate(0, speed * dt * dir, 0);

		if (abs(distanceDone) >= maxDist) 
		{
			dir = -dir;
			distanceDone = 0.0f;
		}
	}
}

void UpDown::toggleActive()
{
	activated = !activated;
}

Component *UpDown::createNew(GameObject *owner)
{
	return new UpDown(owner, COMPONENT_TYPE::UPDOWN);
}

void UpDown::serialize(std::fstream& stream)
{
}

void UpDown::deserialize(std::fstream& stream)
{
	std::cout << "DESERIALIZING UPDOWN BEGIN" << std::endl;

	int direction; 
	float maxD;
	if (stream >> maxD >> direction)
	{
		maxDist = maxD;
		dir = direction;
		activated = false;

		///Suscription to events (should be serialized)
		//pManager->GetEventManager()->suscribe(EventType::ON_ENTER_TRIGGER, this->getOwner());
		//pManager->GetEventManager()->suscribe(EventType::ON_EXIT_TRIGGER, this->getOwner());
	}
	else 
	{
		std::cout << "(UpDown::deserialize)- Error reading the stream" << std::endl;
	}

	std::cout << "DESERIALIZING UPDOWN END" << std::endl;
}


void UpDown::SetDir(int dir) 
{
	this->dir = dir;
}


void UpDown::handleEvent(Event *pEvent)
{
	if (pEvent->type == EventType::ON_ENTER_TRIGGER)
	{
		std::vector<std::string> eventKeys = this->getOwner()->getEventKeys(pEvent->type);
		for (std::string key : eventKeys) 
		{
			if (pEvent->eventKey == "LavaRoomSwitchWall") 
			{
				std::cout << "" << std::endl;
			}
			if (key == pEvent->eventKey) 
			{
				toggleActive();
				break;
			}
		}
	}
	if (pEvent->type == EventType::ON_EXIT_TRIGGER)
	{
		toggleActive();
	}
}