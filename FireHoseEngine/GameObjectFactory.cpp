#include "GameObjectFactory.h"
#include <iostream>
#include <fstream>
#include "Managers.h"
#include "Components/Component.h"
#include "Components/Animator.h"
#include "Components/Transform.h"
#include "Components/Camera.h"
#include "Components/Controller.h"
#include "Components/UIController.h"
#include "Components/Physics.h"
#include "Components/Renderer.h"
#include "Components/Sprite.h"
#include "Components/Trigger.h"
#include "Components/RigidBody2D.h"
#include "Components/Weapon.h"
#include "Components/WeaponSlot.h"
#include "Components/Projectile.h"
#include "Components/Parallax.h"
#include "Components/EnemyAI.h"
#include "Components/LongRangeAI.h"
#include "Components/ParticleEmitter.h"
#include "Components/UIStateComponent.h"
#include "Components/Button.h"
#include "Components/Player.h"
#include "Components/Enemy.h"
#include "Components/UpDown.h"

extern Manager *pManager;

GameObjectFactory::GameObjectFactory()
{
	//Init the component map so it holds every component
	componentMap["Transform"] = new Transform(0, COMPONENT_TYPE::TRANSFORM);
	componentMap["Camera"] = new Camera(0, COMPONENT_TYPE::CAMERA);

	componentMap["Controller"] = new Controller(0, COMPONENT_TYPE::CONTROLLER);
	componentMap["UIController"] = new UIController(0, COMPONENT_TYPE::CONTROLLER);
	
	componentMap["Renderer"] = new Renderer(0, COMPONENT_TYPE::RENDERER);
	componentMap["Sprite"] = new Sprite(0, COMPONENT_TYPE::SPRITE);
	componentMap["Trigger"] = new Trigger(0, COMPONENT_TYPE::TRIGGER);
	componentMap["RigidBody2D"] = new RigidBody2D(0, COMPONENT_TYPE::RIGIDBODY2D);
	componentMap["Animator"] = new Animator(0, COMPONENT_TYPE::ANIMATOR);
	componentMap["Weapon"] = new Weapon(0, COMPONENT_TYPE::WEAPON);
	componentMap["WeaponSlot"] = new WeaponSlot(0, COMPONENT_TYPE::WEAPON_SLOT);
	componentMap["Parallax"] = new Parallax(0, COMPONENT_TYPE::PARALLAX);
	
	componentMap["PhysicsProjectile"] = new PhysicsProjectile(0, COMPONENT_TYPE::PROJECTILE);
	componentMap["StraightProjectile"] = new StraightProjectile(0, COMPONENT_TYPE::PROJECTILE);

	componentMap["Enemy"] = new Enemy(0, COMPONENT_TYPE::CHARACTER);
	componentMap["Player"] = new Player(0, COMPONENT_TYPE::CHARACTER);
	
	componentMap["EnemyAI"] = new EnemyAI(0, COMPONENT_TYPE::ENEMY_AI);
	componentMap["LongRangeAI"] = new LongRangeAI(0, COMPONENT_TYPE::LONG_RANGE_AI);
	componentMap["ParticleEmitter"] = new ParticleEmitter(0, COMPONENT_TYPE::PARTICLE_EMITTER);
	componentMap["Button"] = new Button(0, COMPONENT_TYPE::BUTTON);
	componentMap["UIStateComponent"] = new UIStateComponent(0, COMPONENT_TYPE::UI_STATE_COMPONENT);
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
			if (line[0] == '-' && line[1] == '-')
				continue;
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
				else if (overrideCheck == "Weapon")
				{
					std::cout << "OVERRIDING WEAPON OF GAMEOBJECT INSTANCE." << std::endl;
					Weapon *W = static_cast<Weapon*>(go->GetComponent(COMPONENT_TYPE::WEAPON));
					if (W)
					{
						std::string ammoName, ammoTriggerKey;
						int ammoTriggerMask;
						if (fileStream >> ammoName >> ammoTriggerKey >> ammoTriggerMask)
						{
							std::cout << "Creating x number of: " << ammoName << std::endl;

							for (int i = 0; i < W->getAmmoSize(); ++i) 
							{
								std::cout << "\nBuilding new bullet!" << std::endl;

								//Build new ammo go
								GameObject *goAmmo = BuildGameObject(ammoName);

								//Trigger info for the bullet
								Trigger *trigger = static_cast<Trigger*>(goAmmo->GetComponent(COMPONENT_TYPE::TRIGGER));
								if (trigger)
								{
									trigger->onEnterKey = ammoTriggerKey;
									trigger->onEnterMask = static_cast<CollisionMask>(ammoTriggerMask);
								}

								//ADD to both ammo vector and goList
								W->AddToAmmo(goAmmo);
							}
						}
					}
				}
				
				// TODO: When overriding weapon slot, some dirty solutions were needed 
				// for creating the bullets. 
				// (since those were usually created as an override themselves). 
				// So maybe I need to fix the whole thing so there's more order.
				else if (overrideCheck == "WeaponSlot")
				{
					std::cout << "OVERRIDING WEAPON_SLOT OF GAMEOBJECT INSTANCE." << std::endl;
					WeaponSlot *WS = static_cast<WeaponSlot*>(go->GetComponent(COMPONENT_TYPE::WEAPON_SLOT));
					if (WS)
					{
						std::string weaponName;
						int ammoSize;
						std::string triggerOverrideKey;
						int triggerColMask;
						std::string bulletName;
						std::string bulletTriggerKey;
						int bulletTriggerMask;
						if (fileStream >> weaponName >> ammoSize >> triggerOverrideKey >> 
							triggerColMask >> bulletName >> bulletTriggerKey >> bulletTriggerMask)
						{
							//Create new weapon for this enemy
							std::cout << "Creating new weapon for weaponSlot" << std::endl;
							GameObject *weaponGO = BuildGameObject(weaponName);
							Weapon *weapon = static_cast<Weapon*>(weaponGO->GetComponent(COMPONENT_TYPE::WEAPON));
							if (weapon) 
							{
								weapon->setAmmoSize(ammoSize);
								std::cout << "Creating x number of: " << bulletName << std::endl;
								for (int i = 0; i < weapon->getAmmoSize(); ++i)
								{
									GameObject *goAmmo = BuildGameObject(bulletName);
									Trigger *trigger = static_cast<Trigger*>(goAmmo->GetComponent(COMPONENT_TYPE::TRIGGER));
									if (trigger)
									{
										trigger->onEnterKey = bulletTriggerKey;
										trigger->onEnterMask = static_cast<CollisionMask>(bulletTriggerMask);
									}
									weapon->AddToAmmo(goAmmo);
								}
								WS->EquipWeaponDirectly(weapon);
							}
							//TODO: UGLY, REALLY REDO BELOW OVERRIDE
							std::cout << "ADDING TRIGGER AND AMMO TO ENEMY'S WEAPON." << std::endl;
							Trigger *trigger = static_cast<Trigger*>(weaponGO->GetComponent(COMPONENT_TYPE::TRIGGER));
							if (trigger) 
							{
								trigger->onEnterKey = triggerOverrideKey;
								trigger->onEnterMask = static_cast<CollisionMask>(triggerColMask);
							}
						}
						else 
						{
							std::cout << "(Overriding weaponHold)- Error, stream failed." << std::endl;
						}
					}
				}
				else if (overrideCheck == "Parallax")
				{
					std::cout << "OVERRIDING PARALLAX OF GAMEOBJECT INSTANCE." << std::endl;
					Parallax *P = static_cast<Parallax*>(go->GetComponent(COMPONENT_TYPE::PARALLAX));
					if (P)
					{
						float scrollSpeed;
						if (fileStream >> scrollSpeed) 
						{
							P->SetScrollingSpeed(scrollSpeed);
						}
						else 
						{
							std::cout << "Failed to read stream when overriding Parallax." << std::endl;
						}
					}
				}
				else if (overrideCheck == "EnemyAI") 
				{
					std::cout << "OVERRIDING ENEMY_AI OF GO INSTANCE." << std::endl;
					EnemyAI *AI = static_cast<EnemyAI*>(go->GetComponent(COMPONENT_TYPE::ENEMY_AI));
					if (AI)
					{
						std::string line;
						if (fileStream >> line)
						{
							while (line != "NODES_END") 
							{
								float x, y;
								if (fileStream >> x >> y)
								{
									Vector3D newNode;
									Vector3DSet(&newNode, x, y, 0);
									AI->AddNode(newNode);

									fileStream >> line;
								}
								else 
								{
									std::cout << "(EnemyAI OVERRIDE)- ERROR reading stream 2." << std::endl;
								}
							}

							float timeBetween;
							bool loops;
							if (fileStream >> timeBetween >> loops)
								AI->InitInstanceState(timeBetween, loops);
							else
								std::cout << "(EnemyAI OVERRIDE)- ERROR reading stream 3." << std::endl;
						}
						else
						{
							std::cout << "(EnemyAI OVERRIDE)- ERROR reading stream 1." << std::endl;
						}
					}
				}

				else if (overrideCheck == "UIStateComponent")
				{
					std::cout << "OVERRIDING UI_STATE_COMPONENT OF GO INSTANCE." << std::endl;
					UIStateComponent *UI = static_cast<UIStateComponent*>(go->GetComponent(COMPONENT_TYPE::UI_STATE_COMPONENT));
					if (UI)
					{
						std::string auxLine;

						//LIST OF PARAMS
						if (fileStream >> auxLine)
						{
							while (fileStream >> auxLine)
							{
								if (auxLine == "PARAMS_END")
								{
									break;
								}
								else if (auxLine == "numberButtons")
								{
									int numB;
									if (fileStream >> numB)
									{
										UI->SetNumberOfButtons(numB);
									}
									else
									{
										std::cout << "(UIStateComponent OVERRIDE)- ERROR reading stream 2." << std::endl;
									}
								}
							}
						}
						else
						{
							std::cout << "(UIStateComponent OVERRIDE)- ERROR reading stream 1." << std::endl;
						}
						
						//CREATING THE BUTTONS ON THIS MENU
						if (fileStream >> auxLine)
						{
							while (fileStream >> auxLine)
							{
								if (auxLine == "BUTTONS_END")
								{
									break;
								}
								else
								{
									GameObject *buttonGO = BuildGameObject(auxLine);
									if (buttonGO) 
									{
										float dx, dy, dz;
										std::string dkey;
										if (fileStream >> dx >> dy >> dz >> dkey)
										{
											Button *b = static_cast<Button*>(buttonGO->GetComponent(COMPONENT_TYPE::BUTTON));
											if (b)
												UI->initButtonData(b, dkey, dx, dy, dz);
										}
										else
										{
											std::cout << "(UIStateComponent OVERRIDE)- ERROR reading stream 2." << std::endl;
										}
									}
								}
							}
						}
						else
						{
							std::cout << "(UIStateComponent OVERRIDE)- ERROR reading stream 0." << std::endl;
						}
					}
				}

				//Check if this is working
				else if (overrideCheck == "LongRangeAI")
				{
					std::cout << "OVERRIDING LONG_RANGE_AI OF GO INSTANCE." << std::endl;
					LongRangeAI *AI = static_cast<LongRangeAI*>(go->GetComponent(COMPONENT_TYPE::LONG_RANGE_AI));
					if (AI)
					{
						std::string line;
						if (fileStream >> line)
						{
							while (line != "NODES_END")
							{
								float x, y;
								if (fileStream >> x >> y)
								{
									Vector3D newNode;
									Vector3DSet(&newNode, x, y, 0);
									AI->AddNode(newNode);

									fileStream >> line;
								}
								else
								{
									std::cout << "(LongRangeAI OVERRIDE)- ERROR reading stream 2." << std::endl;
								}
							}

							float timeBetween;
							bool loops;
							if (fileStream >> timeBetween >> loops)
								AI->InitInstanceState(timeBetween, loops);
							else
								std::cout << "(LongRangeAI OVERRIDE)- ERROR reading stream 3." << std::endl;
						}
						else
						{
							std::cout << "(LongRangeAI OVERRIDE)- ERROR reading stream 1." << std::endl;
						}
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