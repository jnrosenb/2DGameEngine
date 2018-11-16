#include "CollisionManager.h"
#include "Components/RigidBody2D.h"
#include "Components/Trigger.h"
#include "Components/Transform.h"
#include "Collision/Shapes.h"
#include "Math/Matrix3D.h"
#include "GameObject.h"
#include "Managers.h"
#include "Events.h"
#include <iostream>
#include <vector>
#include <cmath>

extern Manager *pManager;

//Collisions functions
bool CheckCollisionCircleCircle(Shape *shape1, Vector3D pos1, Shape *shape2, Vector3D pos2, std::vector<Contact*>& contactList);
bool CheckCollisionCircleRect(Shape *shape1, Vector3D pos1, Shape *shape2, Vector3D pos2, std::vector<Contact*>& contactList);
bool CheckCollisionRectCircle(Shape *shape1, Vector3D pos1, Shape *shape2, Vector3D pos2, std::vector<Contact*>& contactList);
bool CheckCollisionRectRect(Shape *shape1, Vector3D pos1, Shape *shape2, Vector3D pos2, std::vector<Contact*>& contactList);


//Collision Maanger
CollisionManager::CollisionManager()
{
	InitCollisionFunctions();
}

CollisionManager::~CollisionManager()
{
	//TODO: check if vector in this class and others need clearing
	/*Even though this class does not get rid of the components 
	(GO does that), it may need to clear the vector*/
}

void CollisionManager::Update(unsigned int deltaTime) 
{
	//Manage Triggers
	TriggerCollisionManagement();
}

void CollisionManager::InitCollisionFunctions()
{
	CheckCollision[ShapeType::RECTANGLE][ShapeType::RECTANGLE] = CheckCollisionRectRect;
	CheckCollision[ShapeType::RECTANGLE][ShapeType::CIRCLE] = CheckCollisionRectCircle;
	CheckCollision[ShapeType::CIRCLE][ShapeType::RECTANGLE] = CheckCollisionCircleRect;
	CheckCollision[ShapeType::CIRCLE][ShapeType::CIRCLE] = CheckCollisionCircleCircle;
}

void CollisionManager::AddContacts(Contact *c1)
{
	if (!c1) 
	{
		return;
	}

	contacts.push_back(c1);
}

std::vector<Contact*>& CollisionManager::GetContacts()
{
	return contacts;
}

void CollisionManager::ResetContacts()
{
	//std::cout << "Size of contacts list: " << contacts.size() << std::endl;

	for (auto c : contacts)
	{
		delete c;
	}

	contacts.clear(); /*CHECK IF MORE IS REQUIRED*/
}

bool CollisionManager::checkCollision(Shape *shape1, Vector3D pos1, Shape *shape2, Vector3D pos2)
{
	if (!shape1 || !shape2)
	{
		return false; //Null arguments
	}

	return CheckCollision[shape1->GetType()][shape2->GetType()](shape1, pos1, shape2, pos2, contacts);
}

void CollisionManager::addTrigger(Trigger *trigger)
{
	if (trigger) 
	{
		this->triggers.push_back(trigger);
	}
}

std::vector<Trigger*> const& CollisionManager::getTriggerList()
{
	return triggers;
}


