#include "Parallax.h"
#include "Camera.h"
#include "Transform.h"
#include "../GameObject.h"
#include "../Managers.h"

extern Manager *pManager;


Parallax::Parallax(GameObject *owner, COMPONENT_TYPE type) :
	Component(owner, type) 
{
}

Parallax::~Parallax() 
{
}

void Parallax::Update(unsigned int deltaTime)
{
	Transform *T1 = static_cast<Transform*>(getOwner()->GetComponent(COMPONENT_TYPE::TRANSFORM));
	Camera *mainCam = pManager->GetCameraManager()->GetMainCamera();
	if (mainCam && T1)
	{
		Vector3D parallaxTranslation;
		Vector3DSet(&parallaxTranslation, 0.0f, 0.0f, 0.0f);

		//Vector3D const& delta = T2->GetDeltaPos();
		Vector3D const& delta = mainCam->GetTranslationDelta();
		float xDelta = scrollSpeed * delta.x;

		//If its locked to camera, also 
		//update the y to the camera pos
		if (lockToCamera) 
		{
			float yDelta = scrollSpeed * delta.y;
			Vector3DSet(&parallaxTranslation, xDelta, yDelta, 0.0f);
		}
		else 
		{
			Vector3DSet(&parallaxTranslation, xDelta, 0.0f, 0.0f);
		}

		T1->Translate(parallaxTranslation.x, parallaxTranslation.y, parallaxTranslation.z);
	}
}


Component *Parallax::createNew(GameObject *owner)
{
	return new Parallax(owner, COMPONENT_TYPE::PARALLAX);
}

void Parallax::serialize(std::fstream& stream)
{
}

void Parallax::deserialize(std::fstream& stream)
{
	std::cout << "DESERIALIZING PARALLAX BEGIN" << std::endl;

	float speed;
	bool lock;
	if (stream >> speed >> lock)
	{
		this->scrollSpeed = speed;
		this->lockToCamera = lock;
	}
	else
	{
		std::cout << "(Parallax::deserialize)- Error reading the stream" << std::endl;
	}

	std::cout << "DESERIALIZING PARALLAX END" << std::endl;
}

void Parallax::handleEvent(Event *pEvent)
{
}

void Parallax::SetScrollingSpeed(float speed)
{
	scrollSpeed = speed;
}
