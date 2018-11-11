#ifndef TRIGGER_H
#define TRIGGER_H

#include "Component.h"

class GameObject;

class Trigger : public Component 
{
public:
	Trigger(GameObject *owner, COMPONENT_TYPE type);
	~Trigger();

	void Update(unsigned int deltaTime);

	virtual Component *createNew(GameObject *owner);
	virtual void serialize(std::fstream& stream);
	virtual void deserialize(std::fstream& stream);
};

#endif