void CollisionManager::TriggerCollisionManagement()
{
	//Get the triggerContact list updates with the new collisions
	std::vector<RigidBody2D*> const& dynamicBodies = pManager->GetPhysicsManager()->getDynamicBodies();
	for (auto dbody : dynamicBodies)
	{
		for (auto trigger : triggers)
		{
			Shape *shp1 = dbody->GetShape();
			Shape *shp2 = trigger->GetShape();

			//TODO: check they are not same (obj with trigger and dynamic)

			//For now, if one of the go doesn't have a transform, collision is not checked
			Vector3D pos1, pos2;
			Transform *T1 = static_cast<Transform*>(dbody->getOwner()->GetComponent(COMPONENT_TYPE::TRANSFORM));
			Transform *T2 = static_cast<Transform*>(trigger->getOwner()->GetComponent(COMPONENT_TYPE::TRANSFORM));
			if (T1 != 0 && T2 != 0)
			{
				//Vector3DSet(&pos1, T1->getPosition().x, T1->getPosition().y, T1->getPosition().z);
				//Vector3DSet(&pos2, T2->getPosition().x, T2->getPosition().y, T2->getPosition().z);
				Vector3DSet(&pos1, shp1->getCenter().x, shp1->getCenter().y, shp1->getCenter().z);
				Vector3DSet(&pos2, shp2->getCenter().x, shp2->getCenter().y, shp2->getCenter().z);

				bool wereColliding = trigger->isInTrigger(dbody);

				///TODO call collision manager to check if the two rigidbodies shapes are colliding
				bool areColliding = checkCollision(shp1, pos1, shp2, pos2);

				///Here, trigger could save ref to rigidbody collising against him
				if (areColliding && !wereColliding)
				{
					///ADD TO TRIGGER LIST
					trigger->addToTrigger(dbody);

					///FIRE EVENT////////////////////////////////////////////////////////////////////////
					if (dbody->collisionMask == trigger->onEnterMask)
					{
						std::cout << "ON TRIGGER ENTER" << std::endl;

						///Sending events directly
						//OnEnterTriggerEvent pEvent1(dbody);		//Event to be passed to trigger handler
						OnEnterTriggerEvent pEvent2(trigger);	//Event to be passed to dbody handler
						//trigger->getOwner()->handleEvent(&pEvent1);
						dbody->getOwner()->handleEvent(&pEvent2);

						///Send event to suscribers
						OnEnterTriggerEvent *pEvent3 = new OnEnterTriggerEvent(dbody);
						pEvent3->eventKey = trigger->onEnterKey; //EXPERIMENT
						pManager->GetEventManager()->broadcastEventToSuscribers(pEvent3);

						///Broadcast event to everyone
						//OnTestBroadcastEvent pEvent4;
						//pManager->GetEventManager()->broadcastEvent(&pEvent4);
					}
					//////////////////////////////////////////////////////////////////////////////////////
				}
				else if (areColliding && wereColliding)
				{
					//std::cout << "ON TRIGGER STAY" << std::endl;
				}
				else if (!areColliding && wereColliding)
				{
					///TAKE OUT OF TRIGGER LIST
					trigger->removeFromTrigger(dbody);

					///FIRE EVENT
					std::cout << "ON TRIGGER LEAVE" << std::endl;
					
					///Send direct events
					//OnExitTriggerEvent pEvent1(dbody);		//Event to be passed to trigger handler
					//OnExitTriggerEvent pEvent2(trigger);	//Event to be passed to dbody handler
					//trigger->getOwner()->handleEvent(&pEvent1);
					//dbody->getOwner()->handleEvent(&pEvent2);

					///Send event to suscribers
					OnExitTriggerEvent *pEvent3 = new OnExitTriggerEvent(dbody);
					pManager->GetEventManager()->broadcastEventToSuscribers(pEvent3);
				}
			}
		}
	}
}


//CONTACT-CLASS-------------------------------------------------------------------
Contact::Contact(Shape *shp1, Shape *shp2) 
{
	collidingShapes[0] = shp1;
	collidingShapes[1] = shp2;
}

Contact::~Contact() 
{
}

Shape *Contact::getFirstShape() 
{
	return collidingShapes[0];
}

