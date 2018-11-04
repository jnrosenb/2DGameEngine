#include "UpDown.h"
#include "Transform.h"
#include "../GameObject.h"
#include <iostream>
#include <cstdlib>

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
	float speed = 1.0f;
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
	}
	else 
	{
		std::cout << "(UpDown::deserialize)- Error reading the stream" << std::endl;
	}

	std::cout << "DESERIALIZING UPDOWN END" << std::endl;
}