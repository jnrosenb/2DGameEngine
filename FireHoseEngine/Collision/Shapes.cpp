#include "Shapes.h"
#include "../Components/Component.h"
#include "../Components/Transform.h"
#include "../GameObject.h"
#include <iostream>


//ABSTRACT-SHAPE---------------------------------
Shape::Shape(Component *owner, ShapeType type)
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

Component *Shape::GetShapeOwner()
{ 
	return shapeOwner; 
}

//CIRCLE-SHAPE-----------------------------------
CircleShape::CircleShape(Component *owner, ShapeType type) :
	Shape(owner, type)
{
}

CircleShape::~CircleShape()
{
}

void CircleShape::update()
{
	//Circle shape only needs position
	Component *ownerComp = GetShapeOwner();
	if (ownerComp == 0)
		return;

	GameObject *owner = ownerComp->getOwner();
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

bool CircleShape::TestPointCollision(Vector3D point)
{
	float distanceSqr = Vector3DSquareDistance(&point, &getCenter());

	if (distanceSqr <= radius * radius)
		return true;
	return false;
}

//RECTANGLE-SHAPE-----------------------------------
RectangleShape::RectangleShape(Component *owner, ShapeType type) :
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
	Component *ownerComp = GetShapeOwner();
	if (ownerComp == 0)
		return;

	GameObject *owner = ownerComp->getOwner();
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

bool RectangleShape::TestPointCollision(Vector3D point)
{
	Vector3D axis; //TO ROTATE IN XY PLANE
	Vector3DSet(&axis, 0, 0, -1);
	Matrix3D ROT, T;
	Vector3D normals[2];
	Vector3D pos1 = getCenter(); /*CHECK IF THIS WORKS*/

	//Get the rotated vertices (for AST) -- GET THIS VERTEX CODE IN A METHOD
	// TR, TL, BR, BL;
	Vector3D vertices1[4];
	Vector3DSet(&vertices1[0], width / 2.0f, height / 2.0f, 0);
	Vector3DSet(&vertices1[1], -width / 2.0f, height / 2.0f, 0);
	Vector3DSet(&vertices1[2], width / 2.0f, -height / 2.0f, 0);
	Vector3DSet(&vertices1[3], -width / 2.0f, -height / 2.0f, 0);
	//Rotate them
	Matrix3DRotAxisDeg(&ROT, &axis, angle);
	Matrix3DMultVec(&vertices1[0], &ROT, &vertices1[0]);
	Matrix3DMultVec(&vertices1[1], &ROT, &vertices1[1]);
	Matrix3DMultVec(&vertices1[2], &ROT, &vertices1[2]);
	Matrix3DMultVec(&vertices1[3], &ROT, &vertices1[3]);
	//Translate them back
	Matrix3DTranslate(&T, pos1.x, pos1.y, 0);
	Matrix3DMultVec(&vertices1[0], &T, &vertices1[0]);
	Matrix3DMultVec(&vertices1[1], &T, &vertices1[1]);
	Matrix3DMultVec(&vertices1[2], &T, &vertices1[2]);
	Matrix3DMultVec(&vertices1[3], &T, &vertices1[3]);

	//Get the two evaluation axis
	Vector3D n1_1, n1_2;
	Vector3DSub(&n1_1, &vertices1[0], &vertices1[1]);
	Vector3DNormalize(&normals[0], &n1_1);
	Vector3DSub(&n1_2, &vertices1[0], &vertices1[2]);
	Vector3DNormalize(&normals[1], &n1_2);

	//Check if point is inside using dot product with axis
	for (int i = 0; i < 2; ++i)
	{
		float pDot = Vector3DDotProduct(&point, &normals[i]);

		float min = FLT_MAX;
		float max = -FLT_MAX;
		for (int j = 0; j < 4; ++j)
		{
			float dot = Vector3DDotProduct(&vertices1[j], &normals[i]);	
			if (dot < min) min = dot;
			if (dot > max) max = dot;
		}

		if (pDot < min || pDot > max)
			return false;
	}

	return true;
}


