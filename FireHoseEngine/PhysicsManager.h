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

	int Sign(float a);

private:
	//Contact resolution techniques
	void randomContactResolution(Contact *c);
	void impulseContactResolution(Contact *c);


	std::vector<RigidBody2D*> rigidBodies;
};

#endif