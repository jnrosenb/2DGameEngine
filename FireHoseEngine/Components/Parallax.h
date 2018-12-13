#ifndef PARALLAX_H
#define PARALLAX_H

#include "Component.h"
#include <fstream>
#include "../Events.h"

class GameObject;

class Parallax : public Component
{
public:
	Parallax(GameObject *owner, COMPONENT_TYPE type);
	virtual ~Parallax();

	virtual void Update(unsigned int deltaTime);

	virtual Component *createNew(GameObject *owner);
	virtual void serialize(std::fstream& stream);
	virtual void deserialize(std::fstream& stream);
	virtual void handleEvent(Event *pEvent);

	void SetScrollingSpeed(float speed);

private:
	float scrollSpeed;
	bool wraps; //TODO

	bool lockToCamera;
};

#endif