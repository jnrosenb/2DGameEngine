#ifndef SHAPES_H
#define SHAPES_H

#include "../Math/Vector3D.h"

class Component;


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
	Shape(Component *owner, ShapeType type);
	virtual ~Shape() {}

	virtual bool TestPointCollision(Vector3D point) = 0;
	virtual void update() = 0;

	ShapeType GetType();
	Component *GetShapeOwner();
	Vector3D getCenter();
	void setCenter(float x, float y, float z);
	void setOffset(float x, float y, float z);
	Vector3D getOffset();

private:
	Vector3D center;
	Vector3D offset;
	ShapeType type;
	Component *shapeOwner;
};


//------------------------------
class CircleShape : public Shape
{
public:
	CircleShape(Component *owner, ShapeType type);
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
	RectangleShape(Component *owner, ShapeType type);
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