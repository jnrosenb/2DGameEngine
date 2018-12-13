#ifndef LONGRANGEAI_H
#define LONGRANGEAI_H

#include "Component.h"
#include <vector>
#include "../Math/Vector3D.h"
#include <fstream>
#include "../Events.h"

class GameObject;

enum class LongRangeState
{
	PATROL,
	IDLE,
	CHASE,
	ATTACK,
	ATTACK_COOLDOWN,
	JUMP_AWAY,
	DYING,
	NUM
};

class LongRangeAI : public Component
{
public:
	LongRangeAI(GameObject *owner, COMPONENT_TYPE type);
	virtual ~LongRangeAI();

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
	void JumpAway();

	//DELEGATES
	void onHitAnimationEnd();

private:
	Vector3D GetNextPositionNode();

private:
	LongRangeState currentState;
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

	//CHASE_WEAPON state variables
	GameObject *targetWeapon;

	//Attack and COOLDOWN state vars
	float attackCooldown;
};

#endif