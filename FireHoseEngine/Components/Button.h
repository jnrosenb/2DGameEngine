#ifndef BUTTON_H
#define BUTTON_H

#include "Component.h"
#include "../Managers.h"

class Event;
class GameObject;


class Button : public Component
{
public:
	Button(GameObject *owner, COMPONENT_TYPE type);
	virtual ~Button();

	void Update(unsigned int deltaTime);

	virtual Component *createNew(GameObject *owner);
	virtual void serialize(std::fstream& stream);
	virtual void deserialize(std::fstream& stream);
	virtual void handleEvent(Event *pEvent);

	void initButton(std::string key, float x, float y, float z);
	void ClickButton();
	std::string getButtonKey();

private:
	std::string key;
};

#endif