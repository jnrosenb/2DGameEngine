#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "Component.h"
#include "../Managers.h"

class Event;


class Controller : public Component
{
public:
	Controller(GameObject *owner, COMPONENT_TYPE type);
	~Controller();

	void Update(unsigned int deltaTime);

	virtual Component *createNew(GameObject *owner);
	virtual void serialize(std::fstream& stream);
	virtual void deserialize(std::fstream& stream);

	virtual void handleEvent(Event *pEvent);

	void toggleController();

private:
	bool active;
};

#endif