#ifndef ANIMATOR_H
#define ANIMATOR_H

#include "../AnimationClip.h"
#include "Component.h"
#include <unordered_map>
#include "../Events.h"

class GameObject;

class Animator: public Component
{
public:
	Animator(GameObject *owner, COMPONENT_TYPE type);
	virtual ~Animator();

	virtual void Update(unsigned int deltaTime);

	virtual Component *createNew(GameObject *owner);
	virtual void serialize(std::fstream& stream);
	virtual void deserialize(std::fstream& stream);
	virtual void handleEvent(Event *pEvent);

	AnimationClip *getCurrentClip();
	std::string getCurrentAnimationTag();
	void Play(std::string animation);

	//Experiment
	void Play(std::string animation, callbackEvent *ev);

	//Experiment
	void onAnimationEndCallback();

private:
	std::unordered_map<std::string, AnimationClip*> clips;
	std::string currentAnimation;
};


class AnimatorCallback : public callbackEvent
{
public:
	//AnimatorCallback() : callbackEvent()
	AnimatorCallback(Animator *obj, void (Animator::*mthd)())
	{
		object = obj;
		method = mthd;
	}

	virtual ~AnimatorCallback() { }

	virtual void callback()
	{
		(object->*method)();
	}

private:
	Animator *object;
	void (Animator::*method)();

private:
	AnimatorCallback(AnimatorCallback const& rhs);
	AnimatorCallback& operator=(AnimatorCallback const& rhs);
};
//*/

#endif