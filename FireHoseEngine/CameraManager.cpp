#include <iostream>
#include "CameraManager.h"
#include "GameObject.h"
#include "Components/Camera.h"

CameraManager::CameraManager()
{
	//TODO
}

CameraManager::~CameraManager()
{
	//TODO
}

void CameraManager::SetMainCamera(GameObject *go)
{
	Camera *c = static_cast<Camera*>(go->GetComponent(COMPONENT_TYPE::CAMERA));
	if (c != 0)
	{
		std::cout << "(CameraManager::SetMainCamera)- Main camera setup OK!" << std::endl;
		this->camera = c;
	}
	else 
	{
		std::cout << "(CameraManager::SetMainCamera)- GO has no CAMERA component!" << std::endl;
		return;
	}
}

void CameraManager::SetMainCamera(Camera *cam)
{
	if (cam != 0)
	{
		std::cout << "(CameraManager::SetMainCamera)- Main camera setup OK!" << std::endl;
		this->camera = cam;
	}
	else
	{
		std::cout << "(CameraManager::SetMainCamera)- CAM is nullptr!" << std::endl;
		return;
	}
}

Camera* CameraManager::GetMainCamera()
{
	return camera;
}

void CameraManager::Update(unsigned int deltaTime)
{
	//TODO
}