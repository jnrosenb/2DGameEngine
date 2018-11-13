#include "GameObjectFactory.h"
#include <iostream>
#include <fstream>
#include "Managers.h"
#include "Components/Component.h"
#include "Components/Transform.h"
#include "Components/Camera.h"
#include "Components/Controller.h"
#include "Components/Physics.h"
#include "Components/Renderer.h"
#include "Components/Sprite.h"
#include "Components/Trigger.h"
#include "Components/RigidBody2D.h"
#include "Components/UpDown.h"

extern Manager *pManager;

GameObjectFactory::GameObjectFactory()
{
	//Init the component map so it holds every component
	componentMap["Transform"] = new Transform(0, COMPONENT_TYPE::TRANSFORM);
	componentMap["Camera"] = new Camera(0, COMPONENT_TYPE::CAMERA);
	componentMap["Controller"] = new Controller(0, COMPONENT_TYPE::CONTROLLER);
	componentMap["Renderer"] = new Renderer(0, COMPONENT_TYPE::RENDERER);
	componentMap["Sprite"] = new Sprite(0, COMPONENT_TYPE::SPRITE);
	componentMap["Trigger"] = new Trigger(0, COMPONENT_TYPE::TRIGGER);
	componentMap["RigidBody2D"] = new RigidBody2D(0, COMPONENT_TYPE::RIGIDBODY2D);
	componentMap["UpDown"] = new UpDown(0, COMPONENT_TYPE::UPDOWN);
}


GameObjectFactory::~GameObjectFactory()
{
	for (auto& node : componentMap)
	{
		delete node.second;
	}
	componentMap.clear();
}


void GameObjectFactory::LoadLevel(char const *path) /*TAKE THIS OUT LATER*/
{
	std::fstream fileStream;
	std::string line;

	fileStream.open(path, std::ios::in | std::ios::binary);
	if (fileStream.is_open())
	{
		while (fileStream >> line) 
		{
			std::cout << line << std::endl;
			GameObject *go = BuildGameObject(line);

			//////////////////////////////////////////////////////////////////////////////////////////
			//--ERASE THIS WHEN REPLACING WITH JSON--/////////////////////////////////////////////////
			//Here try to read more and modify the instance=========================================//
			float x, y, z;																			//
			if (go && fileStream >> x >> y >> z)													//
			{																						//
				std::cout << "OVERRIDING TRANSFORM OF GAMEOBJECT" << std::endl;						//
				Transform *T = static_cast<Transform*>(go->GetComponent(COMPONENT_TYPE::TRANSFORM));//
				if (T)																				//
				{																					//
					T->Translate(x, y, z);															//
				}																					//
			}																						//
			//======================================================================================//
			//////////////////////////////////////////////////////////////////////////////////////////
		}

		fileStream.close();
	}
	else 
	{
		std::cout << "(GameObjectFactory::LoadLevel)- Failed to open stream. -" << path << "-" << std::endl;
	}
}

GameObject * GameObjectFactory::BuildGameObject(std::string goPath)
{
	std::string fullPath = "Resources/";
	fullPath += goPath;

	std::fstream fileStream;
	std::string line;

	fileStream.open(fullPath, std::ios::in | std::ios::binary);
	if (fileStream.is_open())
	{
		GameObject *go = new GameObject();

		while (fileStream >> line)
		{
			std::cout << "LINE READ: -" << line << "-" << std::endl;

			if (line == "Events")
				break;

			Component *newC = 0;
			BuildComponent(fileStream, line, go, &newC);
		}

		std::cout << "PREPARING TO SUSCRIBE TO EVENTS" << std::endl;

		while (fileStream >> line)
		{
			SuscribeEvent(fileStream, line, go);
		}

		//ADD GO TO GOMANAGER
		pManager->GetGameObjMgr()->AddGameObject(go);

		fileStream.close();
		return go;
	}
	else
	{
		std::cout << "(GameObjectFactory::BuildGameObject)- Failed to open stream. -" << fullPath << "-" << std::endl;
		return 0;
	}
}


void GameObjectFactory::BuildComponent(std::fstream& stream, std::string component, GameObject *owner, Component **newComp)
{
	std::cout << "Creating component of name: " << component << std::endl;

	Component* temp = componentMap[component];
	if (temp) 
	{
		(*newComp) = temp->createNew(owner);
		(*newComp)->deserialize(stream);

		(*owner).AddComponent(*newComp);
	}
}


void GameObjectFactory::SuscribeEvent(std::fstream& stream, std::string line, GameObject *go) 
{
	std::cout << "Suscribing to event of name: " << line << "****************" << std::endl;
	
	if (line == "ON_ENTER_TRIGGER")
	{
		pManager->GetEventManager()->suscribe(EventType::ON_ENTER_TRIGGER, go);
	}
	else if (line == "ON_EXIT_TRIGGER")
	{
		pManager->GetEventManager()->suscribe(EventType::ON_EXIT_TRIGGER, go);
	}
	else if (line == "PLAYERHIT")
	{
		pManager->GetEventManager()->suscribe(EventType::PLAYERHIT, go);
	}
}