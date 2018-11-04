#include "RigidBody2D.h"
#include "../Managers.h"
#include "Transform.h"
#include "../GameObject.h"
#include <string>

extern Manager *pManager;

RigidBody2D::RigidBody2D(GameObject *owner, COMPONENT_TYPE type) : 
	Component(owner, type)
{
}

RigidBody2D::~RigidBody2D()
{
}

void RigidBody2D::Update(unsigned int deltaTime)
{
	//This will be called by the GO, but wont be used for now
	shape->update();
}

void RigidBody2D::AddForce(Vector3D force) 
{
	Vector3DAdd(&mForce, &mForce, &force);
}

void RigidBody2D::LateUpdate(float deltaTime, Vector3D gravity)
{
	Vector3DAdd(&mForce, &mForce, &gravity); //For now, only this force acts on the objects

	Vector3DScale(&mAcceleration, &mForce, massInv); //Get acceleration from force

	Vector3D deltaVel;
	Vector3DScale(&deltaVel, &mAcceleration, deltaTime);
	Vector3DAdd(&mVelocity, &mVelocity, &deltaVel); //We get the velocity

	Vector3D deltaPos;
	Vector3DScale(&deltaPos, &mVelocity, deltaTime);
	Vector3DAdd(&mPos, &mPos, &deltaPos); //We get the Position

	Vector3DSet(&mForce, 0, 0, 0); //reset forces to zero

	//With the position, we set the transform
	Transform *T = static_cast<Transform*>(getOwner()->GetComponent(COMPONENT_TYPE::TRANSFORM));
	if (T != 0) 
	{
		T->Translate(deltaPos.x, deltaPos.y, deltaPos.z);
		//T->setPosition();
	}
}

Shape *RigidBody2D::GetShape()
{
	return shape;
}

Vector3D RigidBody2D::GetPosition()
{
	return mPos;
}

Component *RigidBody2D::createNew(GameObject *owner)
{
	if (owner == 0)
		return 0;

	return new RigidBody2D(owner, COMPONENT_TYPE::RIGIDBODY2D);
}

void RigidBody2D::serialize(std::fstream& stream)
{
}

void RigidBody2D::deserialize(std::fstream& stream)
{
	std::cout << "DESERIALIZING RIGIDBODY2D BEGIN-----------------------------------------------" << std::endl;

	std::string shapeType;
	float m;
	if (stream >> m >> shapeType)
	{
		mass = m;
		if (mass != 0.0f)
			massInv = 1.0f / mass;
		else
			massInv = 0.0f;

		if (shapeType == "RectangleShape") 
		{
			float w, h;
			if (stream >> w >> h) 
			{
				shape = new RectangleShape(this, ShapeType::RECTANGLE);
				RectangleShape *rect = static_cast<RectangleShape*>(shape);
				rect->setSize(w, h);
			}
			else
			{
				std::cout << "(RigidBody2D::deserialize)- Error 2.1 reading from stream" << std::endl;
			}
		}
		else if (shapeType == "CircleShape") 
		{
			float r;
			if (stream >> r)
			{
				CircleShape *circle = new CircleShape(this, ShapeType::CIRCLE);
				circle->setRadius(r);
				shape = circle;
			}
			else
			{
				std::cout << "(RigidBody2D::deserialize)- Error 2.2 reading from stream" << std::endl;
			}
		}

		//Add this component to list managed by physicsManager
		pManager->GetPhysicsManager()->addRigidBody2D(this);
	}
	else
	{
		std::cout << "(RigidBody2D::deserialize)- Error 1.0 reading from stream" << std::endl;
	}

	std::cout << "DESERIALIZING RIGIDBODY2D END-----------------------------------------------" << std::endl;
}