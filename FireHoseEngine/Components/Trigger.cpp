#include "Trigger.h"


Trigger::Trigger(GameObject *owner, COMPONENT_TYPE type) : Component(owner, type) 
{
}

Trigger::~Trigger() 
{
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
}