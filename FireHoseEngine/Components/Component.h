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
	ANIMATOR,
	SPRITE,
	CONTROLLER,
	CAMERA,
	RENDERER,
	TRIGGER,
	RIGIDBODY2D,
	WEAPON,
	WEAPON_SLOT,
	PROJECTILE,
	ENEMY_AI,
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

	bool isEnabled();
	void setEnabled(bool flag);

private:
	bool enabled;
	GameObject *mOwner;
	COMPONENT_TYPE mType;
};

#endif