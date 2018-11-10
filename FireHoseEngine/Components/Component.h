#ifndef COMPONENT_H
#define COMPONENT_H

#include <iostream>
#include <fstream>

class GameObject;

class Event;

//Temporary measure
enum COMPONENT_TYPE
{
	NONE = -1,
	TRANSFORM,
	SPRITE,
	PHYSICS,
	CONTROLLER,
	CAMERA,
	RENDERER,
	BOXCOLLIDER2D,
	RIGIDBODY2D,
	UPDOWN
};

class Component
{
public:
	Component(GameObject *owner, COMPONENT_TYPE type);
	virtual ~Component();

	virtual void Update(unsigned int deltaTime) = 0;

	GameObject *getOwner() { return this->mOwner; }
	COMPONENT_TYPE getType() { return this->mType; }

	virtual Component *createNew(GameObject *owner) = 0;
	virtual void serialize(std::fstream& stream) = 0;
	virtual void deserialize(std::fstream& stream) = 0;

	virtual void handleEvent(Event *pEvent) { }

private:
	GameObject *mOwner;
	COMPONENT_TYPE mType;
};

#endif