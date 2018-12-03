#include <iostream>
#include "Controller.h"
#include "Transform.h"
#include "RigidBody2D.h"
#include "Animator.h"
#include "Camera.h"
#include "WeaponSlot.h"
#include "../GameObject.h"
#include "../EventManager.h"
#include "../GameStateManager.h"
#include "../Events.h"

#include "ParticleEmitter.h"


extern Manager *pManager;

#define TEMPSPEED 4.0f


Controller::Controller(GameObject *owner, COMPONENT_TYPE type) :
	Component(owner, type)
{
}

Controller::~Controller()
{
}

void Controller::Update(unsigned int deltaTime)
{
	//If controller is not active, skip
	if (!active) 
	{
		return;
	}

	//Later on, see if its better (in terms of architecture) to move this
	//to constructor to avoid calling it each update loop
	Transform *T = static_cast<Transform*>(getOwner()->GetComponent(COMPONENT_TYPE::TRANSFORM));
	if (!T)
	{
		std::cout << "(Controller::Update)- Transform component null" << std::endl;
		return;
	}

	///Deltatime in seconds
	float dt = deltaTime / 1000.f;
	float horizontalSpeedImpulse = 0.25f;

	//Manages vertical motion
	if (pManager->GetInputManager()->getKeyTrigger(SDL_SCANCODE_UP) ||
		pManager->GetInputManager()->getKeyTrigger(SDL_SCANCODE_W))
	{
		float moveAmount = dt * TEMPSPEED;
		//T->Translate(0, 0, -moveAmount);
		//T->Translate(0, moveAmount, 0);

		//*
		RigidBody2D *rgdbdy = static_cast<RigidBody2D*>(getOwner()->GetComponent(COMPONENT_TYPE::RIGIDBODY2D));
		if (rgdbdy != 0) 
			rgdbdy->Jump();
		//*/
	}
	else if (pManager->GetInputManager()->getKeyPress(SDL_SCANCODE_DOWN))
	{
		float moveAmount = dt * TEMPSPEED;
		//T->Translate(0, 0, moveAmount);
		//T->Translate(0, -moveAmount, 0);
	}
	
	//Manages horizontal motion
	if (pManager->GetInputManager()->getKeyPress(SDL_SCANCODE_LEFT) || 
		pManager->GetInputManager()->getKeyPress(SDL_SCANCODE_A))
	{
		float moveAmount = dt * TEMPSPEED;
		//T->Translate(-moveAmount, 0, 0);
		//T->Rotate(5.0f);
		//T->Translate(-0.01f, 0, 0);

		T->Scale(-1, 1, 1);

		//*
		RigidBody2D *rgdbdy = static_cast<RigidBody2D*>(getOwner()->GetComponent(COMPONENT_TYPE::RIGIDBODY2D));
		if (rgdbdy != 0)
		{
			Vector3D leftVel;
			Vector3DSet(&leftVel, -horizontalSpeedImpulse, 0, 0);
			rgdbdy->setVelocity(leftVel);
		}
		//*/
	}
	else if (pManager->GetInputManager()->getKeyPress(SDL_SCANCODE_RIGHT) ||
			pManager->GetInputManager()->getKeyPress(SDL_SCANCODE_D))
	{
		float moveAmount = dt * TEMPSPEED;
		//T->Translate(moveAmount, 0, 0);
		//T->Rotate(-5.0f);
		//T->Translate(0.01f, 0, 0);

		T->Scale(1, 1, 1);

		//*
		RigidBody2D *rgdbdy = static_cast<RigidBody2D*>(getOwner()->GetComponent(COMPONENT_TYPE::RIGIDBODY2D));
		if (rgdbdy != 0)
		{
			Vector3D rightVel;
			Vector3DSet(&rightVel, horizontalSpeedImpulse, 0, 0);
			rgdbdy->setVelocity(rightVel);
		}
		//*/
	}

	//FIRES WEAPON IN MOUSE DIRECTION
	if (pManager->GetInputManager()->getLeftClick())
	{
		//Get the weapon holder to shoot
		WeaponSlot *WS = static_cast<WeaponSlot*>(this->getOwner()->GetComponent(COMPONENT_TYPE::WEAPON_SLOT));
		if (WS)
		{
			WS->Fire();
		}
	}

	//PICKS OR DROP WEAPON DEPENDING
	if (pManager->GetInputManager()->getRightClick())
	{
		WeaponSlot *WS = static_cast<WeaponSlot*>(this->getOwner()->GetComponent(COMPONENT_TYPE::WEAPON_SLOT));
		if (WS && !WS->hasWeapon())
		{
			WS->PickWeaponUp();
		}
		else if (WS && WS->hasWeapon())
		{
			WS->DropWeapon();
		}
	}

	//Rotates the object
	if (pManager->GetInputManager()->getKeyPress(SDL_SCANCODE_SPACE))
	{
		//T->Rotate(5.0f);

		//Get random go and make it target. Then, in 2 seconds, go back to owner target
		int randy = rand() % 10;
		GameObject *go = pManager->GetGameObjMgr()->GetGOByIndex(44);
		pManager->GetCameraManager()->GetMainCamera()->setTargetFor(go, 3.0f, 2.0f);
	}


	/*PARTICLES--EXPERIMENT////////////////////////////////////////////
	if (pManager->GetInputManager()->getKeyTrigger(SDL_SCANCODE_L))
	{
		ParticleEmitter *PE = static_cast<ParticleEmitter*>(this->getOwner()->GetComponent(COMPONENT_TYPE::PARTICLE_EMITTER));
		if (PE)
		{
			std::cout << "<<<-------*****-EMITTED-*****-------->>>" << std::endl;
			PE->EmitOnce(500);
		}
	}
	/////////////////////////////////////////////////////////////////*/


	//EXPERIMENT///////////////////////////////////////////////////////
	if (pManager->GetInputManager()->getKeyTrigger(SDL_SCANCODE_RETURN))
	{
		//TODO: switch to event
		pManager->GetGameStateManager()->SetNextState(GameState::LEVEL_1);
	}
	if (pManager->GetInputManager()->getKeyTrigger(SDL_SCANCODE_P))
	{
		//TODO: switch to event
		pManager->GetGameStateManager()->TogglePause();
	}
	if (pManager->GetInputManager()->getKeyTrigger(SDL_SCANCODE_O))
	{
		//TODO: switch to event
		pManager->GetGameStateManager()->RestartCurrentLevel();
	}
	//EXPERIMENT///////////////////////////////////////////////////////


	//Toggles debug mode
	if (pManager->GetInputManager()->getKeyTrigger(SDL_SCANCODE_TAB))
	{
		//DEBUG MODE
		pManager->GetGraphicManager()->ToggleDebugMode();
	}
}

void Controller::toggleController() 
{
	active = !active;
}

Component *Controller::createNew(GameObject *owner)
{
	return new Controller(owner, COMPONENT_TYPE::CONTROLLER);
}

void Controller::serialize(std::fstream& stream)
{
}

void Controller::deserialize(std::fstream& stream)
{
	std::cout << "DESERIALIZING CONTROLLER BEGIN" << std::endl;

	active = true; //For now this is hardcoded

	std::cout << "DESERIALIZING CONTROLLER END" << std::endl;
}

void Controller::handleEvent(Event *pEvent)
{
	if (pEvent->type == EventType::TOGGLE_CONTROLLER) 
	{
		toggleController();
	}
}