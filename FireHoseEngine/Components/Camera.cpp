#include <iostream>
#include "Camera.h"
#include "Transform.h"
#include <cmath>
#include "../Events.h"
#include "../EventManager.h"
#include "../Managers.h"
#include "../Math/Vector3D.h"

using namespace std;

#define EPSILON 0.000001f
#define PI 3.1415926535897932384626433832795

extern Manager *pManager;


Camera::Camera(GameObject *owner, COMPONENT_TYPE type) :
	Component(owner, type), fov(90.0f), near(1.0f), far(20.0f), aspect(1.0f), 
	distanceToGO(5.0f), isOrtho(false), width(10.0f)
{
	Vector3DSet(&look, 0, 0, -1);
	Vector3DSet(&up, 0, 1, 0);
	Vector3DSet(&right, 1, 0, 0);

	cout << "CAMERA COMPONENT CREATED" << endl;
}

Camera::~Camera()
{
	cout << "CAMERA COMPONENT DESTRUCTED" << endl;
}

void Camera::Update(unsigned int deltaTime)
{
	FollowTarget(deltaTime / 1000.0f);
}

void Camera::cameraSetup(float fov, float n, float f, float ar, float width, float distanceToGO, bool isOrtho)
{
	if (fabs(n - f) <= EPSILON || fabs(n) <= EPSILON || fabs(f) <= EPSILON || fov <= 0.0f || ar <= 0.0f)
	{
		cout << "(Camera::GetOrthographicMatrix)- Error, AR, FOV, near and/or far defined incorrectly" << endl;
		return;
	}

	this->isOrtho = isOrtho;

	this->distanceToGO = distanceToGO;
	this->width = width;
	this->aspect = ar;
	this->fov = fov;
	this->near = n;
	this->far = f;

	this->target = getOwner();
}

void Camera::SynchronizePositionWithGO()
{
	Transform *T = static_cast<Transform*>(getOwner()->GetComponent(COMPONENT_TYPE::TRANSFORM));
	if (T != 0)
	{
		Vector3D goPosition = T->getPosition();
		Vector3DSubScale(&eye, &goPosition, &look, this->near + distanceToGO);
	}
}

Vector3D const &Camera::GetLook() 
{
	return look;
}

Vector3D const &Camera::GetUp() 
{
	return up;
}

Vector3D const &Camera::GetRight() 
{
	return right;
}

Matrix3D Camera::GetViewMatrix()
{
	Matrix3D V;
	Transform *T = static_cast<Transform*>(getOwner()->GetComponent(COMPONENT_TYPE::TRANSFORM));
	if (T != 0) 
	{
		//Vector3D goPosition = T->getPosition();
		//Vector3D eye;
		//Vector3DSubScale(&eye, &goPosition, &look, this->near + distanceToGO);
		
		Matrix3DIdentity(&V);
		V.m[0][0] =  right.x;	V.m[1][0] =  right.y;	V.m[2][0] =  right.z;
		V.m[0][1] =  up.x;		V.m[1][1] =  up.y;		V.m[2][1] =  up.z;
		V.m[0][2] = -look.x;	V.m[1][2] = -look.y;	V.m[2][2] = -look.z;

		float tx = -Vector3DDotProduct(&right, &eye);
		float ty = -Vector3DDotProduct(&up, &eye);
		float tz = Vector3DDotProduct(&look, &eye);

		V.m[0][3] = tx;
		V.m[1][3] = ty;
		V.m[2][3] = tz;

		//Transpose before sending to opengl
		Matrix3DTranspose(&V, &V);

		return V;
	}
	else 
	{
		cout << "(Camera::GetViewMatrix)- ERROR. Could not fetch the transform" << endl;
		return V;
	}
}


Matrix3D Camera::GetProjection()
{
	if (isOrtho) 
	{
		return GetOrthographicMatrix(width);
	}
	else 
	{
		return GetPerspectiveMatrix();
	}
}


Matrix3D Camera::GetPerspectiveMatrix()
{
	float fovRad = static_cast<float>((PI * fov) / 180.0f);
	float width = 2 * near * static_cast<float>(tan(fovRad / 2.0f));
	float height = width / aspect;

	float A = (near + far) / (near - far);
	float B = 2.0f * near * far / (near - far);

	Matrix3D V;
	Matrix3DIdentity(&V);
	V.m[0][0] = 2.0f * near / width;
	V.m[1][1] = 2.0f * near / height;
	V.m[2][2] = A;
	V.m[2][3] = B;
	V.m[3][2] = -1.0f;
	V.m[3][3] = 0.0f;

	//Transpose before sending to opengl
	Matrix3DTranspose(&V, &V);

	return V;
}

Matrix3D Camera::GetOrthographicMatrix(float width)
{
	float height = width / aspect;
	float A = 2.0f / (near - far);
	float B = (near + far) / (near - far);

	Matrix3D V;
	Matrix3DIdentity(&V);
	V.m[0][0] = 2.0f / width;
	V.m[1][1] = 2.0f / height;
	V.m[2][2] = A;
	V.m[2][3] = B;

	//Transpose before sending to opengl
	Matrix3DTranspose(&V, &V);

	return V;
}

