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
		//Vector3DSet(&gravity, 0.0f, -1.0f, 0.0f);
		Vector3DSet(&gravity, 0.0f, 0.0f, 0.0f);
		rby->LateUpdate(dt, gravity);
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

				//TODO call collision manager to check if the two rigidbodies shapes are colliding
				bool areColliding = pManager->GetCollisionManager()->checkCollision(shp1, pos1, shp2, pos2);
			}
		}
	}

	//Solve contacts
	for (Contact *c : pManager->GetCollisionManager()->GetContacts())
	{
		//Decide which shape is gonna move
		Shape *shape1 = c->getFirstShape();
		Shape *shape2 = c->getSecondShape();

		RigidBody2D *rgbdy = shape1->GetShapeOwner();
		if (rgbdy == 0)
			continue;

		///////////////////////////////////////
		//// TEMPORARY MEASURE   //////////////
		///////////////////////////////////////
		if (rgbdy->isDynamic()) /*Moves shape 1*/
		{
			GameObject *owner = rgbdy->getOwner();
			if (owner == 0) continue;

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
				continue;

			if (rgbdy->isDynamic()) /*Moves shape 2*/
			{
				GameObject *owner = rgbdy->getOwner();
				if (owner == 0)
					continue;

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
}

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