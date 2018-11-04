#ifndef PHYSICS_H
#define PHYSICS_H

#include "Component.h"

class Physics : public Component
{
public:
	Physics(GameObject *owner, COMPONENT_TYPE type);
	~Physics();

	void Update(unsigned int deltaTime);

	virtual Component *createNew(GameObject *owner);
	virtual void serialize(std::fstream& stream);
	virtual void deserialize(std::fstream& stream);

private:
};

#endif