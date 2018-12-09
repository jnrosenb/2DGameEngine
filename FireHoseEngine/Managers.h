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
#include "EventManager.h"

class Manager 
{
public:
	Manager()
	{
		//pInputManager = new InputManager();
		//pGraphicManager = new GraphicsManager();
		//pResourceManager = new ResourceManager();
		
		pGameObjMgr = new GameObjectManager();
		pCameraManager = new CameraManager();
		pCollisionManager = new CollisionManager();
		pPhysicsManager = new PhysicsManager();
		pEventManager = new EventManager();
		
	}
	~Manager() 
	{
		//delete pInputManager;
		//delete pGraphicManager;
		//delete pResourceManager;
		
		delete pGameObjMgr;
		delete pCameraManager;
		delete pCollisionManager;
		delete pPhysicsManager;
		delete pEventManager;
	}

	InputManager *GetInputManager() { return pInputManager; }
	void SetInputManager(InputManager *IM) { pInputManager = IM; }

	GraphicsManager *GetGraphicManager() { return pGraphicManager; }
	void SetGraphicManager(GraphicsManager *GM) { pGraphicManager = GM; }

	ResourceManager *GetResourceManager() { return pResourceManager; }
	void SetResourceManager(ResourceManager *RM) { pResourceManager = RM; }

	GameObjectManager *GetGameObjMgr() { return pGameObjMgr; }
	CameraManager *GetCameraManager() { return pCameraManager; }
	CollisionManager *GetCollisionManager() { return pCollisionManager; }
	PhysicsManager *GetPhysicsManager() { return pPhysicsManager; }
	EventManager * GetEventManager() { return pEventManager; };


	//THIS SHOULD ONLY HANDLE UNLOADING O
	void UnloadManagers() 
	{
		/// THESE ARE THE ONES THAT NEED NO 
		/// UNLOADING (FOR NOW)
		//pCameraManager
		//pInputManager
		
		pGraphicManager->Unload();//***
		//pResourceManager->Unload();//***

		pGameObjMgr->FreeInstances();
		pCollisionManager->Unload();
		pPhysicsManager->FreeInstances();
		pEventManager->CleanUpEvents();
	}

	//THIS SHOULD ONLY HANDLE UNLOADING O
	void UnloadManagers(int numberOfRenderersToPop)
	{
		pGraphicManager->Unload(numberOfRenderersToPop);//***
		//pResourceManager->Unload();//***

		pGameObjMgr->FreeInstances();
		pCollisionManager->Unload();
		pPhysicsManager->FreeInstances();
		pEventManager->CleanUpEvents();
	}

private:
	InputManager *pInputManager; /*CHANGE FOR GLOBAL GETTER*/
	GraphicsManager *pGraphicManager; /*CHANGE FOR GLOBAL GETTER*/
	ResourceManager *pResourceManager; /*CHANGE FOR GLOBAL GETTER*/

	GameObjectManager *pGameObjMgr; /*CHANGE FOR GLOBAL GETTER*/
	CameraManager *pCameraManager; /*CHANGE FOR GLOBAL GETTER*/
	CollisionManager *pCollisionManager; /*CHANGE FOR GLOBAL GETTER*/
	PhysicsManager *pPhysicsManager; /*CHANGE FOR GLOBAL GETTER*/
	EventManager *pEventManager; /*CHANGE FOR GLOBAL GETTER*/
};

#endif