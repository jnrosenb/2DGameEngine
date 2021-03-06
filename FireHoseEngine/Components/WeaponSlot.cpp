#include <iostream>
#include "WeaponSlot.h"
#include "Weapon.h"
#include "Camera.h"
#include "Trigger.h"
#include "Renderer.h"
#include "RigidBody2D.h"
#include "Transform.h"
#include "../Managers.h"
#include "../GameObject.h"
#include "../EventManager.h"
#include "../Events.h"

extern Manager *pManager;


WeaponSlot::WeaponSlot(GameObject *owner, COMPONENT_TYPE type) :
	Component(owner, type)
{
	weapon = 0;
}

WeaponSlot::~WeaponSlot()
{
	weaponsToPick.clear();
}

void WeaponSlot::Fire() 
{
	if (weapon == 0)
		return;

	//Get width and height from graphicsManager
	int hW = pManager->GetGraphicManager()->width / 2; 
	int hH = pManager->GetGraphicManager()->height / 2;

	//Get mouse direction for firing projectile
	int x = pManager->GetInputManager()->getMouseX() - hW;
	int y = -(pManager->GetInputManager()->getMouseY() - hH);

	//Now get direction in which to fire projectile
	Vector3D dir;
	Vector3DSet(&dir, static_cast<float>(x), static_cast<float>(y), 0.0f);
	Vector3DNormalize(&dir, &dir);
	weapon->Fire(this->getOwner(), dir);

	//PRINTS CORRECTED MOUSE POSITIONS
	///std::cout << "MouseX: " << x << ", MouseY: " << y << "-  ";
	///Vector3DPrint(&dir);
	///std::cout << std::endl;

	/*
	Transform *T = static_cast<Transform*>(this->getOwner()->GetComponent(COMPONENT_TYPE::TRANSFORM));
	Camera *C = static_cast<Camera*>(this->getOwner()->GetComponent(COMPONENT_TYPE::CAMERA));
	if (T && C)
	{
		float halfViewportWidth = C->GetWidth().x / 2.0f;
		float halfViewportHeight = C->GetWidth().y;

		float xProportion = T->getPosition().x / halfViewportWidth;
		float yProportion = T->getPosition().y / halfViewportHeight;

		float xScreen = xProportion * hW;
		float yScreen = - yProportion * hH;

		Vector3D playerScreenPos;
		Vector3DSet(&playerScreenPos, xScreen, yScreen, 0.0f);

		Vector3D mouseScreenPos;
		Vector3DSet(&mouseScreenPos, x, y, 0.0f);

		//PRINT
		std::cout << "PlayerPos: "; Vector3DPrint(&playerScreenPos);
		std::cout << "MousePos : "; Vector3DPrint(&mouseScreenPos);

		Vector3D dir;
		Vector3DSub(&dir, &mouseScreenPos, &playerScreenPos);
		Vector3DNormalize(&dir, &dir);

		//Fire the weapon
		weapon->Fire(this->getOwner(), dir);
	}//*/
}


void WeaponSlot::Fire(Vector3D direction)
{
	if (weapon == 0)
		return;

	//Fire the weapon
	weapon->Fire(this->getOwner(), direction);
}

void WeaponSlot::Update(unsigned int deltaTime)
{
	if (weapon)
	{
		Transform *T1 = static_cast<Transform*>(getOwner()->GetComponent(COMPONENT_TYPE::TRANSFORM));
		Transform *T2 = static_cast<Transform*>(weapon->getOwner()->GetComponent(COMPONENT_TYPE::TRANSFORM));
		if (T1 && T2)
		{
			Vector3D pos = T1->getPosition();
			Vector3D cpos = T2->getPosition();
			Vector3D c3;
			Vector3DSub(&c3, &pos, &cpos);

			T2->Translate(c3.x, c3.y, 0.0f);
		}
	}
}

bool WeaponSlot::hasWeapon()
{
	bool hasW = (weapon != 0) ? 1 : 0;
	return hasW;
}

void WeaponSlot::PickWeaponUp()
{
	//Here we choose a weapon from list, pick it up, then place
	//it as the default weapon, and get it out of the pick list
	if (!weaponsToPick.empty()) 
	{
		std::cout << "PICKED WEAPON" << std::endl;
		Weapon *choice = weaponsToPick.back();
		weaponsToPick.pop_back();
		
		//AFTER CHOOSING, BUT BEFORE TAKING WEAPON, 
		//WE NEED TO DROP CURRENT WEAPON
		if (weapon)
		{
			DropWeapon();
		}

		//Visual stuff and re-enable trigger
		Trigger * Tr = static_cast<Trigger*>(choice->getOwner()->GetComponent(COMPONENT_TYPE::TRIGGER));
		Renderer * Rn = static_cast<Renderer*>(choice->getOwner()->GetComponent(COMPONENT_TYPE::RENDERER));
		if (Tr && Rn)
		{
			//TODO Check what happens to player ref that was inside of trigger (he should be eliminated from there maybe)
			Tr->setEnabled(false);
			//Tr->disableTrigger();
			Rn->setEnabled(false);

			this->weapon = choice;
		}
		else
		{
			std::cout << "(WeaponSlot::PickWeapon)- Error, no trigger comp in weapon" << std::endl;
		}
	}
}

