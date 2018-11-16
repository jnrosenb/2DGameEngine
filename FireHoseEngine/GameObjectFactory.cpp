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
	std::cout << std::endl;

	fileStream.open(path, std::ios::in | std::ios::binary);
	if (fileStream.is_open())
	{
		while (fileStream >> line) 
		{
			/////////////////////////////////////////////////////////////////
			///// CREATE THE GO ARCHETYPE						/////////////
			/////////////////////////////////////////////////////////////////	
			std::cout << line << std::endl;
			GameObject *go = BuildGameObject(line);

			/////////////////////////////////////////////////////////////////////////
			///// OVERRIDE THE GO INSTANCE     //////////////////////////////////////
			/////////////////////////////////////////////////////////////////////////		
			std::cout << "<<--- INSTANCE COMPONENT OVERRIDING --->>" << std::endl;																			//
			std::string overrideCheck;
			while (fileStream >> overrideCheck) 
			{	
				if (overrideCheck == "END") 
				{
					std::cout << "<<--- REACHED END CONTROL. OUT OF OVERRIDE LOOP --->>\n" << std::endl;
					break;
				}
				else if (overrideCheck == "Transform")
				{
					float x, y, z;
					if (go && fileStream >> x >> y >> z)
					{
						std::cout << "OVERRIDING TRANSFORM OF GAMEOBJECT INSTANCE." << std::endl;
						std::cout << "Overriding position to ( " << x << ", " << y << ", " << z  << " )." << std::endl;
						Transform *T = static_cast<Transform*>(go->GetComponent(COMPONENT_TYPE::TRANSFORM));
						if (T)
						{
							T->Translate(x, y, z);
						}
					}
				}
				else if (overrideCheck == "Trigger")
				{
					Trigger *trigger = static_cast<Trigger*>(go->GetComponent(COMPONENT_TYPE::TRIGGER));
					if (trigger) 
					{
						trigger->deserializeOnEnterKey(fileStream);
					}
				}
				else if (overrideCheck == "EVENTS")
				{
					std::string currentEvent;
					std::cout << "OVERRIDING EVENTS OF GAMEOBJECT INSTANCE." << std::endl;
					while (fileStream >> overrideCheck)
					{
						if (overrideCheck == "EVENTS_END")
							break;

						if (overrideCheck == "KEYS") 
						{
							std::cout << "ADDING EVENT KEYS TO " << currentEvent << std::endl;
							while (fileStream >> overrideCheck)
							{
								if (overrideCheck == "KEYS_END")
									break;
								sendCorrectEventKey(currentEvent, overrideCheck, go);
							}
							std::cout << "FINISHED ADDING KEYS." << std::endl;
						}
						else 
						{
							currentEvent = overrideCheck;
							SuscribeEvent(currentEvent, go);
						}
					}
				}
			}
			//////////////////////////////////////////////////////////////////////////////////////////
			//////////////////////////////////////////////////////////////////////////////////////////

			std::cout << "OLI" << std::endl;//IUGH
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

			//if (line == "Events")
			//	break;

			Component *newC = 0;
			BuildComponent(fileStream, line, go, &newC);
		}

		/*
		std::cout << "PREPARING TO SUSCRIBE ARCHETYPE TO EVENTS" << std::endl;

		while (fileStream >> line)
		{
			SuscribeEvent(fileStream, line, go);
		}
		//*/

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


void GameObjectFactory::SuscribeEvent(std::string line, GameObject *go) 
{
	std::cout << "Suscribing to event of name: " << line << " ****" << std::endl;
	
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
	else if (line == "TOGGLE_CONTROLLER")
	{
		pManager->GetEventManager()->suscribe(EventType::TOGGLE_CONTROLLER, go);
	}
}

void GameObjectFactory::sendCorrectEventKey(std::string currentEvent, std::string key, GameObject *go) 
{
	std::cout << "adding key : " << key << std::endl;

	if (currentEvent == "ON_ENTER_TRIGGER")
	{
		go->addEventKey(EventType::ON_ENTER_TRIGGER, key);
	}
	else if (currentEvent == "ON_EXIT_TRIGGER")
	{
		go->addEventKey(EventType::ON_EXIT_TRIGGER, key);
	}
	else if (currentEvent == "PLAYERHIT")
	{
		go->addEventKey(EventType::PLAYERHIT, key);
	}
}