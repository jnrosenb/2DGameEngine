#include "Trigger.h"
#include "../Managers.h"
#include <string>
#include "RigidBody2D.h"

extern Manager *pManager;


Trigger::Trigger(GameObject *owner, COMPONENT_TYPE type) : Component(owner, type) 
{
}

Trigger::~Trigger() 
{
}

Shape *Trigger::GetShape() 
{
	return shape;
}

//O(N), later see if change to hash or something
bool Trigger::isInTrigger(RigidBody2D *obj)
{
	for (RigidBody2D const *r : insideTrigger) 
	{
		if (r == obj)
			return true;
	}
	return false;
}

//for now, this assumes the check to see 
//if it was inside was done externally
void Trigger::addToTrigger(RigidBody2D *obj)
{
	insideTrigger.push_back(obj);
}

//CHECK THIS URGENTLY
void Trigger::removeFromTrigger(RigidBody2D *obj) 
{
	for (auto r : insideTrigger)
	{
		if (r == obj) 
		{
			insideTrigger.remove(r);
			return;
		}
	}
}

void Trigger::Update(unsigned int deltaTime)
{
}

Component *Trigger::createNew(GameObject *owner) 
{
	return new Trigger(owner, COMPONENT_TYPE::TRIGGER);
}

void Trigger::serialize(std::fstream& stream) 
{
}

void Trigger::deserialize(std::fstream& stream) 
{
	std::cout << "DESERIALIZING TRIGGER BEGIN" << std::endl;

	std::string shapeType;
	if (stream >> shapeType)
	{
		if (shapeType == "RectangleShape")
		{
			float w, h;
			if (stream >> w >> h)
			{
				shape = new RectangleShape(this, ShapeType::RECTANGLE);
				RectangleShape *rect = static_cast<RectangleShape*>(shape);
				rect->setSize(w, h);
			}
			else
			{
				std::cout << "(Trigger::deserialize)- Error 2.1 reading from stream" << std::endl;
			}
		}
		else if (shapeType == "CircleShape")
		{
			float r;
			if (stream >> r)
			{
				CircleShape *circle = new CircleShape(this, ShapeType::CIRCLE);
				circle->setRadius(r);
				shape = circle;
			}
			else
			{
				std::cout << "(Trigger::deserialize)- Error 2.2 reading from stream" << std::endl;
			}
		}

		//Add this component to list managed by collisionManager
		pManager->GetCollisionManager()->addTrigger(this);
	}
	else
	{
		std::cout << "(Trigger::deserialize)- Error reading the stream" << std::endl;
	}

	std::cout << "DESERIALIZING TRIGGER END" << std::endl;
}