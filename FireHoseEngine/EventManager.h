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
	TOGGLE_CONTROLLER,
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
	
	//This key will bind this event to one or more listeners.
	//When the event is fired, only suscribers who share the key will react
	//Idea is that this key is going to be taken via serialization
	std::string eventKey;
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

	//This will be used so I can serialize events and generate new ones using virtual construction system
	std::unordered_map<std::string, Event*> EventsMap;
};

#endif