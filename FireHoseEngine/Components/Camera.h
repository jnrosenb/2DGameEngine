#ifndef CAMERA_H
#define CAMERA_H

#include "Component.h"
#include "../GameObject.h"
#include "../Math/Vector3D.h"
#include "../Math/Matrix3D.h"

class Transform;
class Event;

class Camera : public Component
{
public:
	Camera(GameObject *owner, COMPONENT_TYPE type);
	~Camera();

	void Update(unsigned int deltaTime);
	void cameraSetup(float fov, float n, float f, float ar, float width, float distanceToGO, bool isOrtho);
	void SynchronizePositionWithGO();
	void FollowTarget(float deltaTime);

	void setTarget(GameObject *target);
	void setTargetFor(GameObject *target, float seconds, float followSpeed);
	void resetTarget();

	Vector3D const& GetLook();
	Vector3D const& GetUp();
	Vector3D const& GetRight();
	Vector3D const& GetTranslationDelta();
	float getOrtoWidth();
	float getAspect();

	Matrix3D GetViewMatrix();
	Matrix3D GetProjection();
	Matrix3D GetPerspectiveMatrix();
	Matrix3D GetOrthographicMatrix(float width);

	virtual Component *createNew(GameObject *owner);
	virtual void serialize(std::fstream& stream);
	virtual void deserialize(std::fstream& stream);
	virtual void handleEvent(Event *pEvent);

private:
	float fov, near, far, aspect, width, distanceToGO;
	Vector3D eye;
	Vector3D look;
	Vector3D up;
	Vector3D right;
	float xTolerance, yTolerance;
	bool isOrtho;
	Vector3D translationDelta;

	//Smooth interp following
	float currentfollowSpeed;
	float playerfollowSpeed;
	Vector3D origin;
	Vector3D destination;
	GameObject *target;
};

#endif