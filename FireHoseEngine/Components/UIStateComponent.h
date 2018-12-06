#ifndef UISTATECOMPONENT_H
#define UISTATECOMPONENT_H

#include "Component.h"
#include "../Managers.h"
#include "../Events.h"

class GameObject;
class Button;


class UIStateComponent : public Component
{
public:
	UIStateComponent(GameObject *owner, COMPONENT_TYPE type);
	virtual ~UIStateComponent();

	void Update(unsigned int deltaTime);

	virtual Component *createNew(GameObject *owner);
	virtual void serialize(std::fstream& stream);
	virtual void deserialize(std::fstream& stream);
	virtual void handleEvent(Event *pEvent);

	void pressCurrentSelection();
	void MoveSelection(int dir);
	void SetNumberOfButtons(int num);
	void initButtonData(Button *buttonGO, std::string key, 
		float x, float y, float z);
	void SetCurrentButtonTo(std::string clip);
	void SetCurrentButtonTo(std::string clip, callbackEvent *ev);
	void onUIStateCallback();

private:
	int maxButtons;
	int currentButton;

	std::vector<Button*> buttons;
};


class UIStateCallback : public callbackEvent
{
public:
	//AnimatorCallback() : callbackEvent()
	UIStateCallback(UIStateComponent *obj, void (UIStateComponent::*mthd)())
	{
		object = obj;
		method = mthd;
	}

	virtual ~UIStateCallback() { }

	virtual void callback()
	{
		(object->*method)();
	}

private:
	UIStateComponent *object;
	void (UIStateComponent::*method)();

private:
	UIStateCallback(UIStateCallback const& rhs);
	UIStateCallback& operator=(UIStateCallback const& rhs);
};
//*/

#endif