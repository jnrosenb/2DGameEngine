#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <fstream>
#include "Component.h"
#include "../Math/Vector3D.h"

class GameObject;
class Event;


//ENUM (CHECK IF DELETE LATER)
enum class ProjectileType
{
	NON_PHYSICS,
	PHYSICS,
	NUM
};


//////////////////////////////////////
////  BASE PROJECTILE CLASS       ////
//////////////////////////////////////
class Projectile : public Component
{
public:
	Projectile(GameObject *owner, COMPONENT_TYPE type);
	virtual ~Projectile();

	virtual void Update(unsigned int deltaTime) = 0;

	virtual Component *createNew(GameObject *owner) = 0;
	virtual void serialize(std::fstream& stream) = 0;
	virtual void deserialize(std::fstream& stream) = 0;
	virtual void handleEvent(Event *pEvent) = 0;

	virtual void resetState() = 0;
	virtual void Fire(GameObject *shooter, Vector3D dir) = 0;

	ProjectileType getType();
	void setProjectileType(ProjectileType t);
	void checkIfOutOfBounds();
	
	//Fired position is saved to know when to reset projectile
	void setFiredPosition(Vector3D fPos);

	//This is used so the enabling will happen the frame after firing
	void setIsFiringOnNextFrame(bool flag);
	bool getIsFiringOnNextFrame();

private:
	ProjectileType projectiletype;
	Vector3D firedPosition;
	bool isFiringOnNextFrame;
};


//////////////////////////////////////
////  PHYSICS PROJECTILE CLASS    ////
//////////////////////////////////////
class PhysicsProjectile : public Projectile
{
public:
	PhysicsProjectile(GameObject *owner, COMPONENT_TYPE type);
	virtual ~PhysicsProjectile();

	virtual void Update(unsigned int deltaTime);

	virtual Component *createNew(GameObject *owner);
	virtual void serialize(std::fstream& stream);
	virtual void deserialize(std::fstream& stream);
	virtual void handleEvent(Event *pEvent);

	virtual void resetState();
	virtual void Fire(GameObject *shooter, Vector3D dir);

private:
	bool isFired;

	bool delayEffect;
	float delayTime;
	float timeElapsed;
};


//////////////////////////////////////
////  STRAIGHT PROJECTILE CLASS   ////
//////////////////////////////////////
class StraightProjectile : public Projectile
{
public:
	StraightProjectile(GameObject *owner, COMPONENT_TYPE type);
	virtual ~StraightProjectile();

	virtual void Update(unsigned int deltaTime);

	virtual Component *createNew(GameObject *owner);
	virtual void serialize(std::fstream& stream);
	virtual void deserialize(std::fstream& stream);
	virtual void handleEvent(Event *pEvent);

	virtual void resetState();
	virtual void Fire(GameObject *shooter, Vector3D dir);

private:
	Vector3D direction;
	bool isFired;
};

#endif