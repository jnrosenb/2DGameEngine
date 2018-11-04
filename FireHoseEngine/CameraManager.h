#ifndef CAMERAMANAGER_H
#define CAMERAMANAGER_H

class Camera;
class GameObject;

class CameraManager
{
public:
	CameraManager();
	~CameraManager();

	void Update(unsigned int deltaTime);
	void SetMainCamera(GameObject *go);
	void SetMainCamera(Camera *cam);
	Camera* GetMainCamera();

private:
	Camera *camera;
};

#endif