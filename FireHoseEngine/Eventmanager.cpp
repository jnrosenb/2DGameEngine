#include "EventManager.h"
#include "Events.h"
#include "GameObject.h"


///////////////////////////////////////////////////////////////////
//////  EVENT												///////
///////////////////////////////////////////////////////////////////
Event::Event(EventType t) : type(t), mTimer(0)
{
}

Event::~Event()
{
}


///////////////////////////////////////////////////////////////////
//////  EVENT MANAGER										///////
///////////////////////////////////////////////////////////////////
#include "Managers.h"
extern Manager *pManager;

EventManager::EventManager() 
{
	//EventsMap["OnCollisionHit"] = new OnCollisionHitEvent(0, CollisionMask::NUM);
	//EventsMap["OnEnterTrigger"] = new OnEnterTriggerEvent(0);
	//EventsMap["OnExitTrigger"] = new OnExitTriggerEvent(0);
	//EventsMap["OnTestBroadcast"] = new OnTestBroadcastEvent();
}

EventManager::~EventManager() 
{
	CleanUpEvents();
}

void EventManager::CleanUpEvents() 
{
	for (Event *ev : mTimedEvents)
	{
		delete ev;
	}
	mTimedEvents.clear();

	for (auto node : mSuscriptions)
	{
		node.second.clear();
	}
	mSuscriptions.clear();
	
	/*for (auto node : EventsMap)
	{
		delete node.second;
	}
	EventsMap.clear();//*/
}

void EventManager::Update(float deltaTime)
{
	//Loop through all timed events
	//Decrese timer by delta time
	//if its less or equal than 0 -> fire event
	std::list<Event*>::iterator it, itTemp;
	it = mTimedEvents.begin();
	while (it != mTimedEvents.end())
	{
		Event * pEvent = *it;
		pEvent->mTimer -= deltaTime;
		if (pEvent->mTimer <= 0.0f) 
		{
			broadcastEvent(pEvent);
			delete pEvent; //content inside iterator
			itTemp = it;
			++itTemp;
			mTimedEvents.erase(it); //Doesnt delete whats inside iterator
			it = itTemp;
		}
		else 
		{
			++it;
		}
	}
}

void EventManager::broadcastEvent(Event *pEvent) 
{
	for (auto go : pManager->GetGameObjMgr()->getGoList()) 
	{
		go->handleEvent(pEvent);
	}
}

void EventManager::addTimedEvent(Event *pEvent) 
{
	mTimedEvents.push_back(pEvent);
}

void EventManager::broadcastEventToSuscribers(Event *pEvent)
{
	std::list<GameObject*> &listOfSubscribers = mSuscriptions[pEvent->type];
	for (auto pGo : listOfSubscribers)
	{
		pGo->handleEvent(pEvent);
	}
}

void EventManager::suscribe(EventType type, GameObject *pGameObject)
{
	std::list<GameObject*> &listOfSubscribers = mSuscriptions[type];
	
	for (auto pGo : listOfSubscribers) 
	{
		if (pGo == pGameObject) 
			return;
	}

	listOfSubscribers.push_back(pGameObject);
}


void EventManager::unsuscribe(EventType type, GameObject *pGameObject)
{
	std::list<GameObject*> &listOfSubscribers = mSuscriptions[type];

	auto it = std::find(listOfSubscribers.begin(), listOfSubscribers.end(), pGameObject);

	if (it == listOfSubscribers.end())
		return;

	listOfSubscribers.erase(it);
}