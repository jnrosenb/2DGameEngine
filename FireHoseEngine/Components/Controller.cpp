#include <iostream>
#include "Controller.h"
#include "Transform.h"
#include "RigidBody2D.h"
#include "Animator.h"
#include "../GameObject.h"
#include "../EventManager.h"
#include "../Events.h"

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

	//Deltatime in seconds
	float dt = deltaTime / 1000.f;
	//float horizontalSpeedImpulse = 1.0f;
	float horizontalSpeedImpulse = 0.25f;
	float JumpVelocityImpulse = 20.0f;

	//Manages vertical motion
	if (pManager->GetInputManager()->getKeyTrigger(SDL_SCANCODE_UP))
	{
		float moveAmount = dt * TEMPSPEED;
		//T->Translate(0, 0, -moveAmount);
		//T->Translate(0, moveAmount, 0);

		//*
		RigidBody2D *rgdbdy = static_cast<RigidBody2D*>(getOwner()->GetComponent(COMPONENT_TYPE::RIGIDBODY2D));
		if (rgdbdy != 0) 
		{
			//if (rgdbdy->jumping)
				//return;

			Vector3D upVel;
			Vector3DSet(&upVel, 0, JumpVelocityImpulse, 0);
			rgdbdy->setVelocity(upVel);
			
			rgdbdy->jumping = true;
		}
		//*/
	}
	else if (pManager->GetInputManager()->getKeyPress(SDL_SCANCODE_DOWN))
	{
		float moveAmount = dt * TEMPSPEED;
		//T->Translate(0, 0, moveAmount);
		//T->Translate(0, -moveAmount, 0);
	}
	
	//Manages horizontal motion
	if (pManager->GetInputManager()->getKeyPress(SDL_SCANCODE_LEFT))
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
	else if (pManager->GetInputManager()->getKeyPress(SDL_SCANCODE_RIGHT))
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

	//Rotates the object
	if (pManager->GetInputManager()->getKeyPress(SDL_SCANCODE_SPACE))
	{
		T->Rotate(5.0f);
	}

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