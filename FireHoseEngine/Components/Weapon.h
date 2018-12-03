#ifndef WEAPON_H
#define WEAPON_H

#include "Component.h"
#include "../Math/Vector3D.h"
#include <vector>

class Event;
class Projectile;
class GameObject;

class Weapon : public Component
{
public:
	Weapon(GameObject *owner, COMPONENT_TYPE type);
	virtual ~Weapon();

	virtual void Update(unsigned int deltaTime);

	virtual Component *createNew(GameObject *owner);
	virtual void serialize(std::fstream& stream);
	virtual void deserialize(std::fstream& stream);
	virtual void handleEvent(Event *pEvent);

	void Fire(GameObject *shooter, Vector3D dir);
	int getAmmoSize();
	void setAmmoSize(int size);
	void AddToAmmo(GameObject *goAmmo);
	void startPickupAnimation();
	void stopPickupAnimation();
	GameObject *getNewBullet();

private:
	//TODO check dangling pointer
	std::vector<GameObject*> ammo;
	int ammoSize;
	int currentAmmo;

	bool playerCanPickup;
	float totalTime;
	Vector3D originalScale;
};

#endif