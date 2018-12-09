#ifndef CHARACTER_H
#define CHARACTER_H

#include "Component.h"
#include "../Managers.h"

class Event;
class GameObject;


class Character : public Component
{
public:
	Character(GameObject *owner, COMPONENT_TYPE type) :
		Component(owner, type) 
	{ }

	virtual ~Character() 
	{ }

	virtual void Update(unsigned int deltaTime) = 0;
	virtual Component *createNew(GameObject *owner) = 0;
	virtual void serialize(std::fstream& stream) = 0;
	virtual void deserialize(std::fstream& stream) = 0;
	virtual void handleEvent(Event *pEvent) = 0;

	virtual void TakeDamage(int damage = 1) = 0;
	virtual void TakeDamage(Vector3D launchDir, int damage = 1) = 0;

private:
};

#endif