#include <iostream>
#include "Projectile.h"
#include "RigidBody2D.h"
#include "Renderer.h"
#include "Camera.h"
#include "Transform.h"
#include "Trigger.h"
#include "../GameObject.h"
#include "../Managers.h"

extern Manager *pManager;

////////////////////////////////////////////////////////////////////
////// BASE PROJECTILE CLASS                ////////////////////////
////////////////////////////////////////////////////////////////////
Projectile::Projectile(GameObject *owner, COMPONENT_TYPE type) : 
	Component(owner, type)
{
}

Projectile::~Projectile()
{
}

ProjectileType Projectile::getType()
{
	return projectiletype;
}

void Projectile::setProjectileType(ProjectileType t) 
{
	projectiletype = t;
}

void Projectile::setFiredPosition(Vector3D fPos)
{
	Vector3DSet(&firedPosition, fPos.x, fPos.y, fPos.z);
}

void Projectile::checkIfOutOfBounds()
{
	Transform *T = static_cast<Transform*>(getOwner()->GetComponent(COMPONENT_TYPE::TRANSFORM));
	if (T) 
	{
		float width = pManager->GetCameraManager()->GetMainCamera()->getOrtoWidth();
		float aspect = pManager->GetCameraManager()->GetMainCamera()->getAspect();
		float halfW = width / 2.0f;
		float halfH = (width / aspect) / 2.0f;

		Vector3D const goPos = T->getPosition();
		float offset = 2.0f;

		if ( fabs(goPos.x - firedPosition.x) > halfW + offset ||
			fabs(goPos.y - firedPosition.y) > halfH + offset)
		{
			//Out of bounds. Call reset on this projectile
			resetState();
		}
	}
}

////////////////////////////////////////////////////////////////////
//////     PHYSICS PROJECTILE					////////////////////
////////////////////////////////////////////////////////////////////
PhysicsProjectile::PhysicsProjectile(GameObject *owner, COMPONENT_TYPE type) :
	Projectile(owner, type)
{
	setProjectileType(ProjectileType::PHYSICS);
}

PhysicsProjectile::~PhysicsProjectile()
{
}

void PhysicsProjectile::Update(unsigned int deltaTime)
{
	if (isFired)
	{
		//Have a way to check if out of screen
		//(if so, reset position and everythin needed)
		checkIfOutOfBounds();
	}
}

void PhysicsProjectile::Fire(GameObject *shooter, Vector3D dir)
{
	//Enable the components and fire
	Transform   *T1 = static_cast<Transform*>(shooter->GetComponent(COMPONENT_TYPE::TRANSFORM));
	Transform   *T2 = static_cast<Transform*>(getOwner()->GetComponent(COMPONENT_TYPE::TRANSFORM));
	Trigger     *TR = static_cast<Trigger*>(getOwner()->GetComponent(COMPONENT_TYPE::TRIGGER));
	RigidBody2D *RB = static_cast<RigidBody2D*>(getOwner()->GetComponent(COMPONENT_TYPE::RIGIDBODY2D));
	Renderer    *RN = static_cast<Renderer*>(getOwner()->GetComponent(COMPONENT_TYPE::RENDERER));
	if (T1 && T2 && TR && RN && RB)
	{
		RB->setEnabled(true);
		RN->setEnabled(true);
		TR->setEnabled(true);

		isFired = true;

		Vector3D pos = T1->getPosition();
		Vector3D cpos = T2->getPosition();
		Vector3D c3;
		Vector3DSub(&c3, &pos, &cpos);
		T2->Translate(c3.x + dir.x, c3.y + dir.y, 0.0f);
		RB->ResetKinematics();
		Vector3DScale(&dir, &dir, 35);
		RB->setVelocity(dir);
		setFiredPosition(T2->getPosition());
	}
}

Component *PhysicsProjectile::createNew(GameObject *owner)
{
	return new PhysicsProjectile(owner, COMPONENT_TYPE::PROJECTILE);
}

void PhysicsProjectile::serialize(std::fstream& stream)
{
}

void PhysicsProjectile::deserialize(std::fstream& stream)
{
}

void PhysicsProjectile::handleEvent(Event *pEvent)
{
}

