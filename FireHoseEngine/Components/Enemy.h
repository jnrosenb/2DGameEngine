#ifndef ENEMY_H
#define ENEMY_H

#include "Character.h"
#include "../Managers.h"
#include "../Events.h"

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
	void OnDying();
	void OnDyingAnimationEnd();

private:
	int maxHealth;
	int health;
	float invulnerabilityTime;
	float timeSinceHit;
};


class EnemyDeathCallback : public callbackEvent
{
public:
	//AnimatorCallback() : callbackEvent()
	EnemyDeathCallback(Enemy *obj, void (Enemy::*mthd)())
	{
		object = obj;
		method = mthd;
	}

	virtual ~EnemyDeathCallback() { }

	virtual void callback()
	{
		(object->*method)();
	}

private:
	Enemy *object;
	void (Enemy::*method)();

private:
	EnemyDeathCallback(EnemyDeathCallback const& rhs);
	EnemyDeathCallback& operator=(EnemyDeathCallback const& rhs);
};


#endif