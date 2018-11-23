#include "RigidBody2D.h"
#include "../Managers.h"
#include "Transform.h"
#include "../GameObject.h"
#include <string>
#include <iostream>
#include "../Events.h"

extern Manager *pManager;

RigidBody2D::RigidBody2D(GameObject *owner, COMPONENT_TYPE type) : 
	Component(owner, type)
{
}

RigidBody2D::~RigidBody2D()
{
}

void RigidBody2D::Update(unsigned int deltaTime)
{
	//This will be called by the GO, but wont be used for now
	if (shape != 0) 
	{
		shape->update();
	}

	grounded = 0; //GROUND AST CORRECTION FLAG
}

void RigidBody2D::AddForce(Vector3D force)
{
	Vector3DAdd(&mForce, &mForce, &force);
}

void RigidBody2D::setVelocity(Vector3D vel)
{
	Vector3DAdd(&mVelocity, &mVelocity, &vel);
}

bool RigidBody2D::isDynamic() 
{
	return dynamic;
}

void RigidBody2D::LateUpdate(float deltaTime)
{
	if (!isDynamic())
	{
		/*FOR NOW, THIS IS SO NO FORCE IS APPLIED 
		TO NON DYNAMIC OBJECTS*/
		return;
	}

	//Get current position from transform
	Transform *T = static_cast<Transform*>(getOwner()->GetComponent(COMPONENT_TYPE::TRANSFORM));
	if (T == 0)
	{
		std::cout << "ERROR - No transform for rigidBody calculation" << std::endl;
		return;
	}
	Vector3DSet(&mPos, T->getPosition().x, T->getPosition().y, T->getPosition().z);

	Vector3DScale(&mAcceleration, &mForce, massInv);

	Vector3D deltaVel;
	Vector3DScale(&deltaVel, &mAcceleration, deltaTime);
	Vector3DAdd(&mVelocity, &mVelocity, &deltaVel);

	//TEMPORAL VELOCITY CAP
	float p = 0.05f;
	Vector3D AirResistance;
	Vector3DSet(&AirResistance, -mVelocity.x * p, -mVelocity.y * p, -mVelocity.z * p);
	Vector3DAdd(&mVelocity, &mVelocity, &AirResistance);

	Vector3D deltaPos;
	Vector3DScale(&deltaPos, &mVelocity, deltaTime);
	Vector3DAdd(&mPos, &mPos, &deltaPos); //We get the Position
	
	//Send animation events
	if (fabs(mVelocity.x) > 1.0f )
	{
		OnAnimationSwitch pEvent;
		pEvent.animTag = "run";
		getOwner()->handleEvent(&pEvent);
	}
	else if (fabs(mVelocity.x) <= 1.0f) 
	{
		OnAnimationSwitch pEvent;
		pEvent.animTag = "idle";
		getOwner()->handleEvent(&pEvent);
	}
	if (jumping)
	{
		OnAnimationSwitch pEvent;
		pEvent.animTag = "jump";
		getOwner()->handleEvent(&pEvent);
	}

	//reset forces to zero
	Vector3DSet(&mForce, 0, 0, 0);

	//With the position, we set the transform for next frame
	T->Translate(deltaPos.x, deltaPos.y, deltaPos.z);/*CHECK; USING DELTA INSTEAD OF POS*/
}

Shape *RigidBody2D::GetShape()
{
	return shape;
}

Vector3D RigidBody2D::GetPosition()
{
	return mPos;
}

float RigidBody2D::getMass() 
{
	return mass;
}

float RigidBody2D::getBounce()
{
	return bounciness;
}

Vector3D RigidBody2D::GetVelocity()
{
	return mVelocity;
}

Component *RigidBody2D::createNew(GameObject *owner)
{
	if (owner == 0)
		return 0;

	return new RigidBody2D(owner, COMPONENT_TYPE::RIGIDBODY2D);
}

void RigidBody2D::serialize(std::fstream& stream)
{
}

