#ifndef SHAPES_H
#define SHAPES_H

#include "../Math/Vector3D.h"

class RigidBody2D;


//Num is used as a length argument
enum ShapeType
{
	CIRCLE = 0,
	RECTANGLE,
	NUM
};


//---------
class Shape
{
public:
	Shape(RigidBody2D *owner, ShapeType type);
	virtual ~Shape() {}

	virtual bool TestPointCollision(Vector3D point) = 0;
	virtual void update() = 0;

	ShapeType GetType();
	RigidBody2D *GetShapeOwner();
	Vector3D getCenter();
	void setCenter(float x, float y, float z);

private:
	Vector3D center;
	ShapeType type;
	RigidBody2D *shapeOwner;
};


//------------------------------
class CircleShape : public Shape
{
public:
	CircleShape(RigidBody2D *owner, ShapeType type);
	virtual ~CircleShape();

	virtual void update();

	//virtual Vector3D getCenter();
	//virtual void setCenter(float x, float y, float z);
	float getRadius();
	void setRadius(float r);

	virtual bool TestPointCollision(Vector3D point);

private:
	float radius;
	//Vector3D center;
};


//---------------------------------
class RectangleShape : public Shape
{
public:
	RectangleShape(RigidBody2D *owner, ShapeType type);
	virtual ~RectangleShape();

	virtual void update();

	//virtual Vector3D getCenter();
	//virtual void setCenter(float x, float y, float z);
	void setSize(float w, float h);
	Vector3D getSize();
	float getAngle();

	virtual bool TestPointCollision(Vector3D point);

private:
	//Vector3D center;
	float angle;
	float width, height;
};

#endif