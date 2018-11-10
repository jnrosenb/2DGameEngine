#ifndef PHYSICSMANAGER_H
#define PHYSICSMANAGER_H

#include <vector>

class Contact;
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
	//Contact resolution techniques
	void randomContactResolution(Contact *c);
	void impulseContactResolution(Contact *c);
	int Sign(float a);

	std::vector<RigidBody2D*> rigidBodies;
};

#endif