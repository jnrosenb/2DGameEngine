#ifndef EVENTS_H
#define EVENTS_H

#include "Managers.h"

class Component;

///////////////////////////////////////////////////////////////////
////// <<--IMPORTANT-->>									///////
////// Remember to update the EventManager EventsMap		///////
////// hash table when adding new Events types here			///////
///////////////////////////////////////////////////////////////////

class OnCollisionHitEvent : public Event 
{
public:
	OnCollisionHitEvent(Component *other, CollisionMask mask) : 
		Event(EventType::COLLISIONHIT), other(other), colMask(mask)
	{ }
	virtual ~OnCollisionHitEvent() { }

public:
	Component *other;
	CollisionMask colMask;
};


class OnEnterTriggerEvent : public Event
{
public:
	OnEnterTriggerEvent(Component *other) :
		Event(EventType::ON_ENTER_TRIGGER), other(other)
	{ }
	virtual ~OnEnterTriggerEvent() { }

public:
	Component *other;
};


class OnExitTriggerEvent : public Event
{
public:
	OnExitTriggerEvent(Component *other) :
		Event(EventType::ON_EXIT_TRIGGER), other(other)
	{ }
	virtual ~OnExitTriggerEvent() { }

public:
	Component *other;
};


class OnToggleControllerEvent : public Event
{
public:
	OnToggleControllerEvent() :
		Event(EventType::TOGGLE_CONTROLLER)
	{ }
	virtual ~OnToggleControllerEvent() { }
};


#endif