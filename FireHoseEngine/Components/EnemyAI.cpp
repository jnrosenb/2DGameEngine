#include "../GameObject.h"
#include "Transform.h"
#include "Animator.h"
#include "EnemyAI.h"
#include "Player.h"


EnemyAI::EnemyAI(GameObject *owner, COMPONENT_TYPE type) : 
	Component(owner, type)
{
}

EnemyAI::~EnemyAI() 
{
	nodes.clear();
}

void EnemyAI::InitInstanceState(float timeBetween, bool loops)
{
	loop = loops;
	waitBetweenNodes = timeBetween;
	nextPtr = 0;
	nextNode = GetNextPositionNode();
}

void EnemyAI::Update(unsigned int deltaTime)
{
	if (!isEnabled())
		return;

	switch (currentState) 
	{
	case EnemyState::PATROL:
		UnawareStateUpdate(deltaTime / 1000.0f);
		break;
	case EnemyState::IDLE:
		WaitInNode(deltaTime / 1000.0f);
		break;
	case EnemyState::CHASE:
		ChaseStateUpdate(deltaTime / 1000.0f);
		break;
	case EnemyState::ATTACK:
		break;
	case EnemyState::ATTACK_COOLDOWN:
		WaitBeforeAttack(deltaTime / 1000.0f);
		break;
	case EnemyState::DYING:
		break;
	default:
		break;
	}
}

void EnemyAI::Attack()
{
	Animator *A = static_cast<Animator*>(getOwner()->GetComponent(COMPONENT_TYPE::ANIMATOR));
	if (A)
	{
		callbackEvent *onHitAnimationEnd = new EnemyAICallback(this, &EnemyAI::onHitAnimationEnd);
		A->Play("hit", onHitAnimationEnd);
	}
}

void EnemyAI::UnawareStateUpdate(float dt)
{
	RigidBody2D *R = static_cast<RigidBody2D*>(getOwner()->GetComponent(COMPONENT_TYPE::RIGIDBODY2D));
	Transform *T = static_cast<Transform*>(getOwner()->GetComponent(COMPONENT_TYPE::TRANSFORM));
	if (R && T) 
	{
		Vector3D currentPos = T->getPosition();
		
		//TODO change for smarter approach
		float epsilon = 0.5f;
		//if (Vector3DSquareDistance2D(&currentPos, &nextNode) < epsilon * epsilon) 
		if (fabs(currentPos.x - nextNode.x) < epsilon) 
		{
			currentState = EnemyState::IDLE;
			idleWaitingTime = 0.0f;
			//std::cout << "CHANGE TO IDLE." << std::endl;

			//TODO make this prettier
			OnAnimationSwitch pEvent;
			pEvent.animTag = "idle";
			getOwner()->handleEvent(&pEvent);
		}
		else 
		{
			//TODO Fix all below!!!!
			//Move in that direction
			float speed = 0.125f;
			Vector3D dir;
			Vector3DSub(&dir, &nextNode, &currentPos);
			Vector3DSet(&dir, dir.x, 0, 0);
			Vector3DNormalize(&dir, &dir);
			Vector3DScale(&dir, &dir, speed);
			R->setVelocity(dir);
		}
	}
}

void EnemyAI::ChaseStateUpdate(float dt)
{
	Transform *T = static_cast<Transform*>(getOwner()->GetComponent(COMPONENT_TYPE::TRANSFORM));
	RigidBody2D *R = static_cast<RigidBody2D*>(getOwner()->GetComponent(COMPONENT_TYPE::RIGIDBODY2D));
	Transform *targetT = static_cast<Transform*>(target->GetComponent(COMPONENT_TYPE::TRANSFORM));
	Player *targetPlayer = static_cast<Player*>(target->GetComponent(COMPONENT_TYPE::CHARACTER));
	if (targetPlayer && T && R && targetT)
	{
		if (targetPlayer->IsDead())
			return;

		Vector3D currentPos = T->getPosition();
		Vector3D targetPos = targetT->getPosition();

		//Switch to IDLE
		if (fabs(currentPos.x - targetPos.x) > maxDistance || 
			fabs(currentPos.y - targetPos.y) > maxDistance)
		{
			currentState = EnemyState::IDLE;
			idleWaitingTime = 0.0f;
			std::cout << "CHANGE TO IDLE." << std::endl;

			//TODO make this prettier
			OnAnimationSwitch pEvent;
			pEvent.animTag = "idle";
			getOwner()->handleEvent(&pEvent);
		}

		//Switch to ATTACK
		if (fabs(currentPos.x - targetPos.x) < minDistance)
		{
			currentState = EnemyState::ATTACK; 
			Attack();
			std::cout << "CHANGE TO ATTACK." << std::endl;

			R->ResetKinematics();

			//Here you have two options: Still let the enemy 
			//advance towards you, or not. Lets test first one.
			//return; //if not
		}

		//Cheaty way to flip the dude right now (should use rotation maybe)
		//TODO rotate the trigger also
		Vector3D scale = T->getScale();
		int sign = targetPos.x > currentPos.x ? 1 : -1;
		T->Scale(sign * fabs(scale.x), scale.y, scale.z);

		float speed = 0.2f;
		Vector3D dir;
		Vector3DSub(&dir, &targetPos, &currentPos);
		Vector3DSet(&dir, dir.x, 0, 0);
		Vector3DNormalize(&dir, &dir);
		Vector3DScale(&dir, &dir, speed);
		R->setVelocity(dir);
	}
}