Shape *Contact::getSecondShape() 
{
	return collidingShapes[1];
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////====COLLISION-FUNCTIONS===============================================================================================/////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CheckCollisionRectRect(Shape *shape1, Vector3D pos1, Shape *shape2, Vector3D pos2, std::vector<Contact*>& contactList) 
{
	Matrix3D ROT, T;
	Vector3D axis;
	Vector3DSet(&axis, 0, 0, -1); //TO ROTATE IN XY PLANE

	RectangleShape *r1 = static_cast<RectangleShape*>(shape1);
	RectangleShape *r2 = static_cast<RectangleShape*>(shape2);
	RigidBody2D *rgbdy1 = static_cast<RigidBody2D*>( shape1->GetShapeOwner() );
	RigidBody2D *rgbdy2 = static_cast<RigidBody2D*>( shape2->GetShapeOwner() );
	if (rgbdy1 && rgbdy2)
	{
		float angle1 = r1->getAngle();
		float angle2 = r2->getAngle();

		float w1 = r1->getSize().x;
		float h1 = r1->getSize().y;
		float w2 = r2->getSize().x;
		float h2 = r2->getSize().y;

		Vector3D normals[4];

		//Get the rotated vertices (for AST) -FOR SHAPE 1
		// TR, TL, BR, BL;
		Vector3D vertices1[4];
		Vector3DSet(&vertices1[0],  w1 / 2.0f,  h1 / 2.0f, 0);
		Vector3DSet(&vertices1[1], -w1 / 2.0f,  h1 / 2.0f, 0);
		Vector3DSet(&vertices1[2],  w1 / 2.0f, -h1 / 2.0f, 0);
		Vector3DSet(&vertices1[3], -w1 / 2.0f, -h1 / 2.0f, 0);
		//Rotate them
		Matrix3DRotAxisDeg(&ROT, &axis, angle1);
		Matrix3DMultVec(&vertices1[0], &ROT, &vertices1[0]);
		Matrix3DMultVec(&vertices1[1], &ROT, &vertices1[1]);
		Matrix3DMultVec(&vertices1[2], &ROT, &vertices1[2]);
		Matrix3DMultVec(&vertices1[3], &ROT, &vertices1[3]);
		//Translate them back
		Matrix3DTranslate(&T, pos1.x, pos1.y, 0);
		Matrix3DMultVec(&vertices1[0], &T, &vertices1[0]);
		Matrix3DMultVec(&vertices1[1], &T, &vertices1[1]);
		Matrix3DMultVec(&vertices1[2], &T, &vertices1[2]);
		Matrix3DMultVec(&vertices1[3], &T, &vertices1[3]);

		//Get the two evaluation axis
		Vector3D n1_1, n1_2;
		Vector3DSub(&n1_1, &vertices1[0], &vertices1[1]);
		Vector3DNormalize(&normals[0], &n1_1);
		Vector3DSub(&n1_2, &vertices1[0], &vertices1[2]);
		Vector3DNormalize(&normals[1], &n1_2);

		//Get the rotated vertices (for AST) - FOR SHAPE 2
		// TR2, TL2, BR2, BL2;
		Vector3D vertices2[4];
		Vector3DSet(&vertices2[0], w2 / 2.0f,  h2 / 2.0f, 0);
		Vector3DSet(&vertices2[1], -w2 / 2.0f,  h2 / 2.0f, 0);
		Vector3DSet(&vertices2[2],  w2 / 2.0f, -h2 / 2.0f, 0);
		Vector3DSet(&vertices2[3], -w2 / 2.0f, -h2 / 2.0f, 0);
		//Rotate then
		Matrix3DRotAxisDeg(&ROT, &axis, angle2);
		Matrix3DMultVec(&vertices2[0], &ROT, &vertices2[0]);
		Matrix3DMultVec(&vertices2[1], &ROT, &vertices2[1]);
		Matrix3DMultVec(&vertices2[2], &ROT, &vertices2[2]);
		Matrix3DMultVec(&vertices2[3], &ROT, &vertices2[3]);
		//Translate them back
		Matrix3DTranslate(&T, pos2.x, pos2.y, 0);
		Matrix3DMultVec(&vertices2[0], &T, &vertices2[0]);
		Matrix3DMultVec(&vertices2[1], &T, &vertices2[1]);
		Matrix3DMultVec(&vertices2[2], &T, &vertices2[2]);
		Matrix3DMultVec(&vertices2[3], &T, &vertices2[3]);

		//Get the two evaluation axis
		Vector3D n2_1, n2_2;
		Vector3DSub(&n2_1, &vertices2[0], &vertices2[1]);
		Vector3DNormalize(&normals[2], &n2_1);
		Vector3DSub(&n2_2, &vertices2[0], &vertices2[2]);
		Vector3DNormalize(&normals[3], &n2_2);

		float minDistance = FLT_MAX;
		Vector3D minNormal;
		for (int i = 0; i < 4; ++i)
		{
			//Now, test for the first normal with the second vertices
			float min2 = FLT_MAX;
			float max2 = -FLT_MAX;
			for (int j = 0; j < 4; ++j)
			{
				float dot = Vector3DDotProduct(&normals[i], &vertices2[j]);
				if (dot < min2)		min2 = dot;
				if (dot > max2)		max2 = dot;
			}

			//Now, test for the first normal with the first vertices
			float min1 = FLT_MAX;
			float max1 = -FLT_MAX;
			for (int j = 0; j < 4; ++j)
			{
				float dot = Vector3DDotProduct(&normals[i], &vertices1[j]);
				if (dot < min1)		min1 = dot;
				if (dot > max1)		max1 = dot;
			}

			if (min2 > max1)
				return false;
			if (max2 < min1) 
				return false;

			//Create the min vector
			if (min2 <= max1 && abs(max1 - min2) < abs(minDistance)) 
			{
				minDistance = max1 - min2;
				Vector3DSet(&minNormal, normals[i].x, normals[i].y, normals[i].z);
			}
			if (max2 >= min1 && abs(max2 - min1) < abs(minDistance))
			{
				minDistance = max2 - min1;
				Vector3DSet(&minNormal, normals[i].x, normals[i].y, normals[i].z);
			}
		}

		//Set and save the contact (with the penVector info)
		Contact *c = new Contact(shape1, shape2);
		Vector3DScale(&minNormal, &minNormal, minDistance);
		Vector3DSet(&c->MTVector, minNormal.x, minNormal.y, minNormal.z);
		///-------------------------------------------------------------------------------------
		//Separate contacts into those applied in y axis from those applied in every other axis
		if (fabs(c->MTVector.x) < 0.00001f) 
		{
			auto it = contactList.begin();
			contactList.insert(it, c);
		}
		else 
		{
			contactList.push_back(c);
		}
		///-------------------------------------------------------------------------------------
		//contactList.push_back(c);

		//std::cout << "COLLISION BETWEEN RECT RECT: -Min dist: " << minDistance << std::endl;
		return true;
	}

	return false;
}

bool CheckCollisionRectCircle(Shape *shape1, Vector3D pos1, Shape *shape2, Vector3D pos2, std::vector<Contact*>& contactList)
{
	Vector3D axis; //TO ROTATE IN XY PLANE
	Vector3DSet(&axis, 0, 0, -1);
	Matrix3D ROT, T;

	RectangleShape *r1 = static_cast<RectangleShape*>(shape1);
	CircleShape *c2 = static_cast<CircleShape*>(shape2);
	
	float angle = r1->getAngle();
	float width = r1->getSize().x;
	float height = r1->getSize().y;
	
	float radius = c2->getRadius();
	Vector3D center;
	Vector3DSet(&center, pos2.x, pos2.y, 0);

	Vector3D normals[3];

	//Get the rotated vertices (for AST)
	// TR, TL, BR, BL;
	Vector3D vertices1[4];
	Vector3DSet(&vertices1[0],  width / 2.0f,  height / 2.0f, 0);
	Vector3DSet(&vertices1[1], -width / 2.0f,  height / 2.0f, 0);
	Vector3DSet(&vertices1[2],  width / 2.0f, -height / 2.0f, 0);
	Vector3DSet(&vertices1[3], -width / 2.0f, -height / 2.0f, 0);
	//Rotate them
	Matrix3DRotAxisDeg(&ROT, &axis, angle);
	Matrix3DMultVec(&vertices1[0], &ROT, &vertices1[0]);
	Matrix3DMultVec(&vertices1[1], &ROT, &vertices1[1]);
	Matrix3DMultVec(&vertices1[2], &ROT, &vertices1[2]);
	Matrix3DMultVec(&vertices1[3], &ROT, &vertices1[3]);
	//Translate them back
	Matrix3DTranslate(&T, pos1.x, pos1.y, 0);
	Matrix3DMultVec(&vertices1[0], &T, &vertices1[0]);
	Matrix3DMultVec(&vertices1[1], &T, &vertices1[1]);
	Matrix3DMultVec(&vertices1[2], &T, &vertices1[2]);
	Matrix3DMultVec(&vertices1[3], &T, &vertices1[3]);

	//Get the three evaluation axis
	Vector3D n1_1, n1_2, nCircle;
	Vector3DSub(&n1_1, &vertices1[0], &vertices1[1]);
	Vector3DNormalize(&normals[0], &n1_1);
	Vector3DSub(&n1_2, &vertices1[0], &vertices1[2]);
	Vector3DNormalize(&normals[1], &n1_2);

	//Get the evaluation axis (first find closer vertex to circle center)
	int minDistanceIndex = -1;
	float minDistanceValue = FLT_MAX;
	for (int i = 0; i < 4; ++i) 
	{
		float dist = Vector3DSquareDistance(&vertices1[i], &center);
		if (dist <= minDistanceValue) 
		{
			minDistanceIndex = i;
			minDistanceValue = dist;
		}
	}
	Vector3DSub(&nCircle, &center, &vertices1[minDistanceIndex]);
	Vector3DNormalize(&normals[2], &nCircle);

	//Check for every axis if there is intersection
	float minBacking = FLT_MAX;
	Vector3D minNormal;
	for (int i = 0; i < 3; ++i)
	{
		//First test for the first normal with the 4 vertices
		float min1 = FLT_MAX;
		float max1 = -FLT_MAX;
		for (int j = 0; j < 4; ++j)
		{
			float dot = Vector3DDotProduct(&normals[i], &vertices1[j]);
			if (dot < min1)		min1 = dot;
			if (dot > max1)		max1 = dot;
		}

		//Now check the axis against the circle
		float minC, maxC;
		float dot = Vector3DDotProduct(&normals[i], &center);
		minC = dot - radius;
		maxC = dot + radius;

		if (min1 > maxC)
			return false;
		if (max1 < minC)
			return false;

		//Create the min vector
		if (min1 <= maxC && abs(maxC - min1) < abs(minBacking))
		{
			minBacking = maxC - min1;
			Vector3DSet(&minNormal, normals[i].x, normals[i].y, normals[i].z);
		}
		if (max1 >= minC && abs(max1 - minC) < abs(minBacking))
		{
			minBacking = max1 - minC;
			Vector3DSet(&minNormal, normals[i].x, normals[i].y, normals[i].z);
		}
	}

	//Set and save the contact (with the penVector info)
	Contact *c = new Contact(shape1, shape2);
	Vector3DScale(&minNormal, &minNormal, minBacking);
	Vector3DSet(&c->MTVector, minNormal.x, minNormal.y, minNormal.z);
	contactList.push_back(c);

	//std::cout << "COLLISION BETWEEN RECT RECT: -Min dist: " << minDistance << std::endl;
	return true;
}

bool CheckCollisionCircleRect(Shape *shape1, Vector3D pos1, Shape *shape2, Vector3D pos2, std::vector<Contact*>& contactList)
{
	bool collide = CheckCollisionRectCircle(shape2, pos2, shape1, pos1, contactList);
	return collide;
}

bool CheckCollisionCircleCircle(Shape *shape1, Vector3D pos1, Shape *shape2, Vector3D pos2, std::vector<Contact*>& contactList)
{
	CircleShape *c1 = static_cast<CircleShape*>(shape1);
	CircleShape *c2 = static_cast<CircleShape*>(shape2);

	Vector3DSet(&pos1, pos1.x, pos1.y, 0);
	Vector3DSet(&pos2, pos2.x, pos2.y, 0);

	float dist = Vector3DDistance(&pos1, &pos2);
	float sumOfRadius = c1->getRadius() + c2->getRadius();

	Vector3D normal;
	if (dist < sumOfRadius) 
	{
		float backingAmount = sumOfRadius - dist;

		//Set and save the contact (with the penVector info)
		Contact *c = new Contact(shape1, shape2);
		Vector3DSub(&normal, &pos1, &pos2);
		Vector3DNormalize(&normal, &normal);						//IMPROVE
		Vector3DScale(&normal, &normal, backingAmount);				//IMPROVE
		Vector3DSet(&c->MTVector, normal.x, normal.y, normal.z);
		contactList.push_back(c);
		//std::cout << "COLLISION BETWEEN CIRCLE CIRCLE" << std::endl;
		return true;
	}

	return false;
}