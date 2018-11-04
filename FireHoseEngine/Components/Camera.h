#ifndef CAMERA_H
#define CAMERA_H

#include "Component.h"
#include "../GameObject.h"
#include "../Math/Vector3D.h"
#include "../Math/Matrix3D.h"

class Camera : public Component
{
public:
	Camera(GameObject *owner, COMPONENT_TYPE type);
	~Camera();

	void Update(unsigned int deltaTime);
	void cameraSetup(float fov, float n, float f, float ar, float width, float distanceToGO, bool isOrtho);

	Vector3D const &GetLook();
	Vector3D const &GetUp();
	Vector3D const &GetRight();

	Matrix3D GetViewMatrix();
	Matrix3D GetProjection();
	Matrix3D GetPerspectiveMatrix();
	Matrix3D GetOrthographicMatrix(float width);

	virtual Component *createNew(GameObject *owner);
	virtual void serialize(std::fstream& stream);
	virtual void deserialize(std::fstream& stream);

private:
	float fov, near, far, aspect, width, distanceToGO;
	Vector3D look;
	Vector3D up;
	Vector3D right;
	bool isOrtho;
};

#endif