void EnemyAI::WaitInNode(float dt)
{
	idleWaitingTime += dt;

	if (idleWaitingTime >= waitBetweenNodes) 
	{
		//Change back to unaware
		currentState = EnemyState::PATROL;
		
		Transform *T = static_cast<Transform*>(getOwner()->GetComponent(COMPONENT_TYPE::TRANSFORM));
		if (T)
		{
			//Change to next target
			nextNode = GetNextPositionNode();

			Vector3D scale = T->getScale();
			int dir = nextNode.x > T->getPosition().x  ? 1 : -1;
			T->Scale(dir * fabs(scale.x), scale.y, scale.z);

			//std::cout << "CHANGE TO MOVING. Dir is " << dir << std::endl;

			//TODO make this prettier
			OnAnimationSwitch pEvent;
			pEvent.animTag = "run";
			getOwner()->handleEvent(&pEvent);
		}
	}
}


void EnemyAI::WaitBeforeAttack(float dt)
{
	idleWaitingTime += dt;

	if (idleWaitingTime >= attackCooldown)
	{
		//Change back to chase
		currentState = EnemyState::CHASE;
		std::cout << "CHANGE TO CHASE." << std::endl;

		//TODO make this prettier
		OnAnimationSwitch pEvent;
		pEvent.animTag = "run";
		getOwner()->handleEvent(&pEvent);
	}
}


Vector3D EnemyAI::GetNextPositionNode() 
{
	Vector3D next = nodes[nextPtr];
	nextPtr = (nextPtr + 1) % nodes.size();
	return next;
}

Component *EnemyAI::createNew(GameObject *owner) 
{
	return new EnemyAI(owner, COMPONENT_TYPE::ENEMY_AI);
}

void EnemyAI::serialize(std::fstream& stream) 
{
}

void EnemyAI::deserialize(std::fstream& stream) 
{
	std::cout << "DESERIALIZING ENEMY_AI BEGIN" << std::endl;

	if (stream)
	{
		setEnabled(true);

		currentState = EnemyState::IDLE;
		//Change to idle with an initial idle time determined via serialization
		maxDistance = 8.0f; //TODO serialize
		minDistance = 1.0f; //TODO serialize
		attackCooldown = 0.5; //TODO serialize
	}
	else
	{
		std::cout << "(EnemyAI::deserialize)- Error reading the stream" << std::endl;
	}

	std::cout << "DESERIALIZING ENEMY_AI END" << std::endl;
}

void EnemyAI::handleEvent(Event *pEvent) 
{
	if (!isEnabled())
		return;

	if (pEvent->type == EventType::ON_ENTER_TRIGGER) 
	{
		// TODO check following problem: If enemy was also suscribed to another trigger, 
		// then we would need to differentiate cases. Two solutions:
		//   1- Differentiate inside with an if, or something like that.
		//   2- Differentiate by making an different event from entering trigger 
		//      than from others entering into your own trigger (better).
		// (for now, just leave like this).

		OnEnterTriggerEvent *ev = static_cast<OnEnterTriggerEvent*>(pEvent);
		if (ev && ev->isYourTrigger)
		{
			std::cout << "SOMEONE ENTERED MY TRIGGER!!!" << std::endl;
			currentState = EnemyState::CHASE;
			target = ev->pBody->getOwner();
			std::cout << "CHANGE TO CHASE." << std::endl;

			//TODO make this prettier
			OnAnimationSwitch pEvent;
			pEvent.animTag = "run";
			getOwner()->handleEvent(&pEvent);
		}
	}
}

void EnemyAI::AddNode(Vector3D newNode) 
{
	nodes.push_back(newNode);
}

void EnemyAI::onHitAnimationEnd() 
{
	//TODO temporary, just for visual fun
	//If player is near and in front, launch in other direction
	Transform *T = static_cast<Transform*>(getOwner()->GetComponent(COMPONENT_TYPE::TRANSFORM));
	RigidBody2D *targetR = static_cast<RigidBody2D*>(target->GetComponent(COMPONENT_TYPE::RIGIDBODY2D));
	Transform   *targetT = static_cast<Transform*>  (target->GetComponent(COMPONENT_TYPE::TRANSFORM));
	if (T && targetR && targetT)
	{
		Vector3D targetPos = targetT->getPosition();
		Vector3D currentPos = T->getPosition();
		int sign = targetPos.x > currentPos.x ? 1 : -1;

		if (Vector3DSquareDistance2D(&targetPos, &currentPos) < (minDistance + 0.5f) * (minDistance + 0.5f))
		{
			Vector3D launchVel;
			Vector3DSet(&launchVel, sign * 8.0f, 4.5f, 0.0f);
			Player *player = static_cast<Player*>(target->GetComponent(COMPONENT_TYPE::CHARACTER));
			if (player)
			{
				player->TakeDamage(launchVel);
			}
		}
	}

	//TODO: make a method, since the code below is copy paste
	//TEMPORARY/////////////////////////////////////
	currentState = EnemyState::ATTACK_COOLDOWN;
	idleWaitingTime = 0.0f;
	std::cout << "CHANGE TO ATTACK_COOLDOWN." << std::endl;

	//TODO make this prettier
	OnAnimationSwitch pEvent;
	pEvent.animTag = "idle";
	getOwner()->handleEvent(&pEvent);
	//TEMPORARY/////////////////////////////////////
}