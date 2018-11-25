#include "PhysicsManager.h"
#include "Components/RigidBody2D.h"
#include "Math/Vector3D.h"
#include "Managers.h"
#include "Events.h"   /*CHECK IF THIS IS NECESSARY*/
#include "GameObject.h"
#include "Components/Transform.h"

extern Manager *pManager;


PhysicsManager::PhysicsManager()
{
}

PhysicsManager::~PhysicsManager()
{
	//TODO: See if I have to clear vector of rigidBodies
}

void PhysicsManager::Update()
{
	//For now, not being used
}

std::vector<RigidBody2D*> const& PhysicsManager::getDynamicBodies()
{
	return dynamicRgdbs;
}

void PhysicsManager::LateUpdate(unsigned int deltaTime)
{
	//Delta time in miliseconds
	float dt = deltaTime / 1000.0f;

	//Physic calculations for rigidbodies
	for (RigidBody2D *rby : rigidBodies)
	{
		rby->LateUpdate(dt);
	}

	//Reset contacts
	pManager->GetCollisionManager()->ResetContacts();

	//Check for collisions
	auto rgbyBgn1 = rigidBodies.begin();
	auto rgbyEnd = rigidBodies.end();
	for (; rgbyBgn1 != rgbyEnd; ++rgbyBgn1) 
	{
		if (!(*rgbyBgn1)->isEnabled()) continue;

		for (auto rgbyBgn2 = rgbyBgn1 + 1; rgbyBgn2 != rgbyEnd; ++rgbyBgn2) 
		{
			if (!(*rgbyBgn2)->isEnabled()) continue;

			Shape *shp1 = (*rgbyBgn1)->GetShape();
			Shape *shp2 = (*rgbyBgn2)->GetShape();

			//For now, if one of the go doesn't have a transform, collision is not checked
			Vector3D pos1, pos2;
			Transform *T1 = static_cast<Transform*>((*rgbyBgn1)->getOwner()->GetComponent(COMPONENT_TYPE::TRANSFORM));
			Transform *T2 = static_cast<Transform*>((*rgbyBgn2)->getOwner()->GetComponent(COMPONENT_TYPE::TRANSFORM));
			if (T1 != 0 && T2 != 0)
			{
				Vector3DSet(&pos1, T1->getPosition().x + shp1->getOffset().x, 
					T1->getPosition().y + shp1->getOffset().y, 
					T1->getPosition().z + shp1->getOffset().z);
				Vector3DSet(&pos2, T2->getPosition().x + shp2->getOffset().x, 
					T2->getPosition().y + shp2->getOffset().y, 
					T2->getPosition().z + shp2->getOffset().z);
				//Vector3DSet(&pos1, shp1->getCenter().x, shp1->getCenter().y, shp1->getCenter().z);
				//Vector3DSet(&pos2, shp2->getCenter().x, shp2->getCenter().y, shp2->getCenter().z);

				/*IF BOTH ARE STATIC, IT IS NOT NECESSARY TO CHECK FOR COLLISIONS*/
				/*TODO LATER*/
				if (!(*rgbyBgn1)->isDynamic() && !(*rgbyBgn2)->isDynamic())
					continue;

				///TODO call collision manager to check if the two rigidbodies shapes are colliding
				bool areColliding = pManager->GetCollisionManager()->checkCollision(shp1, pos1, shp2, pos2);
			}
		}
	}

	//Solve contacts and send COLLIDE HIT MESSAGE
	for (Contact *c : pManager->GetCollisionManager()->GetContacts())
	{
		//randomContactResolution(c);
		impulseContactResolution(c);

		//* MOVE THIS INTO IMPULSE OR SOMEWHERE ELSE
		//Get info of contact
		Shape *shape1 = c->getFirstShape();
		Shape *shape2 = c->getSecondShape();
		RigidBody2D *rgbdy1 = static_cast<RigidBody2D*>(shape1->GetShapeOwner());
		if (rgbdy1 == 0) return;
		GameObject *owner1 = rgbdy1->getOwner();
		if (owner1 == 0) return;
		RigidBody2D *rgbdy2 = static_cast<RigidBody2D*>(shape2->GetShapeOwner());
		if (rgbdy2 == 0) return;
		GameObject *owner2 = rgbdy2->getOwner();
		if (owner2 == 0) return;

		//DIRECT EVENT SENDING TO HANLDER
		OnCollisionHitEvent hitEvent1 (rgbdy1, rgbdy1->collisionMask);
		OnCollisionHitEvent hitEvent2 (rgbdy2, rgbdy2->collisionMask);
		owner1->handleEvent(&hitEvent2);
		owner2->handleEvent(&hitEvent1);
	}
}


