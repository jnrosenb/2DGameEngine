#ifndef ANIMATOR_H
#define ANIMATOR_H

#include "Component.h"

class GameObject;

class Animator: public Component
{
public:
	Animator(GameObject *owner, COMPONENT_TYPE type);
	virtual ~Animator();



private:
	int fps;
	int maxFrames;
	int currentAnimation;
};

#endif