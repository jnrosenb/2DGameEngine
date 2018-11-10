#ifndef UPDOWN_H
#define UPDOWN_H

#include "Component.h"
#include "../Math/Vector3D.h"

class Event;

class UpDown : public Component
{
public:
	UpDown(GameObject *owner, COMPONENT_TYPE type);;
	~UpDown();

	void Update(unsigned int deltaTime);

	virtual Component *createNew(GameObject *owner);
	virtual void serialize(std::fstream& stream);
	virtual void deserialize(std::fstream& stream);

	virtual void handleEvent(Event *pEvent);

private:
	int dir;
	float maxDist;
	float distanceDone;
};

#endif