void RigidBody2D::deserialize(std::fstream& stream)
{
	std::cout << "DESERIALIZING RIGIDBODY2D BEGIN-----------------------------------------------" << std::endl;

	std::string shapeType;
	float m, bounce;
	bool din;
	unsigned int colMask;
	float offsetx, offsety, offsetz;
	if (stream >> m >> din >> bounce >> colMask >> shapeType)
	{
		mass = m;
		if (mass != 0.0f)
			massInv = 1.0f / mass;
		else
			massInv = 0.0f;

		setCollisionMask(colMask);

		dynamic = din;
		
		bounciness = bounce;
		grounded = false;

		if (shapeType == "RectangleShape") 
		{
			float w, h;
			if (stream >> w >> h) 
			{
				shape = new RectangleShape(this, ShapeType::RECTANGLE);
				RectangleShape *rect = static_cast<RectangleShape*>(shape);
				rect->setSize(w, h);

				//Once the shapes are set, we set the offset of the shape
				if (stream >> offsetx >> offsety >> offsetz)
				{
					rect->setOffset(offsetx, offsety, offsetz);
				}
			}
			else
			{
				std::cout << "(RigidBody2D::deserialize)- Error 2.1 reading from stream" << std::endl;
			}
		}
		else if (shapeType == "CircleShape") 
		{
			float r;
			if (stream >> r)
			{
				CircleShape *circle = new CircleShape(this, ShapeType::CIRCLE);
				circle->setRadius(r);
				shape = circle;

				//Once the shapes are set, we set the offset of the shape
				if (stream >> offsetx >> offsety >> offsetz)
				{
					circle->setOffset(offsetx, offsety, offsetz);
				}
			}
			else
			{
				std::cout << "(RigidBody2D::deserialize)- Error 2.2 reading from stream" << std::endl;
			}
		}

		//Add this component to list managed by physicsManager
		pManager->GetPhysicsManager()->addRigidBody2D(this);
		//Also, if dynamic, add to dynamicRgdbds list
		if (dynamic)
			pManager->GetPhysicsManager()->addDynamicRigidBody2D(this);
	}
	else
	{
		std::cout << "(RigidBody2D::deserialize)- Error 1.0 reading from stream" << std::endl;
	}

	std::cout << "DESERIALIZING RIGIDBODY2D END-----------------------------------------------" << std::endl;
}


void RigidBody2D::setCollisionMask(unsigned int colMask)
{
	switch (colMask) 
	{
	case 0:
		collisionMask = CollisionMask::GROUND;
		break;
	case 1:
		collisionMask = CollisionMask::WALL;
		break;
	case 2:
		collisionMask = CollisionMask::UI;
		break;
	case 3:
		collisionMask = CollisionMask::STATIC_OBJ;
		break;
	case 4:
		collisionMask = CollisionMask::DINAMIC_OBJ;
		break;
	case 5:
		collisionMask = CollisionMask::PLAYER;
		break;
	default:
		break;
	}
}


void RigidBody2D::handleEvent(Event *pEvent)
{
	if (pEvent->type == EventType::ON_ENTER_TRIGGER)
	{
		std::vector<std::string> eventKeys = this->getOwner()->getEventKeys(pEvent->type);
		for (std::string key : eventKeys)
		{
			if (key == pEvent->eventKey && key == "trigger02")
			{
				Vector3D vel;
				Vector3DSet(&vel, 0, 20, 0);
				setVelocity(vel);
				break;
			}
		}
	}
	else if (pEvent->type == EventType::COLLISIONHIT) 
	{
		OnCollisionHitEvent *hitEvent = static_cast<OnCollisionHitEvent*>(pEvent);
		if (hitEvent) 
		{
			//std::cout << "HANDLING HIT EVENT. is grounded: " << grounded << std::endl;
			if (hitEvent->colMask == CollisionMask::GROUND 
				&& this->collisionMask ==  CollisionMask::PLAYER) 
			{
				if (!grounded)
					grounded = true;

				if (jumping)
					jumping = false;
			}
		}
	}
	else if (pEvent->type == EventType::PLAYERHIT)
	{
		if (dynamic && collisionMask != CollisionMask::PLAYER)
		{
			std::cout << "EVENT FIRED" << std::endl;
			Vector3D force;
			Vector3DSet(&force, 0, 30, 0);
			setVelocity(force);
		}
	}
}