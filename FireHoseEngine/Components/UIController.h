#ifndef UICONTROLLER_H
#define UICONTROLLER_H

#include "Component.h"
#include "Controller.h"
#include "../Managers.h"

class Event;


class UIController : public Controller
{
public:
	UIController(GameObject *owner, COMPONENT_TYPE type);
	~UIController();

	void Update(unsigned int deltaTime);
	virtual Component *createNew(GameObject *owner);
	virtual void deserialize(std::fstream& stream);
	virtual void handleEvent(Event *pEvent);

private:
	bool active;
};

#endif