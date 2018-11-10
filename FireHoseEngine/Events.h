#ifndef EVENTS_H
#define EVENTS_H

#include "Managers.h"

class Component;


class CollisionHitEvent : public Event 
{
public:
	CollisionHitEvent(EventType type, Component *other, CollisionMask mask) : 
		Event(type), other(other), colMask(mask)
	{ }
	virtual ~CollisionHitEvent() { }

public:
	Component *other;
	CollisionMask colMask;
};

#endif