#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "Component.h"
#include "../Math/Vector3D.h"
#include "../Math/Matrix3D.h"

class Event;

class Transform : public Component
{
public:
	Transform(GameObject *owner, COMPONENT_TYPE type);
	~Transform();

	void Update(unsigned int deltaTime);

	void Translate(float dx, float dy, float dz);
	void Scale(float x, float y, float z);
	void Rotate(float angle);

	Vector3D const& getPosition();
	Vector3D const& getScale();
	float getAngle();
	void getShapeModel(Matrix3D *UM, Vector3D offset);

	virtual Component *createNew(GameObject *owner);
	virtual void serialize(std::fstream& stream);
	virtual void deserialize(std::fstream& stream);
	virtual void handleEvent(Event *pEvent);

	Vector3D const& GetDeltaPos(); // FOR NOW ONLY WORKS ON X AXIS

public:
	Matrix3D M;

private:
	void setInitialMatrix(Matrix3D *Matrix);

private:
	Vector3D initialScale;
	Vector3D mPosition;
	Vector3D mDeltaPos;
	Vector3D mScale;
	float mAngle;
	Matrix3D H, T, R; //, M[16];
};

#endif