#ifndef WEAPONSLOT_H
#define WEAPONSLOT_H

#include "Component.h"
#include "../Math/Vector3D.h"
#include <vector>

class Event;
class Weapon;

class WeaponSlot : public Component
{
public:
	WeaponSlot(GameObject *owner, COMPONENT_TYPE type);
	virtual ~WeaponSlot();

	virtual void Update(unsigned int deltaTime);

	virtual Component *createNew(GameObject *owner);
	virtual void serialize(std::fstream& stream);
	virtual void deserialize(std::fstream& stream);
	virtual void handleEvent(Event *pEvent);

	void AddWeaponToPickList(Weapon *weapon);
	void RemoveWeaponFromPickList(Weapon *weapon);
	void PickWeaponUp();
	void DropWeapon();
	void VisualDrop();
	void Fire();
	void Fire(Vector3D direction);
	bool hasWeapon();
	void EquipWeaponDirectly(Weapon *w);

private:
	//TODO check dangling pointer
	Weapon *weapon;

	//TODO check dangling pointer
	std::vector<Weapon*> weaponsToPick;
};

#endif