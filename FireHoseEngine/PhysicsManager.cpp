#include "PhysicsManager.h"
#include "Components/RigidBody2D.h"
#include "Math/Vector3D.h"
#include "Managers.h"

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

void PhysicsManager::LateUpdate(unsigned int deltaTime)
{
	float dt = deltaTime / 1000.0f;

	for (RigidBody2D *rby : rigidBodies)
	{
		//Gravity crap
		Vector3D gravity;
		Vector3DSet(&gravity, 0.0f, -20.0f * dt, 0.0f);
		rby->setVelocity(gravity); //ADDS, not set

		rby->LateUpdate(dt);
	}

	//Reset contacts (why here?)
	pManager->GetCollisionManager()->ResetContacts();

	//Check for collisions
	auto rgbyBgn1 = rigidBodies.begin();
	auto rgbyEnd = rigidBodies.end();
	for (; rgbyBgn1 != rgbyEnd; ++rgbyBgn1) 
	{
		for (auto rgbyBgn2 = rgbyBgn1 + 1; rgbyBgn2 != rgbyEnd; ++rgbyBgn2) 
		{
			Shape *shp1 = (*rgbyBgn1)->GetShape();
			Shape *shp2 = (*rgbyBgn2)->GetShape();

			//For now, if one of the go doesn't have a transform, collision is not checked
			Vector3D pos1, pos2;
			Transform *T1 = static_cast<Transform*>((*rgbyBgn1)->getOwner()->GetComponent(COMPONENT_TYPE::TRANSFORM));
			Transform *T2 = static_cast<Transform*>((*rgbyBgn2)->getOwner()->GetComponent(COMPONENT_TYPE::TRANSFORM));
			if (T1 != 0 && T2 != 0)
			{
				Vector3DSet(&pos1, T1->getPosition().x, T1->getPosition().y, T1->getPosition().z);
				Vector3DSet(&pos2, T2->getPosition().x, T2->getPosition().y, T2->getPosition().z);

				/*IF BOTH ARE STATIC, IT IS NOT NECESSARY TO CHECK FOR COLLISIONS*/
				/*TODO LATER*/

				///TODO call collision manager to check if the two rigidbodies shapes are colliding
				bool areColliding = pManager->GetCollisionManager()->checkCollision(shp1, pos1, shp2, pos2);
			}
		}
	}

	//Solve contacts
	for (Contact *c : pManager->GetCollisionManager()->GetContacts())
	{
		//randomContactResolution(c);
		impulseContactResolution(c);
	}
}


///////////////////////////////////////////////////////////////
//Contact resolution techniques////////////////////////////////
void PhysicsManager::randomContactResolution(Contact *c)
{
	//Decide which shape is gonna move
	Shape *shape1 = c->getFirstShape();
	Shape *shape2 = c->getSecondShape();

	RigidBody2D *rgbdy = shape1->GetShapeOwner();
	if (rgbdy == 0)
		return;

	if (rgbdy->isDynamic()) /*Moves shape 1*/
	{
		GameObject *owner = rgbdy->getOwner();
		if (owner == 0) return;

		Transform *T = static_cast<Transform*>(owner->GetComponent(COMPONENT_TYPE::TRANSFORM));
		if (T)
		{
			Vector3D sub;
			Vector3DSub(&sub, &shape1->getCenter(), &shape2->getCenter());
			Vector3DSet(&sub, sub.x, sub.y, 0);
			float dot = Vector3DDotProduct(&sub, &c->MTVector);
			int sign = Sign(dot);

			T->Translate(sign * c->MTVector.x, sign * c->MTVector.y, sign * c->MTVector.z);
		}
	}
	else
	{
		//If the first object was static, try with second
		rgbdy = shape2->GetShapeOwner();
		if (rgbdy == 0)
			return;

		if (rgbdy->isDynamic()) /*Moves shape 2*/
		{
			GameObject *owner = rgbdy->getOwner();
			if (owner == 0)
				return;

			Transform *T = static_cast<Transform*>(owner->GetComponent(COMPONENT_TYPE::TRANSFORM));
			if (T)
			{
				Vector3D sub;
				Vector3DSub(&sub, &shape2->getCenter(), &shape1->getCenter());
				float dot = Vector3DDotProduct(&sub, &c->MTVector);
				int sign = Sign(dot);

				T->Translate(sign * c->MTVector.x, sign * c->MTVector.y, sign * c->MTVector.z);
			}
		}
	}
}


