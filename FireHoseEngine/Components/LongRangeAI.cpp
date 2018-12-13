#include "../GameObject.h"
#include "Transform.h"
#include "WeaponSlot.h"
#include "Animator.h"
#include "Trigger.h"
#include "Player.h"
#include "LongRangeAI.h"


LongRangeAI::LongRangeAI(GameObject *owner, COMPONENT_TYPE type) :
	Component(owner, type)
{
}

LongRangeAI::~LongRangeAI()
{
	nodes.clear();
}

void LongRangeAI::InitInstanceState(float timeBetween, bool loops)
{
	loop = loops;
	waitBetweenNodes = timeBetween;
	nextPtr = 0;
	nextNode = GetNextPositionNode();
}

void LongRangeAI::Update(unsigned int deltaTime)
{
	if (!isEnabled())
		return;

	switch (currentState)
	{
	case LongRangeState::PATROL:
		UnawareStateUpdate(deltaTime / 1000.0f);
		break;
	case LongRangeState::IDLE:
		WaitInNode(deltaTime / 1000.0f);
		break;
	case LongRangeState::CHASE:
		ChaseStateUpdate(deltaTime / 1000.0f);
		break;
	case LongRangeState::ATTACK:
		break;
	case LongRangeState::ATTACK_COOLDOWN:
		WaitBeforeAttack(deltaTime / 1000.0f);
		break;
	case LongRangeState::JUMP_AWAY:
		JumpAway();
		break;
	case LongRangeState::DYING:
		break;
	default:
		break;
	}
}

void LongRangeAI::JumpAway() 
{
	Transform *T = static_cast<Transform*>(getOwner()->GetComponent(COMPONENT_TYPE::TRANSFORM));
	RigidBody2D *R = static_cast<RigidBody2D*>(getOwner()->GetComponent(COMPONENT_TYPE::RIGIDBODY2D));
	Transform *targetT = static_cast<Transform*>(target->GetComponent(COMPONENT_TYPE::TRANSFORM));
	if (T && R && targetT)
	{
		Vector3D currentPos = T->getPosition();
		Vector3D targetPos = targetT->getPosition();

		//Cheaty way to flip the dude right now (should use rotation maybe)
		//TODO rotate the trigger also
		Vector3D scale = T->getScale();
		int sign = targetPos.x > currentPos.x ? 1 : -1;
		T->Scale(sign * fabs(scale.x), scale.y, scale.z);

		//Jump over player in other direction
		float jumpSpeed = 0.2f;
		Vector3D jumpDir;
		Vector3DSet(&jumpDir, static_cast<float>(sign * 10), 20.0f, 0.0f);
		R->setVelocity(jumpDir);

		//TODO: make a method, since the code below is copy paste
		//TEMPORARY/////////////////////////////////////
		currentState = LongRangeState::ATTACK_COOLDOWN;
		idleWaitingTime = 0.0f;
		std::cout << "CHANGE TO ATTACK_COOLDOWN." << std::endl;

		//TODO make this prettier
		OnAnimationSwitch pEvent;
		pEvent.animTag = "idle";
		getOwner()->handleEvent(&pEvent);
		//TEMPORARY/////////////////////////////////////
	}
}

void LongRangeAI::Attack()
{
	if (!target) 
		return;

	WeaponSlot *W = static_cast<WeaponSlot*>(getOwner()->GetComponent(COMPONENT_TYPE::WEAPON_SLOT));
	Transform *T1 = static_cast<Transform*>(getOwner()->GetComponent(COMPONENT_TYPE::TRANSFORM));
	Transform *T2 = static_cast<Transform*>(target->GetComponent(COMPONENT_TYPE::TRANSFORM));
	if (W)
	{
		Vector3D dir, yOffset;
		float yoff = 0.0f;
		Vector3DSet(&yOffset, 0, yoff, 0);
		Vector3D pos1 = T1->getPosition();
		Vector3D pos2 = T2->getPosition();
		Vector3DSub(&dir, &pos2, &pos1);
		Vector3DAdd(&dir, &dir, &yOffset);
		Vector3DNormalize(&dir, &dir);
		W->Fire(dir);

		//TODO: make a method, since the code below is copy paste
		//TEMPORARY/////////////////////////////////////
		currentState = LongRangeState::ATTACK_COOLDOWN;
		idleWaitingTime = 0.0f;
		std::cout << "CHANGE TO ATTACK_COOLDOWN." << std::endl;

		//TODO make this prettier
		OnAnimationSwitch pEvent;
		pEvent.animTag = "idle";
		getOwner()->handleEvent(&pEvent);
		//TEMPORARY/////////////////////////////////////
	}
}

void LongRangeAI::UnawareStateUpdate(float dt)
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
			currentState = LongRangeState::IDLE;
			idleWaitingTime = 0.0f;
			target = 0;
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

			//std::cout << "Moving towards goal. Current pos: (" << currentPos.x << ", " << currentPos.y << "). 
			//Destination: (" << nextNode.x << ", " << nextNode.y << ")" << std::endl;
		}
	}
}

