#include "Transform.h"
#include <cstring>
#include <iostream>


Transform::Transform(GameObject *owner, COMPONENT_TYPE type) :
	Component(owner, type)
{
	Vector3DSet(&mPosition, 0.0f, 0.0f, 0.0f);
	Vector3DSet(&mScale, 1.0f, 1.0f, 1.0f);
	mAngle = 0.0f;

	setInitialMatrix(&T);
	setInitialMatrix(&H);
	setInitialMatrix(&R);
	setInitialMatrix(&M);
}

Transform::~Transform()
{
}

void Transform::Update(unsigned int deltaTime)
{
	//Translation;
	Matrix3DTranslate(&T, mPosition.x, mPosition.y, mPosition.z);
	Matrix3DTranspose(&T, &T); //OpenGl takes it in column major order

	//Scale
	Matrix3DScale(&H, mScale.x, mScale.y, mScale.z);

	//Rotate
	Vector3D rotationAxis;
	Vector3DSet(&rotationAxis, 0, 0, 1);
	Matrix3DRotAxisDeg(&R, &rotationAxis, mAngle);
	//Matrix3DPrint(&R, "ROTATION:");

	//Model matrix
	Matrix3D temp;
	Matrix3DConcat(&temp, &H, &R);
	Matrix3DConcat(&M, &temp, &T);
}

void Transform::getShapeModel(Matrix3D *UM, Vector3D pos)
{
	//Returns the shape model, which is unscaled and
	//offsetted by offset
	Matrix3D shapeT;
	Matrix3DTranslate(&shapeT, pos.x, pos.y, pos.z);
	Matrix3DTranspose(&shapeT, &shapeT); //OpenGl takes it in column major order
	Matrix3DConcat(UM, &R, &shapeT);
}

void Transform::Translate(float dx, float dy, float dz)
{
	mPosition.x += dx;
	mPosition.y += dy;
	mPosition.z += dz;
}

void Transform::Scale(float x, float y, float z)
{
	Vector3DSet(&mScale, x, y, z);
}

void Transform::Rotate(float angle)
{
	mAngle += angle;
}

void Transform::setInitialMatrix(Matrix3D *Matrix)
{
	Matrix3DIdentity(Matrix);
}

Vector3D const& Transform::getPosition()
{
	return mPosition;
}

Vector3D const& Transform::getScale()
{
	return mScale;
}

float Transform::getAngle() 
{
	return mAngle;
}

Component *Transform::createNew(GameObject *owner)
{
	return new Transform(owner, COMPONENT_TYPE::TRANSFORM);
}

void Transform::serialize(std::fstream& stream)
{
}

void Transform::deserialize(std::fstream& stream)
{
	std::cout << "DESERIALIZING TRANSFORM BEGIN" << std::endl;
	
	//TODO: Replace with safe way of doing
	
	float x, y, z, sx, sy, sz, angle;

	if (stream >> x >> y >> z >> sx >> sy >> sz >> angle) 
	{
		mPosition.x = x;
		mPosition.y = y;
		mPosition.z = z;

		mScale.x = sx;
		mScale.y = sy;
		mScale.z = sz;

		mAngle = angle;

		setInitialMatrix(&T);
		setInitialMatrix(&H);
		setInitialMatrix(&R);
		setInitialMatrix(&M);
	}
	else 
	{
		std::cout << "(Transform::deserialize)- Error reading from stream" << std::endl;
	}

	std::cout << "DESERIALIZING TRANSFORM END" << std::endl;
}