void PhysicsManager::impulseContactResolution(Contact *c)
{
	//Decide which shape is gonna move
	Shape *shape1 = c->getFirstShape();
	Shape *shape2 = c->getSecondShape();

	RigidBody2D *rgbdy1 = shape1->GetShapeOwner();
	if (rgbdy1 == 0) return;
	RigidBody2D *rgbdy2 = shape2->GetShapeOwner();
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

			//Separating objects
			T1->Translate( sign * c->MTVector.x / 2.0f,  sign * c->MTVector.y / 2.0f,  sign * c->MTVector.z / 2.0f);
			T2->Translate(-sign * c->MTVector.x / 2.0f, -sign * c->MTVector.y / 2.0f, -sign * c->MTVector.z / 2.0f);
			
			//*
			//Add Impulse
			Vector3D vel1;
			Vector3DSub(&vel1, &rgbdy1->GetVelocity(), &rgbdy2->GetVelocity());
			Vector3D vel2;
			Vector3DNeg(&vel2, &vel1);

			Vector3D invVel;
			Vector3DNeg(&invVel, &vel1);
			Vector3D normal;
			Vector3DNormalize(&normal, &c->MTVector); //CAREFUL WITH DIRECTION OF THIS VECTOR
			Vector3DScale(&normal, &normal, -sign);
			float invVELdotNormal = Vector3DDotProduct(&invVel, &normal);
			Vector3D aux;
			Vector3DScale(&aux, &normal, invVELdotNormal);
			float massProp1 = 2 *(rgbdy2->getMass() / (rgbdy1->getMass() + rgbdy2->getMass()));
			Vector3DScale(&aux, &aux, massProp1);
			Vector3DSet(&aux, aux.x, aux.y, 0);
			rgbdy1->setVelocity(aux);
			
			//Add Impulse
			Vector3DNeg(&invVel, &vel2);
			Vector3DNormalize(&normal, &c->MTVector); //CAREFUL WITH DIRECTION OF THIS VECTOR
			Vector3DScale(&normal, &normal, -sign);
			invVELdotNormal = Vector3DDotProduct(&invVel, &normal);
			Vector3DScale(&aux, &normal, invVELdotNormal);
			float massProp2 = 2 * (rgbdy1->getMass() / (rgbdy1->getMass() + rgbdy2->getMass()));
			Vector3DScale(&aux, &aux, massProp2);
			Vector3DSet(&aux, aux.x, aux.y, 0);
			rgbdy2->setVelocity(aux);
			//*/
		}
	}
	else if (rgbdy1->isDynamic()) /*Moves shape1*/
	{
		GameObject *owner = rgbdy1->getOwner();
		if (owner == 0) return;

		Transform *T = static_cast<Transform*>(owner->GetComponent(COMPONENT_TYPE::TRANSFORM));
		if (T)
		{
			Vector3D sub;
			Vector3DSub(&sub, &shape2->getCenter(), &shape1->getCenter());
			float dot = Vector3DDotProduct(&sub, &c->MTVector);
			int sign = Sign(dot);

			//Separating objects
			T->Translate(sign * c->MTVector.x, sign * c->MTVector.y, sign * c->MTVector.z);

			//Add Impulse
			Vector3D vel = rgbdy1->GetVelocity();
			Vector3D invVel;
			Vector3DNeg(&invVel, &vel);
			Vector3D normal;
			Vector3DNormalize(&normal, &c->MTVector); /*CAREFUL WITH DIRECTION OF THIS VECTOR*/
			Vector3DScale(&normal, &normal, -sign);
			float invVELdotNormal = Vector3DDotProduct(&invVel, &normal);
			Vector3D aux;
			Vector3DScale(&aux, &normal, invVELdotNormal);
			Vector3DScale(&aux, &aux, 2);
			Vector3DSet(&aux, aux.x, aux.y, 0);
			rgbdy1->setVelocity(aux);
		}
	}
	else if (rgbdy2->isDynamic()) /*Moves shape2*/
	{
		GameObject *owner = rgbdy2->getOwner();
		if (owner == 0) return;

		Transform *T = static_cast<Transform*>(owner->GetComponent(COMPONENT_TYPE::TRANSFORM));
		if (T)
		{
			Vector3D sub;
			Vector3DSub(&sub, &shape2->getCenter(), &shape1->getCenter());
			float dot = Vector3DDotProduct(&sub, &c->MTVector);
			int sign = Sign(dot);

			//Separate colliding objects
			T->Translate(sign * c->MTVector.x, sign * c->MTVector.y, sign * c->MTVector.z);

			//Add Impulse
			Vector3D vel = rgbdy2->GetVelocity();
			Vector3D invVel;
			Vector3DNeg(&invVel, &vel);
			Vector3D normal;
			Vector3DNormalize(&normal, &c->MTVector); /*CAREFUL WITH DIRECTION OF THIS VECTOR*/
			Vector3DScale(&normal, &normal, -sign);
			float invVELdotNormal = Vector3DDotProduct(&invVel, &normal);
			Vector3D aux;
			Vector3DScale(&aux, &normal, invVELdotNormal);
			Vector3DScale(&aux, &aux, 2);
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

int PhysicsManager::Sign(float a) 
{
	int a1 = (a > 0.0f);
	int a2 = (a < 0.0f);
	return a1 - a2;
}