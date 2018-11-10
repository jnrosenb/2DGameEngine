#ifndef RIGIDBODY2D_H
#define RIGIDBODY2D_H

#include "Component.h"
#include "../Collision/Shapes.h"
#include "../CollisionManager.h"
#include "../Math/Vector3D.h"

class Event;

class RigidBody2D : public Component
{
public:
	RigidBody2D(GameObject *owner, COMPONENT_TYPE type);
	~RigidBody2D();

	void Update(unsigned int deltaTime);
	void LateUpdate(float deltaTime); //Gravity is temporary here
	Shape *GetShape(); 
	Vector3D GetPosition();
	void AddForce(Vector3D force);
	void setVelocity(Vector3D vel);
	Vector3D GetVelocity();
	bool isDynamic();
	float getMass();
	float getBounce();

	virtual Component *createNew(GameObject *owner);
	virtual void serialize(std::fstream& stream);
	virtual void deserialize(std::fstream& stream);

private:
	void setCollisionMask(unsigned int colMask);
	void handleEvent(Event *pEvent);

public:
	CollisionMask collisionMask;	
	
	bool grounded; /*ONLY PUBLIC FOR NOW*/

private:
	//Physical properties
	bool dynamic;
	float bounciness; //0, does not bounce. 1, 100% bounce

	Shape *shape;
	float mass;
	float massInv;
	Vector3D mForce;
	Vector3D mAcceleration;
	Vector3D mVelocity;
	Vector3D mPos;
	Vector3D mPrevPos;
};

#endif