void WeaponSlot::DropWeapon()
{
	if (weapon != 0) 
	{
		std::cout << "DROPPED WEAPON" << std::endl;

		//Visual stuff and re-enable trigger
		Trigger * Tr = static_cast<Trigger*>(weapon->getOwner()->GetComponent(COMPONENT_TYPE::TRIGGER));
		Renderer * Rn = static_cast<Renderer*>(weapon->getOwner()->GetComponent(COMPONENT_TYPE::RENDERER));
		if (Tr && Rn)
		{
			Tr->setEnabled(true);
			//Tr->enableTrigger();
			Rn->setEnabled(true);

			//Launch the weapon backwards up
			VisualDrop();

			this->weapon = 0;
		}
		else 
		{
			std::cout << "(WeaponSlot::DropWeapon)- Error, no trigger comp in weapon" << std::endl;
		}
	}
}


void WeaponSlot::VisualDrop() 
{
	RigidBody2D *weaponRB = static_cast<RigidBody2D*>(weapon->getOwner()->GetComponent(COMPONENT_TYPE::RIGIDBODY2D));
	RigidBody2D *playerRB = static_cast<RigidBody2D*>(getOwner()->GetComponent(COMPONENT_TYPE::RIGIDBODY2D));
	Transform *wTransform = static_cast<Transform*>(weapon->getOwner()->GetComponent(COMPONENT_TYPE::TRANSFORM));
	if (weaponRB && playerRB && wTransform)
	{
		Vector3D playerVelocity = playerRB->GetVelocity();
		Vector3D weaponPos = wTransform->getPosition();
		int sign = (playerVelocity.x > 0.0f) ? -1 : 1;

		float dropPower = 10.0f;
		Vector3D dropVelocity;
		Vector3DSet(&dropVelocity, sign * dropPower, dropPower, 0.0f);
		weaponRB->setVelocity(dropVelocity);
	}
}


void WeaponSlot::AddWeaponToPickList(Weapon *weapon)
{
	if (weapon) 
	{
		std::cout << "ADDED WEAPON TO PICK STACK" << std::endl;
		weaponsToPick.push_back(weapon);
	}
}

void WeaponSlot::EquipWeaponDirectly(Weapon *w)
{
	//Visual stuff and re-enable trigger
	Trigger * Tr = static_cast<Trigger*>(w->getOwner()->GetComponent(COMPONENT_TYPE::TRIGGER));
	Renderer * Rn = static_cast<Renderer*>(w->getOwner()->GetComponent(COMPONENT_TYPE::RENDERER));
	if (Tr && Rn)
	{
		//TODO Check what happens to player ref that was inside of trigger (he should be eliminated from there maybe)
		Tr->setEnabled(false);
		//Tr->disableTrigger();
		//Rn->setEnabled(false);
		this->weapon = w;
	}
	else
	{
		std::cout << "(WeaponSlot::SetWeapon)- Error, no trigger or renderer comp in weapon" << std::endl;
	}
}

void WeaponSlot::RemoveWeaponFromPickList(Weapon *weapon)
{
	std::vector<Weapon*>::iterator it_b = weaponsToPick.begin();
	std::vector<Weapon*>::iterator it_e = weaponsToPick.end();
	for (; it_b != it_e; ++it_b) 
	{
		if (*it_b == weapon) 
		{
			std::cout << "REMOVED WEAPON FROM PICK STACK" << std::endl;
			weaponsToPick.erase(it_b);
			return;
		}
	}
}

Component *WeaponSlot::createNew(GameObject *owner)
{
	return new WeaponSlot(owner, COMPONENT_TYPE::WEAPON_SLOT);
}

void WeaponSlot::serialize(std::fstream& stream)
{
}

void WeaponSlot::deserialize(std::fstream& stream)
{
	std::cout << "DESERIALIZING WEAPON SLOT BEGIN" << std::endl;

	if (stream)
	{
		weapon = 0;
	}
	else
	{
		std::cout << "(WeaponSlot::deserialize)- Error reading the stream" << std::endl;
	}

	std::cout << "DESERIALIZING WEAPON SLOT END" << std::endl;
}


void WeaponSlot::ResetWeaponsBulletMask(CollisionMask newMask)
{
	if (weapon) 
	{
		weapon->ResetBulletsMask(newMask);
	}
}


void WeaponSlot::handleEvent(Event *pEvent)
{
	if (pEvent->type == EventType::ON_ENTER_TRIGGER) 
	{
		std::vector<std::string> eventKeys = this->getOwner()->getEventKeys(pEvent->type);
		for (std::string key : eventKeys)
		{
			if (key == pEvent->eventKey && key == "triggerGun") //TODO - DeHardcode this
			{
				OnEnterTriggerEvent *ev = static_cast<OnEnterTriggerEvent*>(pEvent);
				if (ev && !ev->isYourTrigger) 
				{
					GameObject *otherGo = ev->pTrigger->getOwner();
					if (otherGo)
					{
						Weapon *W = static_cast<Weapon*>(otherGo->GetComponent(COMPONENT_TYPE::WEAPON));
						if (W)
						{
							AddWeaponToPickList(W);
						}
					}
				}
			}
		}
	}
	if (pEvent->type == EventType::ON_EXIT_TRIGGER)
	{
		std::vector<std::string> eventKeys = this->getOwner()->getEventKeys(pEvent->type);
		for (std::string key : eventKeys)
		{
			if (key == pEvent->eventKey && key == "triggerGun")
			{
				OnExitTriggerEvent *ev = static_cast<OnExitTriggerEvent*>(pEvent);
				if (ev && !ev->isYourTrigger)
				{
					GameObject *otherGo = ev->pTrigger->getOwner();
					if (otherGo)
					{
						Weapon *W = static_cast<Weapon*>(otherGo->GetComponent(COMPONENT_TYPE::WEAPON));
						if (W)
						{
							RemoveWeaponFromPickList(W);
						}
					}
				}
			}
		}
	}
}