void LongRangeAI::ChaseStateUpdate(float dt)
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

		//2D squared distance between both
		float sqrDistance = Vector3DSquareDistance2D(&currentPos, &targetPos);

		//Cheaty way to flip the dude right now (should use rotation maybe)
		//TODO rotate the trigger also
		Vector3D scale = T->getScale();
		int sign = targetPos.x > currentPos.x ? 1 : -1;
		T->Scale(sign * fabs(scale.x), scale.y, scale.z);

		//Switch to ATTACK
		float jumpOverDistance = 1.0f;
		if (sqrDistance <= jumpOverDistance * jumpOverDistance)
		{
			currentState = LongRangeState::JUMP_AWAY;
			std::cout << "CHANGE TO JUMP_AWAY." << std::endl;
		}
		else if (sqrDistance <= minDistance* minDistance)
		{
			float speed = 0.75f;
			Vector3D dir;
			Vector3DSub(&dir, &targetPos, &currentPos);
			Vector3DSet(&dir, -dir.x, 0, 0);
			Vector3DNormalize(&dir, &dir);
			Vector3DScale(&dir, &dir, speed);
			R->setVelocity(dir);
		}
		else if (sqrDistance > minDistance*minDistance &&
			sqrDistance <= maxDistance* maxDistance)
		{
			Attack();
		}
		else if (sqrDistance > maxDistance*maxDistance)
		{
			currentState = LongRangeState::IDLE;
			idleWaitingTime = 0.0f;
			target = 0;
			std::cout << "CHANGE TO IDLE." << std::endl;

			//TODO make this prettier
			OnAnimationSwitch pEvent;
			pEvent.animTag = "idle";
			getOwner()->handleEvent(&pEvent);
		}
	}
}


void LongRangeAI::WaitInNode(float dt)
{
	idleWaitingTime += dt;

	if (idleWaitingTime >= waitBetweenNodes)
	{
		//Change back to unaware
		currentState = LongRangeState::PATROL;

		Transform *T = static_cast<Transform*>(getOwner()->GetComponent(COMPONENT_TYPE::TRANSFORM));
		if (T)
		{
			//Change to next target
			nextNode = GetNextPositionNode();

			Vector3D scale = T->getScale();
			int dir = nextNode.x > T->getPosition().x ? 1 : -1;
			T->Scale(dir * fabs(scale.x), scale.y, scale.z);

			//std::cout << "CHANGE TO MOVING. Dir is " << dir << std::endl;

			//TODO make this prettier
			OnAnimationSwitch pEvent;
			pEvent.animTag = "run";
			getOwner()->handleEvent(&pEvent);
		}
	}
}


void LongRangeAI::WaitBeforeAttack(float dt)
{
	idleWaitingTime += dt;

	if (idleWaitingTime >= attackCooldown)
	{
		//Change back to chase
		currentState = LongRangeState::CHASE;
		std::cout << "CHANGE TO CHASE." << std::endl;

		//TODO make this prettier
		OnAnimationSwitch pEvent;
		pEvent.animTag = "run";
		getOwner()->handleEvent(&pEvent);
	}
}


Vector3D LongRangeAI::GetNextPositionNode()
{
	Vector3D next = nodes[nextPtr];
	nextPtr = (nextPtr + 1) % nodes.size();
	return next;
}

Component *LongRangeAI::createNew(GameObject *owner)
{
	return new LongRangeAI(owner, COMPONENT_TYPE::LONG_RANGE_AI);
}

void LongRangeAI::serialize(std::fstream& stream)
{
}

void LongRangeAI::deserialize(std::fstream& stream)
{
	std::cout << "DESERIALIZING LONG_RANGE_AI BEGIN" << std::endl;

	if (stream)
	{
		setEnabled(true);

		currentState = LongRangeState::IDLE;
		//Change to idle with an initial idle time determined via serialization
		maxDistance = 20.0f; //TODO serialize
		minDistance = 5.0f; //TODO serialize
		attackCooldown = 1.0f; //TODO serialize
	}
	else
	{
		std::cout << "(LongRangeAI::deserialize)- Error reading the stream" << std::endl;
	}

	std::cout << "DESERIALIZING LONG_RANGE_AI END" << std::endl;
}

void LongRangeAI::handleEvent(Event *pEvent)
{
	if (!isEnabled())
		return;

	// TODO check following problem: If enemy was also suscribed to another trigger, 
	// then we would need to differentiate cases. Two solutions:
	//   1- Differentiate inside with an if, or something like that.
	//   2- Differentiate by making an different event from entering trigger 
	//      than from others entering into your own trigger (better).
	// (for now, just leave like this).
	if (pEvent->type == EventType::ON_ENTER_TRIGGER)
	{
		OnEnterTriggerEvent *ev = static_cast<OnEnterTriggerEvent*>(pEvent);
		if (ev && ev->isYourTrigger) 
		{

			currentState = LongRangeState::CHASE;
			target = ev->pBody->getOwner();
			std::cout << "CHANGE TO CHASE." << std::endl;

			//DEBUG, ERASE LATER////////////////////////////////////////////////////////////////////////////////
			RigidBody2D *R = static_cast<RigidBody2D*>(target->GetComponent(COMPONENT_TYPE::RIGIDBODY2D));
			if (R && R->collisionMask != CollisionMask::PLAYER)
			{
				std::cout << "SOMEONE ENTERED MY TRIGGER AND ITS NOT THE PLAYER ///////////" << std::endl;
			}
			else if (R)
			{
				std::cout << "PLAYER ENTERED MY TRIGGER ////////////////////////////////////" << std::endl;
			}
			////////////////////////////////////////////////////////////////////////////////////////////////////

			//TODO make this prettier
			OnAnimationSwitch pEvent;
			pEvent.animTag = "run";
			getOwner()->handleEvent(&pEvent);
		}
	}
}

void LongRangeAI::AddNode(Vector3D newNode)
{
	nodes.push_back(newNode);
}