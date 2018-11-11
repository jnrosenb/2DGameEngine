#ifndef TRIGGER_H
#define TRIGGER_H

#include "Component.h"
#include <list>

class Shape;
class GameObject;
class RigidBody2D;

class Trigger : public Component 
{
public:
	Trigger(GameObject *owner, COMPONENT_TYPE type);
	virtual ~Trigger();

	void Update(unsigned int deltaTime);

	virtual Component *createNew(GameObject *owner);
	virtual void serialize(std::fstream& stream);
	virtual void deserialize(std::fstream& stream);

	bool isInTrigger(RigidBody2D *obj);
	void addToTrigger(RigidBody2D *obj); 
	void removeFromTrigger(RigidBody2D *obj);

	Shape *GetShape();

private:
	Shape *shape;

	std::list<RigidBody2D*> insideTrigger;
};

#endif