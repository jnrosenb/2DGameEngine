#include <iostream>
#include "WeaponSlot.h"
#include "Weapon.h"
#include "Trigger.h"
#include "Renderer.h"
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
	float z = -1.0f;

	//Now get direction in which to fire projectile
	Vector3D dir;
	Vector3DSet(&dir, static_cast<float>(x), static_cast<float>(y), 0.0f);
	Vector3DNormalize(&dir, &dir);

	//PRINTS CORRECTED MOUSE POSITIONS
	std::cout << "MouseX: " << x << ", MouseY: " << y << "-  ";
	Vector3DPrint(&dir);

	//Fire the weapon
	weapon->Fire(this->getOwner(), dir);
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
			Tr->disableTrigger();
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
			Tr->enableTrigger();
			Rn->setEnabled(true);

			this->weapon = 0;
		}
		else 
		{
			std::cout << "(WeaponSlot::DropWeapon)- Error, no trigger comp in weapon" << std::endl;
		}
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
				GameObject *otherGo = ev->other->getOwner();
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
	if (pEvent->type == EventType::ON_EXIT_TRIGGER)
	{
		std::vector<std::string> eventKeys = this->getOwner()->getEventKeys(pEvent->type);
		for (std::string key : eventKeys)
		{
			if (key == pEvent->eventKey && key == "triggerGun")
			{
				OnExitTriggerEvent *ev = static_cast<OnExitTriggerEvent*>(pEvent);
				GameObject *otherGo = ev->other->getOwner();
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
