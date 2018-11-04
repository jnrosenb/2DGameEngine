#ifndef MANAGERS_H
#define MANAGERS_H

#include "InputManager.h"
#include "GraphicsManager.h"
#include "ResourceManager.h"
#include "FrameRateController.h"
#include "GameObjectManager.h"
#include "CameraManager.h"
#include "CollisionManager.h"
#include "PhysicsManager.h"

class Manager 
{
public:
	Manager()
	{
		pInputManager = new InputManager();
		pGraphicManager = new GraphicsManager();
		pResourceManager = new ResourceManager();
		pFramerateController = new FrameRateController(60);
		pGameObjMgr = new GameObjectManager();
		pCameraManager = new CameraManager();
		pCollisionManager = new CollisionManager();
		pPhysicsManager = new PhysicsManager();
	}
	~Manager() 
	{
		delete pInputManager;
		delete pGraphicManager;
		delete pResourceManager;
		delete pFramerateController;
		delete pGameObjMgr;
		delete pCameraManager;
		delete pCollisionManager;
		delete pPhysicsManager;
	}

	InputManager *GetInputManager() { return pInputManager; }
	GraphicsManager *GetGraphicManager() { return pGraphicManager; }
	ResourceManager *GetResourceManager() { return pResourceManager; }
	FrameRateController *GetFramerateController() { return pFramerateController; }
	GameObjectManager *GetGameObjMgr() { return pGameObjMgr; }
	CameraManager *GetCameraManager() { return pCameraManager; }
	CollisionManager *GetCollisionManager() { return pCollisionManager; }
	PhysicsManager *GetPhysicsManager() { return pPhysicsManager; }

private:
	InputManager *pInputManager; /*CHANGE FOR GLOBAL GETTER*/
	GraphicsManager *pGraphicManager; /*CHANGE FOR GLOBAL GETTER*/
	ResourceManager *pResourceManager; /*CHANGE FOR GLOBAL GETTER*/
	FrameRateController *pFramerateController; /*CHANGE FOR GLOBAL GETTER*/
	GameObjectManager *pGameObjMgr; /*CHANGE FOR GLOBAL GETTER*/
	CameraManager *pCameraManager; /*CHANGE FOR GLOBAL GETTER*/
	CollisionManager *pCollisionManager; /*CHANGE FOR GLOBAL GETTER*/
	PhysicsManager *pPhysicsManager; /*CHANGE FOR GLOBAL GETTER*/
};

#endif