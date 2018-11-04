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
		//Vector3DSet(&gravity, 0.0f, -10.0f, 0.0f);
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

				//TODO call collision manager to check if the two rigidbodies shapes are colliding
				bool areColliding = pManager->GetCollisionManager()->checkCollision(shp1, pos1, shp2, pos2);

				if (areColliding)
				{
					Contact *c1 = new Contact(shp1, shp2); /*TODO: WHERE IS THIS ONE DEALLOCATED??*/
					pManager->GetCollisionManager()->AddContacts(c1);
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