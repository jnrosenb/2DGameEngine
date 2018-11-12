#ifndef EVENTMANAGER_H
#define EVENTMANAGER_H

#include <list>
#include <unordered_map>

class GameObject;


//ENUMERATION OF EVENT TYPES
enum class EventType
{
	COLLISIONHIT = 0,
	ON_ENTER_TRIGGER,
	ON_EXIT_TRIGGER,
	COLLIDE,
	PLAYERHIT,
	NUM
};


//EVENT CLASS
class Event
{
public:
	Event(EventType t);
	virtual ~Event();
public:
	EventType type;
	float mTimer;

	virtual void deserialize() = 0;
	virtual void serialize() = 0;
};


//EVENT MANAGER
class EventManager
{
public:
	EventManager();
	virtual ~EventManager();

	void Update(float deltaTime);

	void broadcastEvent(Event *pEvent);
	void broadcastEventToSuscribers(Event *pEvent);
	void addTimedEvent(Event *pEvent);

	void suscribe(EventType type, GameObject *pGameObject);
	void unsuscribe(EventType type, GameObject *pGameObject);

public:
	std::list<Event*> mTimedEvents;
	std::unordered_map<EventType, std::list<GameObject*> > mSuscriptions;
};

#endif