void PhysicsProjectile::resetState()
{
	std::cout << "(PhysicsProjectile::resetState)- Entered here" << std::endl;

	isFired = false;

	//Reset position and disable the rigidbody
	Transform   *T  = static_cast<Transform*>(getOwner()->GetComponent(COMPONENT_TYPE::TRANSFORM));
	Trigger     *TR = static_cast<Trigger*>(getOwner()->GetComponent(COMPONENT_TYPE::TRIGGER));
	RigidBody2D *RB = static_cast<RigidBody2D*>(getOwner()->GetComponent(COMPONENT_TYPE::RIGIDBODY2D));
	Renderer    *RN = static_cast<Renderer*>(getOwner()->GetComponent(COMPONENT_TYPE::RENDERER));
	if (T && TR && RN && RB)
	{
		Vector3D pos = T->getPosition();
		T->Translate(-pos.x, -pos.y, 0.0f);

		TR->setEnabled(false);
		RB->setEnabled(false);
		RN->setEnabled(false);
	}
}


////////////////////////////////////////////////////////////////////
//////    NON PHYSICS PROJECTILE                 ///////////////////
////////////////////////////////////////////////////////////////////
StraightProjectile::StraightProjectile(GameObject *owner, COMPONENT_TYPE type) :
	Projectile(owner, type)
{
	setProjectileType(ProjectileType::NON_PHYSICS);
}

StraightProjectile::~StraightProjectile()
{
}

void StraightProjectile::Update(unsigned int deltaTime)
{
	if (isFired) 
	{
		//Have a way to check if out of screen
		//(if so, reset position and everythin needed)
		checkIfOutOfBounds();

		float dt = deltaTime / 1000.0f;
		float speed = 15.0f;
		Transform *T2 = static_cast<Transform*>(getOwner()->GetComponent(COMPONENT_TYPE::TRANSFORM));
		T2->Translate(direction.x * speed * dt, direction.y * speed * dt, 0.0f);
	}
}

void StraightProjectile::Fire(GameObject *shooter, Vector3D dir)
{
	//Enable the components
	Transform *T1 = static_cast<Transform*>(shooter->GetComponent(COMPONENT_TYPE::TRANSFORM));
	Transform *T2 = static_cast<Transform*>(getOwner()->GetComponent(COMPONENT_TYPE::TRANSFORM));
	Renderer *RN = static_cast<Renderer*>(getOwner()->GetComponent(COMPONENT_TYPE::RENDERER));
	Trigger  *TR = static_cast<Trigger*>(getOwner()->GetComponent(COMPONENT_TYPE::TRIGGER));
	if (T1 && T2 && RN && TR) 
	{
		RN->setEnabled(true);
		TR->setEnabled(true);

		isFired = true;

		//TODO check everything is ok with the z values//
		Vector3D pos = T1->getPosition();
		Vector3D cpos = T2->getPosition();
		Vector3D c3;
		Vector3DSub(&c3, &pos, &cpos);
		T2->Translate(c3.x + dir.x, c3.y + dir.y, 0.0f);
		direction = dir;
		setFiredPosition(T2->getPosition());
	}
}

Component *StraightProjectile::createNew(GameObject *owner)
{
	return new StraightProjectile(owner, COMPONENT_TYPE::PROJECTILE);
}

void StraightProjectile::serialize(std::fstream& stream)
{
}

void StraightProjectile::deserialize(std::fstream& stream)
{
}

void StraightProjectile::handleEvent(Event *pEvent)
{
}

void StraightProjectile::resetState()
{
	std::cout << "(StraightProjectile::resetState)- Entered here" << std::endl;
	isFired = false;
	
	//Reset position and disable the rigidbody
	Transform   *T = static_cast<Transform*>(getOwner()->GetComponent(COMPONENT_TYPE::TRANSFORM));
	Trigger     *TR = static_cast<Trigger*>(getOwner()->GetComponent(COMPONENT_TYPE::TRIGGER));
	RigidBody2D *RB = static_cast<RigidBody2D*>(getOwner()->GetComponent(COMPONENT_TYPE::RIGIDBODY2D));
	Renderer    *RN = static_cast<Renderer*>(getOwner()->GetComponent(COMPONENT_TYPE::RENDERER));
	if (T && TR && RN && RB)
	{
		Vector3D pos = T->getPosition();
		T->Translate(-pos.x, -pos.y, 0.0f);

		TR->setEnabled(false);
		RB->setEnabled(false);
		RN->setEnabled(false);
	}
}