///////////////////////////////////////////////////////////////
//Contact resolution techniques////////////////////////////////
void PhysicsManager::impulseContactResolution(Contact *c)
{
	//Decide which shape is gonna move
	Shape *shape1 = c->getFirstShape();
	Shape *shape2 = c->getSecondShape();

	RigidBody2D *rgbdy1 = static_cast<RigidBody2D*>(shape1->GetShapeOwner());
	if (rgbdy1 == 0) return;
	RigidBody2D *rgbdy2 = static_cast<RigidBody2D*>(shape2->GetShapeOwner());
	if (rgbdy2 == 0) return;

	if (rgbdy1->isDynamic() && rgbdy2->isDynamic()) /*Moves Both shapes*/
	{
		GameObject *owner1 = rgbdy1->getOwner();
		if (owner1 == 0) return;
		GameObject *owner2 = rgbdy2->getOwner();
		if (owner2 == 0) return;

		Transform *T1 = static_cast<Transform*>(owner1->GetComponent(COMPONENT_TYPE::TRANSFORM));
		Transform *T2 = static_cast<Transform*>(owner2->GetComponent(COMPONENT_TYPE::TRANSFORM));
		if (T1 && T2)
		{
			Vector3D sub;
			Vector3DSub(&sub, &shape1->getCenter(), &shape2->getCenter());
			Vector3DSet(&sub, sub.x, sub.y, 0);
			float dot = Vector3DDotProduct(&sub, &c->MTVector);
			int sign = Sign(dot);

			///Separating objects
			//T1->Translate( sign * c->MTVector.x / 2.0f,  sign * c->MTVector.y / 2.0f,  sign * c->MTVector.z / 2.0f);
			//T2->Translate(-sign * c->MTVector.x / 2.0f, -sign * c->MTVector.y / 2.0f, -sign * c->MTVector.z / 2.0f);
			float p1 = rgbdy1->getMass() / (rgbdy1->getMass() + rgbdy2->getMass());
			float p2 = 1.0f - p1;
			T1->Translate( p1 *  sign * c->MTVector.x, p1 *  sign * c->MTVector.y, p1 *  sign * c->MTVector.z / 2.0f);
			T2->Translate( p2 * -sign * c->MTVector.x, p2 * -sign * c->MTVector.y, p2 * -sign * c->MTVector.z / 2.0f);
			
			//Add Impulse
			Vector3D vel1;
			Vector3DSub(&vel1, &rgbdy1->GetVelocity(), &rgbdy2->GetVelocity());
			Vector3D vel2;
			Vector3DNeg(&vel2, &vel1);

			Vector3D invVel;
			Vector3DNeg(&invVel, &vel1);
			Vector3D normal;
			Vector3DNormalize(&normal, &c->MTVector); //CAREFUL WITH DIRECTION OF THIS VECTOR
			Vector3DScale(&normal, &normal, (float)-sign);
			float invVELdotNormal = Vector3DDotProduct(&invVel, &normal);
			Vector3D aux;
			Vector3DScale(&aux, &normal, invVELdotNormal);
			float massProp1 = rgbdy2->getBounce() * 2 *(rgbdy2->getMass() / (rgbdy1->getMass() + rgbdy2->getMass()));
			Vector3DScale(&aux, &aux, massProp1);
			Vector3DSet(&aux, aux.x, aux.y, 0);
			rgbdy1->setVelocity(aux);
			
			//Add Impulse
			Vector3DNeg(&invVel, &vel2);
			Vector3DNormalize(&normal, &c->MTVector); //CAREFUL WITH DIRECTION OF THIS VECTOR
			Vector3DScale(&normal, &normal, (float)-sign);
			invVELdotNormal = Vector3DDotProduct(&invVel, &normal);
			Vector3DScale(&aux, &normal, invVELdotNormal);
			float massProp2 = rgbdy1->getBounce() * 2 * (rgbdy1->getMass() / (rgbdy1->getMass() + rgbdy2->getMass()));
			Vector3DScale(&aux, &aux, massProp2);
			Vector3DSet(&aux, aux.x, aux.y, 0);
			rgbdy2->setVelocity(aux);
		}
	}
	else if (rgbdy1->isDynamic() && !rgbdy2->isDynamic()) /*Moves shape1*/
	{
		GameObject *owner = rgbdy1->getOwner();
		if (owner == 0) return;

		Transform *T = static_cast<Transform*>(owner->GetComponent(COMPONENT_TYPE::TRANSFORM));
		if (T)
		{
			Vector3D sub;
			Vector3DSub(&sub, &shape1->getCenter(), &shape2->getCenter());
			float dot = Vector3DDotProduct(&sub, &c->MTVector);
			int sign = Sign(dot);

			//*
			if (rgbdy2->collisionMask == CollisionMask::GROUND && rgbdy1->isGrounded())
			{
				return;
			}
			//*/

			//Separating objects
			T->Translate(sign * c->MTVector.x, sign * c->MTVector.y, sign * c->MTVector.z);
			
			//Experiment: Make no impulse modifications when the dynamic go is 
			/*getting away from the other static obj
			Vector3D subTest;
			Vector3DSub(&subTest, &shape2->getCenter(), &shape1->getCenter());
			if (Vector3DDotProduct(&subTest, &rgbdy1->GetVelocity()) < -0.5f)
			{
				//std::cout << "NOT NECESSARY TO DO IMPULSE" << std::endl;
				return;
			}//*/

			//Add Impulse
			Vector3D vel = rgbdy1->GetVelocity();
			Vector3D invVel;
			Vector3DNeg(&invVel, &vel);
			Vector3D normal;
			Vector3DNormalize(&normal, &c->MTVector); //CAREFUL WITH DIRECTION OF THIS VECTOR
			Vector3DScale(&normal, &normal, (float)-sign);
			float invVELdotNormal = Vector3DDotProduct(&invVel, &normal);
			Vector3D aux;
			Vector3DScale(&aux, &normal, invVELdotNormal);
			Vector3DScale(&aux, &aux, 2 * rgbdy1->getBounce());
			Vector3DSet(&aux, aux.x, aux.y, 0);
			rgbdy1->setVelocity(aux);
		}
	}
	else if (rgbdy2->isDynamic() && !rgbdy1->isDynamic()) /*Moves shape2*/
	{
		GameObject *owner = rgbdy2->getOwner();
		if (owner == 0) return;

		Transform *T = static_cast<Transform*>(owner->GetComponent(COMPONENT_TYPE::TRANSFORM));
		if (T)
		{
			Vector3D sub;
			Vector3DSub(&sub, &shape2->getCenter(), &shape1->getCenter());
			Vector3DSet(&sub, sub.x, sub.y, 0.0f);
			float dot = Vector3DDotProduct(&sub, &c->MTVector);
			int sign = Sign(dot);

			//*
			if (rgbdy1->collisionMask == CollisionMask::GROUND && rgbdy2->isGrounded())
			{
				return;
			}
			//*/

			//Separate colliding objects
			T->Translate(sign * c->MTVector.x, sign * c->MTVector.y, sign * c->MTVector.z);

			//Experiment: Make no impulse modifications when the dynamic go is 
			//*getting away from the other static obj
			Vector3D subTest;
			Vector3DSub(&subTest, &shape1->getCenter(), &shape2->getCenter());
			if ( Vector3DDotProduct(&subTest, &rgbdy2->GetVelocity()) < -0.5f )
			{
				//std::cout << "NOT NECESSARY TO DO IMPULSE" << std::endl;
				return;
			}//*/

			//Add Impulse
			Vector3D vel = rgbdy2->GetVelocity();
			Vector3DSet(&vel, vel.x, vel.y, 0);
			Vector3D invVel;
			Vector3DNeg(&invVel, &vel);
			Vector3D normal;
			Vector3DNormalize(&normal, &c->MTVector); //CAREFUL WITH DIRECTION OF THIS VECTOR
			Vector3DScale(&normal, &normal, (float)-sign);
			float invVELdotNormal = Vector3DDotProduct(&invVel, &normal);
			Vector3D aux;
			Vector3DScale(&aux, &normal, invVELdotNormal);
			Vector3DScale(&aux, &aux, 2 * rgbdy2->getBounce());
			Vector3DSet(&aux, aux.x, aux.y, 0);
			rgbdy2->setVelocity(aux);
		}
	}
}
//Contact resolution techniques////////////////////////////////
///////////////////////////////////////////////////////////////


void PhysicsManager::addRigidBody2D(RigidBody2D *rby)
{
	if (rby == 0)
		return;
	rigidBodies.push_back(rby);
}

void PhysicsManager::addDynamicRigidBody2D(RigidBody2D *rby)
{
	if (rby == 0)
		return;
	dynamicRgdbs.push_back(rby);
}

int PhysicsManager::Sign(float a) 
{
	int a1 = (a > 0.0f);
	int a2 = (a < 0.0f);
	return a1 - a2;
}