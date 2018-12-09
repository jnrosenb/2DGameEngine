#ifndef ENEMY_H
#define ENEMY_H

#include "Character.h"
#include "../Managers.h"

class Event;
class GameObject;


class Enemy : public Character
{
public:
	Enemy(GameObject *owner, COMPONENT_TYPE type);
	virtual ~Enemy();

	void Update(unsigned int deltaTime);

	virtual Component *createNew(GameObject *owner);
	virtual void serialize(std::fstream& stream);
	virtual void deserialize(std::fstream& stream);
	virtual void handleEvent(Event *pEvent);

	void TakeDamage(int damage = 1);
	void TakeDamage(Vector3D launchDir, int damage = 1);

private:
	int maxHealth;
	int health;
	float invulnerabilityTime;
	float timeSinceHit;
};

#endif