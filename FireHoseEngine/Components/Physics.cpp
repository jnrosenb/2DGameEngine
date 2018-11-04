#include "Physics.h"


Physics::Physics(GameObject *owner, COMPONENT_TYPE type) :
	Component(owner, type)
{
}

Physics::~Physics()
{
}

void Physics::Update(unsigned int deltaTime)
{
}

Component *Physics::createNew(GameObject *owner)
{
	return new Physics(owner, COMPONENT_TYPE::PHYSICS);
}

void Physics::serialize(std::fstream& stream)
{
}

void Physics::deserialize(std::fstream& stream)
{
}