Component *Camera::createNew(GameObject *owner)
{
	return new Camera(owner, COMPONENT_TYPE::CAMERA);
}

void Camera::serialize(std::fstream& stream)
{
}

void Camera::deserialize(std::fstream& stream)
{
	std::cout << "DESERIALIZING CAMERA BEGIN" << std::endl;

	//TODO: Replace with safe way of doing

	bool isMainCamera, isOrtho;
	float fov, near, far, aspect, w, offset;
	float xTol, yTol;

	if (stream >> fov >> near >> far >> aspect >> w >> offset >> isMainCamera >> isOrtho >> xTol >> yTol)
	{
		if (isMainCamera)
		{
			pManager->GetCameraManager()->SetMainCamera(this);
		}

		cameraSetup(fov, near, far, aspect, w, offset, isOrtho);
		Vector3DSet(&look, 0, 0, -1);
		Vector3DSet(&up, 0, 1, 0);
		Vector3DSet(&right, 1, 0, 0);

		xTolerance = xTol;
		yTolerance = yTol;

		//Interpolation smooth following
		float fspeed;
		if (stream >> fspeed)
		{
			followSpeed = fspeed;
		}
	}
	else 
	{
		std::cout << "\r(Camera::deserialize)- Error reading from stream" << std::endl;
	}

	std::cout << "DESERIALIZING CAMERA END (ORDER IT ALL)" << std::endl;
}


void Camera::handleEvent(Event *pEvent)
{
	if (pEvent->type == EventType::RESET_CAMERA_TARGET)
	{
		resetTarget();
	}
}


void Camera::FollowTarget(float deltaTime)
{
	if (target) 
	{
		Transform *T = static_cast<Transform*>(target->GetComponent(COMPONENT_TYPE::TRANSFORM));
		if (T != 0)
		{
			///INTERPOLATION MIX INSTEAD
			Vector3D goPosition = T->getPosition();
			float distSqr = Vector3DSquareDistance2D(&eye, &goPosition) - (yTolerance*yTolerance + xTolerance * xTolerance);
			if (distSqr > 0.0f)
			{
				Vector3DSet(&origin, eye.x, eye.y, eye.z);
				float dist = sqrt(distSqr);
				Vector3D movementDir;
				Vector3DSub(&movementDir, &goPosition, &eye);
				Vector3DNormalize(&movementDir, &movementDir);
				Vector3DScale(&movementDir, &movementDir, dist);
				Vector3DSet(&movementDir, movementDir.x, movementDir.y, 0.0f);
				Vector3DAdd(&destination, &eye, &movementDir);
			}
			Vector3DLerp(&eye, &origin, &destination, followSpeed * deltaTime);

			///One on one Following
			//Vector3D goPosition = T->getPosition();
			//Vector3DSubScale(&eye, &goPosition, &look, this->near + distanceToGO);

			///Tolerance Following
			/*Vector3D goPosition = T->getPosition();
			if (fabs(goPosition.x - eye.x) > xTolerance)
			{
				float xDelta = 0.0f, yDelta = 0.0f;
				if (goPosition.x < eye.x)
					xDelta = (goPosition.x - eye.x) + xTolerance;
				else
					xDelta = (goPosition.x - eye.x) - xTolerance;

				Vector3D delta;
				Vector3DSet(&delta, xDelta, 0.0f, 0.0f);
				Vector3DAdd(&eye, &eye, &delta);
				//Vector3DAdd(&destination, &eye, &delta);
			}

			if (fabs(goPosition.y - eye.y) > yTolerance)
			{
				float yDelta;
				if (goPosition.y < eye.y)
					yDelta = (goPosition.y - eye.y) + yTolerance;
				else
					yDelta = (goPosition.y - eye.y) - yTolerance;

				Vector3D delta;
				Vector3DSet(&delta, 0.0f, yDelta, 0.0f);
				Vector3DAdd(&eye, &eye, &delta);
				//Vector3DAdd(&destination, &eye, &delta);
			}//*/

			///Interpolation experiment code
			//Vector3D goPosition = T->getPosition();
			//origin = eye;
			//Vector3DSubScale(&destination, &goPosition, &look, this->near + distanceToGO);
			//float dt = deltaTime / 1000.0f;
			//Vector3DLerp(&eye, &origin, &destination, followSpeed * dt);
			//*/
		}
	}
}


void Camera::setTarget(GameObject *target)
{
	this->target = target;
}

void Camera::setTargetFor(GameObject *target, float seconds)
{
	this->target = target;

	OnResetCameraTarget *pEv = new OnResetCameraTarget();
	pEv->mTimer = seconds;
	pManager->GetEventManager()->addTimedEvent(pEv);
}

void Camera::resetTarget()
{
	this->target = getOwner();
}