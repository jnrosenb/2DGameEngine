#include <iostream>
#include "Weapon.h"
#include "Transform.h"
#include "Projectile.h"
#include "../Managers.h"
#include "../GameObject.h"

extern Manager *pManager;


Weapon::Weapon(GameObject *owner, COMPONENT_TYPE type) :
	Component(owner, type)
{
}

Weapon::~Weapon()
{
	//TODO check if the bullets are being frees by the goManager or this method
	ammo.clear();
}

void Weapon::Update(unsigned int deltaTime)
{
	totalTime += deltaTime / 1000.0f;

	Transform *T = static_cast<Transform*>(getOwner()->GetComponent(COMPONENT_TYPE::TRANSFORM));
	if (T != 0 && playerCanPickup)
	{
		float aux = fabs(0.5f * cos(totalTime));
		T->Scale(originalScale.x + aux, originalScale.y + aux, originalScale.z);
	}
}

void Weapon::Fire(GameObject *shooter, Vector3D dir)
{
	if (!shooter)
	{
		std::cout << "(Weapon::Fire)- Shooter pointer is nullptr" << std::endl;
		return;
	}

	//TODO REDO Get a bullet from list, activates it, and fires it
	GameObject *bullet = getNewBullet();

	Projectile *P = static_cast<Projectile*>(bullet->GetComponent(COMPONENT_TYPE::PROJECTILE));
	if (P)
	{
		P->Fire(shooter, dir);
	}
}

Component *Weapon::createNew(GameObject *owner)
{
	return new Weapon(owner, COMPONENT_TYPE::WEAPON);
}

void Weapon::serialize(std::fstream& stream)
{
}

void Weapon::deserialize(std::fstream& stream)
{
	std::cout << "DESERIALIZING WEAPON BEGIN" << std::endl;
	
	int ammoS;
	if (stream >> ammoS)
	{
		this->ammoSize = ammoS;
		ammo.reserve(ammoSize);
		currentAmmo = 0;
	}
	else
	{
		std::cout << "(Weapon::deserialize)- Error reading the stream" << std::endl;
	}

	std::cout << "DESERIALIZING WEAPON END" << std::endl;
}


void Weapon::handleEvent(Event *pEvent)
{
	if (pEvent->type == EventType::ON_ENTER_TRIGGER) 
	{
		startPickupAnimation();
	}
	else if (pEvent->type == EventType::ON_EXIT_TRIGGER)
	{
		//This and above should only run if this gun is the one that fires the event
		stopPickupAnimation();
	}
}

void Weapon::startPickupAnimation()
{
	playerCanPickup = true;
	
	totalTime = 0.0f;
	Transform *T = static_cast<Transform*>(getOwner()->GetComponent(COMPONENT_TYPE::TRANSFORM));
	if (T != 0 && playerCanPickup)
	{
		originalScale = T->getScale();
	}
}

void Weapon::stopPickupAnimation()
{
	playerCanPickup = false;

	Transform *T = static_cast<Transform*>(getOwner()->GetComponent(COMPONENT_TYPE::TRANSFORM));
	if (T != 0)
	{
		T->Scale(originalScale.x, originalScale.y, originalScale.z);
	}
}

void Weapon::AddToAmmo(GameObject *goAmmo) 
{
	if (goAmmo)
		ammo.push_back(goAmmo);
}

int Weapon::getAmmoSize()
{
	return ammoSize;
}

void Weapon::setAmmoSize(int size)
{
	ammoSize = size;
}


GameObject *Weapon::getNewBullet()
{
	//TODO Redo all this function
	GameObject *b = ammo[currentAmmo];
	currentAmmo = (currentAmmo + 1) % ammoSize;
	return b;
}