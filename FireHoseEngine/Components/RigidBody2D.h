#ifndef RIGIDBODY2D_H
#define RIGIDBODY2D_H

#include "Component.h"
#include "../Collision/Shapes.h"
#include "../Math/Vector3D.h"


class RigidBody2D : public Component
{
public:
	RigidBody2D(GameObject *owner, COMPONENT_TYPE type);
	~RigidBody2D();

	void Update(unsigned int deltaTime);
	void LateUpdate(float deltaTime, Vector3D gravity); //Gravity is temporary here
	Shape *GetShape(); 
	Vector3D GetPosition();
	void AddForce(Vector3D force);
	bool isDynamic();

	virtual Component *createNew(GameObject *owner);
	virtual void serialize(std::fstream& stream);
	virtual void deserialize(std::fstream& stream);

private:
	bool dynamic;
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