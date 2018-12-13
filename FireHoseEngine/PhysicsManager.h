/* Start Header -------------------------------------------------------
*  Copyright (C) 2018 DigiPen Institute of Technology.
*  Reproduction or disclosure of this file or its contents without the prior
*  written consent of DigiPen Institute of Technology is prohibited.

*  File Name: InputManager.h
*  Purpose: Main source file. Contains the main loop, and is in charge of
*  creating and destroying all the managers used in the engine.
*  Project: CS529_JRosenbluth_3
*  Author: Jose Rosenbluth Chiu, J.Rosenbluth, 60003418
*  Creation date: 10/14/2018
-  End Header --------------------------------------------------------*/

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

	void FreeInstances();
	void Update();
	void LateUpdate(unsigned int deltaTime);
	void addRigidBody2D(RigidBody2D *rby);
	void addDynamicRigidBody2D(RigidBody2D *rby);

	std::vector<RigidBody2D*> const& getDynamicBodies();

private:
	//Contact resolution techniques
	void impulseContactResolution(Contact *c);
	int Sign(float a);

	std::vector<RigidBody2D*> rigidBodies;
	std::vector<RigidBody2D*> dynamicRgdbs;
};

#endif