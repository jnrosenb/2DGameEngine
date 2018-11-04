#include <iostream>
#include "Controller.h"
#include "Transform.h"
#include "RigidBody2D.h"
#include "../GameObject.h"
#include "../Managers.h"

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
	//Deltatime in seconds
	float dt = deltaTime / 1000.f;

	//Later on, see if its better (in terms of architecture) to move this
	//to constructor to avoid calling it each update loop
	Transform *T = static_cast<Transform*>(getOwner()->GetComponent(COMPONENT_TYPE::TRANSFORM));
	if (!T)
	{
		std::cout << "(Controller::Update)- Transform component null" << std::endl;
		return;
	}

	if (pManager->GetInputManager()->getKeyPress(SDL_SCANCODE_UP))
	{
		float moveAmount = dt * TEMPSPEED;
		//T->Translate(0, 0, -moveAmount);
		T->Translate(0, moveAmount, 0);
		/*
		RigidBody2D *rgdbdy = static_cast<RigidBody2D*>(getOwner()->GetComponent(COMPONENT_TYPE::RIGIDBODY2D));
		if (rgdbdy != 0) 
		{
			Vector3D upForce;
			Vector3DSet(&upForce, 0, 100, 0);
			rgdbdy->AddForce(upForce);
		}
		//*/
	}
	else if (pManager->GetInputManager()->getKeyPress(SDL_SCANCODE_DOWN))
	{
		float moveAmount = dt * TEMPSPEED;
		//T->Translate(0, 0, moveAmount);
		T->Translate(0, -moveAmount, 0);
	}
	if (pManager->GetInputManager()->getKeyPress(SDL_SCANCODE_LEFT))
	{
		float moveAmount = dt * TEMPSPEED;
		T->Translate(-moveAmount, 0, 0);
		//T->Rotate(5.0f);
		//T->Translate(-0.01f, 0, 0);
	}
	else if (pManager->GetInputManager()->getKeyPress(SDL_SCANCODE_RIGHT))
	{
		float moveAmount = dt * TEMPSPEED;
		T->Translate(moveAmount, 0, 0);
		//T->Rotate(-5.0f);
		//T->Translate(0.01f, 0, 0);
	}
	if (pManager->GetInputManager()->getKeyPress(SDL_SCANCODE_SPACE))
	{
		T->Rotate(5.0f);
	}
	
	if (pManager->GetInputManager()->getKeyTrigger(SDL_SCANCODE_SPACE))
	{
	}
	else if (pManager->GetInputManager()->getKeyReleased(SDL_SCANCODE_SPACE))
	{
	}


	if (pManager->GetInputManager()->getKeyTrigger(SDL_SCANCODE_TAB))
	{
		//DEBUG MODE
		pManager->GetGraphicManager()->ToggleDebugMode();
	}
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
	std::cout << "DESERIALIZING CONTROLLER END" << std::endl;
}