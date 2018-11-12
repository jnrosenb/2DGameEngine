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

	virtual void deserialize() {}
	virtual void serialize() {}

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

	virtual void deserialize() {}
	virtual void serialize() {}

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

	virtual void deserialize() {}
	virtual void serialize() {}

public:
	Component *other;
};


class TestBroadcastEvent : public Event
{
public:
	TestBroadcastEvent() :
		Event(EventType::PLAYERHIT)
	{ }
	virtual ~TestBroadcastEvent() { }

	virtual void deserialize() {}
	virtual void serialize() {}
};


#endif