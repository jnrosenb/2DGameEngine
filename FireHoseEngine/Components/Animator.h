#ifndef ANIMATOR_H
#define ANIMATOR_H

#include "../AnimationClip.h"
#include "Component.h"
#include <unordered_map>

class GameObject;
class Event;

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

private:
	std::unordered_map<std::string, AnimationClip*> clips;
	std::string currentAnimation;
};

#endif