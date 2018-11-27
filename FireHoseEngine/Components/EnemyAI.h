#ifndef ENEMYAI_H
#define ENEMYAI_H

#include "Component.h"
#include <vector>
#include "../Math/Vector3D.h"
#include <fstream>
#include "../Events.h"

class GameObject;

enum class EnemyState 
{
	PATROL,
	IDLE,
	CHASE,
	ATTACK,
	ATTACK_COOLDOWN,
	NUM
};

class EnemyAI : public Component
{
public:
	EnemyAI(GameObject *owner, COMPONENT_TYPE type);
	virtual ~EnemyAI();

	virtual void Update(unsigned int deltaTime);

	virtual Component *createNew(GameObject *owner);
	virtual void serialize(std::fstream& stream);
	virtual void deserialize(std::fstream& stream);
	virtual void handleEvent(Event *pEvent);

	void InitInstanceState(float timeBetween, bool loops);
	void UnawareStateUpdate(float dt);
	void ChaseStateUpdate(float dt);
	void WaitInNode(float dt);
	void WaitBeforeAttack(float dt);
	void Attack();
	void AddNode(Vector3D newNode);
	
	//DELEGATES
	void onHitAnimationEnd();

private:
	Vector3D GetNextPositionNode();

private:
	EnemyState currentState;
	Vector3D nextNode;
	int nextPtr;
	float idleWaitingTime;

	//PATROL and IDLE state variables
	std::vector<Vector3D> nodes;
	float waitBetweenNodes;
	bool loop;

	//CHASE state variables
	GameObject *target;
	float maxDistance;
	float minDistance;

	//Attack and COOLDOWN state vars
	float attackCooldown;
};


class EnemyAICallback : public callbackEvent 
{
public:
	//AnimatorCallback() : callbackEvent()
	EnemyAICallback(EnemyAI *obj, void (EnemyAI::*mthd)())
	{
		object = obj;
		method = mthd;
	}

	virtual ~EnemyAICallback() { }

	virtual void callback()
	{
		(object->*method)();
	}

private:
	EnemyAI *object;
	void (EnemyAI::*method)();

private:
	EnemyAICallback(EnemyAICallback const& rhs);
	EnemyAICallback& operator=(EnemyAICallback const& rhs);
};

#endif