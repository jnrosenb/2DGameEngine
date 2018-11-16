#include "Animator.h"
#include "../GameObject.h"


Animator::Animator(GameObject *owner, COMPONENT_TYPE type) : Component(owner, type)
{
}

Animator::~Animator()
{
}