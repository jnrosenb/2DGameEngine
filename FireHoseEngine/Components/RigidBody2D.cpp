#include "RigidBody2D.h"
#include "ParticleEmitter.h"
#include "../Managers.h"
#include "Transform.h"
#include "Player.h"
#include "Enemy.h"
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
	///Skip disabled rigidBodies
	if (!isEnabled())
		return;

	///Skip Non dynamic rigidBodies
	if (!isDynamic())
		return;

	///Gravity stuff
	if (hasGravity())
	{
		float gravityPull = -30.0f;
		Vector3D gravity;
		Vector3DSet(&gravity, 0.0f, gravityPull * deltaTime, 0.0f);
		setVelocity(gravity);
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

	//VELOCITY CAP
	float p = 0.05f;
	Vector3D AirResistance;
	Vector3DSet(&AirResistance, -mVelocity.x * p, -mVelocity.y * p, -mVelocity.z * p);
	Vector3DAdd(&mVelocity, &mVelocity, &AirResistance);

	Vector3D deltaPos;
	Vector3DScale(&deltaPos, &mVelocity, deltaTime);
	Vector3DAdd(&mPos, &mPos, &deltaPos); //We get the Position

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

bool RigidBody2D::hasGravity()
{
	return gravity;
}

void RigidBody2D::setGravity(bool flag)
{
	gravity = flag;
}

float RigidBody2D::getBounce()
{
	return bounciness;
}

Vector3D RigidBody2D::GetVelocity()
{
	return mVelocity;
}

void RigidBody2D::Jump()
{
	float JumpVelocityImpulse = 20.0f;
	Vector3D upVel;
	Vector3DSet(&upVel, 0, JumpVelocityImpulse, 0);
	setVelocity(upVel);
	jumping = true;
}

bool RigidBody2D::isJumping()
{
	return jumping;
}

bool RigidBody2D::isGrounded()
{
	return grounded;
}

void RigidBody2D::ResetKinematics()
{
	Vector3DSet(&mForce, 0, 0, 0);
	Vector3DSet(&mAcceleration, 0, 0, 0);
	Vector3DSet(&mVelocity, 0, 0, 0);
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
	bool enable, gvty, din;
	unsigned int colMask;
	float offsetx, offsety, offsetz;
	if (stream >> enable >> gvty >> m >> din >> bounce >> colMask >> shapeType)
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
		setEnabled(enable);
		setGravity(gvty);

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
	case 6:
		collisionMask = CollisionMask::ENEMY;
		break;
	case 7:
		collisionMask = CollisionMask::WEAPONS;
		break;
	case 8:
		collisionMask = CollisionMask::AMMO;
		break;
	default:
		break;
	}
}


void RigidBody2D::handleEvent(Event *pEvent)
{
	if (pEvent->type == EventType::ON_ENTER_TRIGGER)
	{
		//////////////////////////////////////////////////////////////////////////
		// FIRST WE CHECK KEY DRIVEN EVENTS, WHICH CAN ONLY AFFECT SUSCRIBERS	//
		// (SHOULD NOT BE USED FOR THE TWO MAIN INVOLVED IN THE TRIGGER)		//
		//////////////////////////////////////////////////////////////////////////
		std::vector<std::string> eventKeys = this->getOwner()->getEventKeys(pEvent->type);
		for (std::string key : eventKeys)
		{
			//Rigidbody suscribers that get affected by the switch jump mechanic
			if (key == pEvent->eventKey && key == "trigger02")
			{
				Vector3D vel;
				Vector3DSet(&vel, 0, 20, 0);
				setVelocity(vel);
				break;
			}


			//////////////////////////////////////////////////////////////////////////
			// HERE WE CHECK THE MAIN ACTORS INVOLVED IN THE TRIGGER COLLISION		//
			// (	but we still use keys to know if it is affected	)				//
			//////////////////////////////////////////////////////////////////////////
			if (key == pEvent->eventKey && (key == "triggerBullet01" || key == "triggerBullet02"))
			{
				OnEnterTriggerEvent *ev = static_cast<OnEnterTriggerEvent*>(pEvent);
				RigidBody2D *rgbdy = static_cast<RigidBody2D*>(ev->pBody);
				if (rgbdy == this && !ev->isYourTrigger)
				{
					//THIS is the blood splatter particle for the enemies and players apparently
					/*

					//VISUAL STUFF FOR ENEMY//////////////////////////////////////////////////////////////////
					//TODO temporary, just for visual fun
					/*
					Transform *T = static_cast<Transform*>(getOwner()->GetComponent(COMPONENT_TYPE::TRANSFORM));
					Transform *ammoT = static_cast<Transform*>(ev->pTrigger->getOwner()->GetComponent(COMPONENT_TYPE::TRANSFORM));
					RigidBody2D *ammoRB = static_cast<RigidBody2D*>(ev->pTrigger->getOwner()->GetComponent(COMPONENT_TYPE::RIGIDBODY2D));
					if (T && ammoT && ammoRB)
					{
						Vector3D ammoVel = ammoRB->GetVelocity();
						int sign = ammoVel.x > 0 ? 1 : -1;

						Vector3D launchVel;
						Vector3DSet(&launchVel, ammoVel.x, 6.0f, 0.0f);
						setVelocity(launchVel);
					}
					//this->getOwner()->setEnabled(false);
					//*/

					Player *player = static_cast<Player*>(this->getOwner()->GetComponent(COMPONENT_TYPE::CHARACTER));
					Enemy *enemy = static_cast<Enemy*>(this->getOwner()->GetComponent(COMPONENT_TYPE::CHARACTER));
					if (player)
					{
						player->TakeDamage();
					}
					else if (enemy) 
					{
						enemy->TakeDamage();
					}

					//VISUAL STUFF FOR ENEMY//////////////////////////////////////////////////////////////////
				}
			}
		}
	}
	//Rigidbody hit event
	else if (pEvent->type == EventType::COLLISIONHIT) 
	{
		OnCollisionHitEvent *hitEvent = static_cast<OnCollisionHitEvent*>(pEvent);
		if (hitEvent) 
		{
			//For player, whenever he hits any object while having negative y velocity
			// (and the object being beneath him), then he will be grounded
			if ( hitEvent->colMask == CollisionMask::GROUND && 
					( 
						this->collisionMask ==  CollisionMask::PLAYER || 
						this->collisionMask == CollisionMask::ENEMY      
					) 
				)
			{
				RigidBody2D *otherRdbdy = static_cast<RigidBody2D*>(hitEvent->other);
				//check if the colliding obj center is below the player
				int isAbove = (this->shape->getCenter().y) > (otherRdbdy->shape->getCenter().y);

				if (!grounded)
					grounded = true;
				jumping = false;
			}
		}
	}
	//Not sure if this is even being used anymore
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