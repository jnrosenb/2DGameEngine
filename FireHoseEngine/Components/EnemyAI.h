#ifndef ENEMYAI_H
#define ENEMYAI_H

#include "Component.h"
#include <vector>
#include "../Math/Vector3D.h"
#include <fstream>

class GameObject;
class Event;

enum class EnemyState 
{
	PATROL,
	IDLE,
	CHASE,
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
	void AddNode(Vector3D newNode);

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
};

#endif