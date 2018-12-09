#include "Player.h"
#include "Transform.h"
#include "RigidBody2D.h"
#include "ParticleEmitter.h"
#include "Animator.h"
#include "../GameObject.h"
#include "../Managers.h"
#include "../GameStateManager.h"
#include "../Events.h"
#include "../Math/Vector3D.h"
#include <iostream>

extern Manager *pManager;
extern GameStateManager *gamestateMgr;


Player::Player(GameObject *owner, COMPONENT_TYPE type) : 
	Character(owner, type), score(0), gold(0)
{
}

Player::~Player()
{
}

void Player::Update(unsigned int deltaTime)
{
	float dt = deltaTime / 1000.0f;

	//Regenerate gasoline
	if (gas < maxGas)
		gas += gasRegenSpeed * dt;
	else
		gas = maxGas;

	//Player animations state
	//TODO all this will go to a player state machine manager or component
	Animator *A = static_cast<Animator*>(getOwner()->GetComponent(COMPONENT_TYPE::ANIMATOR));
	RigidBody2D *RB = static_cast<RigidBody2D*>(getOwner()->GetComponent(COMPONENT_TYPE::RIGIDBODY2D));
	if (A && RB)
	{
		Vector3D mVelocity = RB->GetVelocity();

		//Send animation events
		if (fabs(mVelocity.x) > 1.0f)
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
		if (RB->isJumping() && !hasJetpack)
		{
			OnAnimationSwitch pEvent;
			pEvent.animTag = "jump";
			getOwner()->handleEvent(&pEvent);
		}
		else if (RB->isJumping() && hasJetpack)
		{
			OnAnimationSwitch pEvent;
			pEvent.animTag = "jetpack";
			getOwner()->handleEvent(&pEvent);
		}
	}
	//std::cout << "GAS: " << gas << std::endl;
}

void Player::UseJetpack()
{
	if (hasJetpack && gas >= 1.0f) 
	{
		RigidBody2D *rgdbdy = static_cast<RigidBody2D*>(getOwner()->GetComponent(COMPONENT_TYPE::RIGIDBODY2D));
		if (rgdbdy != 0)
		{
			gas -= 0.1f;
		
			Vector3D jetpackImpulse;
			Vector3DSet(&jetpackImpulse, 0, 0.75f, 0);

			//PARTICLES DOWN
			ParticleEmitter *PE = static_cast<ParticleEmitter*>(this->getOwner()->GetComponent(COMPONENT_TYPE::PARTICLE_EMITTER));
			if (PE)
			{
				PE->EmitOnce(10, EmissionShape::CONE_DOWN, 72, 94, 23);
			}

			rgdbdy->setVelocity(jetpackImpulse); 
		}
	}
}

Component *Player::createNew(GameObject *owner)
{
	return new Player(owner, COMPONENT_TYPE::CHARACTER);
}

void Player::serialize(std::fstream& stream)
{
}

void Player::deserialize(std::fstream& stream)
{
	std::cout << "DESERIALIZING PLAYER COMPONENT BEGIN" << std::endl;

	int maxHP;
	float maxGasoline;
	float gasSpeed;
	if (stream >> maxHP >> maxGasoline >> gasSpeed)
	{
		this->maxHealth = maxHP;
		this->maxGas = maxGasoline;
		this->gasRegenSpeed = gasSpeed;

		hasJetpack = true;
		//hasJetpack = false;
		health = maxHealth;
		gas = maxGas;
		score = 0;
		gold = 0;
	}

	std::cout << "DESERIALIZING  PLAYER COMPONENT END" << std::endl;
}

void Player::handleEvent(Event *pEvent)
{
}

void Player::EquipJetpack()
{
	hasJetpack = true;
}

void Player::TakeDamage(int damage) 
{
	std::cout << "TOOK DAMAGE" << std::endl;

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

	//Dead condition
	if (health <= 0)
	{
		//DO DEAD ANIMATION HERE
		gamestateMgr->SetNextState(GameState::GAMEOVER);
	}
}

void Player::TakeDamage(Vector3D launchDir, int damage)
{
	std::cout << "TOOK DAMAGE" << std::endl;

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

	//Dead condition
	if (health <= 0)
	{
		//DO DEAD ANIMATION HERE
		gamestateMgr->SetNextState(GameState::GAMEOVER);
	}
}