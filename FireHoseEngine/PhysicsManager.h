#ifndef PHYSICSMANAGER_H
#define PHYSICSMANAGER_H

#include <vector>

class RigidBody2D;

class PhysicsManager
{
public:
	PhysicsManager();
	~PhysicsManager();

	void Update();
	void LateUpdate(unsigned int deltaTime);
	void addRigidBody2D(RigidBody2D *rby);

private:
	std::vector<RigidBody2D*> rigidBodies;
};

#endif