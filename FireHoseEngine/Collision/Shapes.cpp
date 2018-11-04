#include "Shapes.h"
#include "../Components/RigidBody2D.h"
#include "../Components/Transform.h"
#include "../GameObject.h"
#include <iostream>


//ABSTRACT-SHAPE---------------------------------
Shape::Shape(RigidBody2D *owner, ShapeType type)
{
	this->type = type;
	this->shapeOwner = owner;
}

ShapeType Shape::GetType()
{
	return type; 
}

Vector3D Shape::getCenter()
{
	return center;
}

void Shape::setCenter(float x, float y, float z)
{
	Vector3DSet(&center, x, y, z);
}

RigidBody2D *Shape::GetShapeOwner()
{ 
	return shapeOwner; 
}

//CIRCLE-SHAPE-----------------------------------
CircleShape::CircleShape(RigidBody2D *owner, ShapeType type) :
	Shape(owner, type)
{
}

CircleShape::~CircleShape()
{
}

void CircleShape::update()
{
	//Circle shape only needs position
	RigidBody2D *rgbdy = GetShapeOwner();
	if (rgbdy == 0)
		return;

	GameObject *owner = rgbdy->getOwner();
	Transform *T = static_cast<Transform*>(owner->GetComponent(COMPONENT_TYPE::TRANSFORM));
	if (T == 0)
		return;

	this->setCenter(T->getPosition().x, T->getPosition().y, T->getPosition().z);
}

float CircleShape::getRadius()
{
	return radius;
}

void CircleShape::setRadius(float r)
{
	radius = r;
}

bool CircleShape::TestPointCollision()
{
	return false;
}

//RECTANGLE-SHAPE-----------------------------------
RectangleShape::RectangleShape(RigidBody2D *owner, ShapeType type) :
	Shape(owner, type)
{
}

RectangleShape::~RectangleShape()
{
}

void RectangleShape::update()
{
	//The rectangle shape wants to know 
	//the position and rotation of GO
	RigidBody2D *rgbdy = GetShapeOwner();
	if (rgbdy == 0)
		return;

	GameObject *owner = rgbdy->getOwner();
	Transform *T = static_cast<Transform*>(owner->GetComponent(COMPONENT_TYPE::TRANSFORM));
	if (T == 0)
		return;

	this->setCenter(T->getPosition().x, T->getPosition().y, T->getPosition().z);
	angle = T->getAngle();
}

Vector3D RectangleShape::getSize()
{
	Vector3D size;
	Vector3DSet(&size, width, height, 0);
	return size;
}

float RectangleShape::getAngle()
{
	return angle;
}

void RectangleShape::setSize(float w, float h)
{
	width = w;
	height = h;
}

bool RectangleShape::TestPointCollision()
{
	return false;
}


