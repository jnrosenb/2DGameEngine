#include "Enemy.h"
#include "Transform.h"
#include "RigidBody2D.h"
#include "ParticleEmitter.h"
#include "Animator.h"
#include "WeaponSlot.h"
#include "../GameObject.h"
#include "../Managers.h"
#include "../GameStateManager.h"
#include "../Events.h"
#include "../Math/Vector3D.h"
#include <iostream>

extern Manager *pManager;
extern GameStateManager *gamestateMgr;


Enemy::Enemy(GameObject *owner, COMPONENT_TYPE type) :
	Character(owner, type)
{
}

Enemy::~Enemy()
{
}

void Enemy::Update(unsigned int deltaTime)
{
}

Component *Enemy::createNew(GameObject *owner)
{
	return new Enemy(owner, COMPONENT_TYPE::CHARACTER);
}

void Enemy::serialize(std::fstream& stream)
{
}

void Enemy::deserialize(std::fstream& stream)
{
	std::cout << "DESERIALIZING ENEMY COMPONENT BEGIN" << std::endl;

	int maxHP;
	if (stream >> maxHP)
	{
		this->maxHealth = maxHP;
		health = maxHealth;
	}

	std::cout << "DESERIALIZING  ENEMY COMPONENT END" << std::endl;
}

void Enemy::handleEvent(Event *pEvent)
{
}

void Enemy::TakeDamage(int damage)
{
	health -= damage;

	//Damage animation
	RigidBody2D *rgdbdy = static_cast<RigidBody2D*>(getOwner()->GetComponent(COMPONENT_TYPE::RIGIDBODY2D));
	if (rgdbdy != 0)
	{
		Vector3D launchDir;
		Vector3DSet(&launchDir, 0.0f, 4.5f, 0.0f);
		rgdbdy->setVelocity(launchDir);
	}

	//Particle effect
	ParticleEmitter *PE = static_cast<ParticleEmitter*>(getOwner()->GetComponent(COMPONENT_TYPE::PARTICLE_EMITTER));
	if (PE)
	{
		PE->EmitOnce(10, EmissionShape::CIRCLE, 24, 46, 23);
	}

	//Dead condition for enemy
	if (health <= 0)
	{
		//Particle effect
		WeaponSlot *WH = static_cast<WeaponSlot*>(getOwner()->GetComponent(COMPONENT_TYPE::WEAPON_SLOT));
		if (WH)
		{
			WH->DropWeapon();
		}

		this->getOwner()->setEnabled(false);
	}
}

void Enemy::TakeDamage(Vector3D launchDir, int damage)
{
	health -= damage;

	//Damage animation
	RigidBody2D *rgdbdy = static_cast<RigidBody2D*>(getOwner()->GetComponent(COMPONENT_TYPE::RIGIDBODY2D));
	if (rgdbdy != 0)
	{
		rgdbdy->setVelocity(launchDir);
	}

	//Particle effect
	ParticleEmitter *PE = static_cast<ParticleEmitter*>(getOwner()->GetComponent(COMPONENT_TYPE::PARTICLE_EMITTER));
	if (PE)
	{
		PE->EmitOnce(10, EmissionShape::CIRCLE, 24, 46, 23);
	}

	//Dead condition for enemy
	if (health <= 0)
	{
		//Particle effect
		WeaponSlot *WH = static_cast<WeaponSlot*>(getOwner()->GetComponent(COMPONENT_TYPE::WEAPON_SLOT));
		if (WH)
		{
			WH->DropWeapon();
		}

		this->getOwner()->setEnabled(false);
	}
}