#ifndef COLLISIONMANAGER_H
#define COLLISIONMANAGER_H

#include <vector>
#include "Collision/Shapes.h"
#include "Math/Vector3D.h"

class Contact;
class Trigger;

enum class CollisionMask 
{
	GROUND = 0,
	WALL,
	UI,
	STATIC_OBJ,
	DINAMIC_OBJ,
	PLAYER,
	ENEMY,
	NUM
};

class CollisionManager 
{
public:
	CollisionManager();
	~CollisionManager();

	void Update(unsigned int deltaTime);
	void InitCollisionFunctions();
	void AddContacts(Contact *c1);
	void ResetContacts();
	std::vector<Contact*>& GetContacts();
	bool checkCollision(Shape *shape1, Vector3D pos1, Shape *shape2, Vector3D pos2);

	//Trigger experiment
	//TODO check dangling pointer
	void addTrigger(Trigger *trigger);
	void removeTrigger(Trigger *trigger);
	//TODO check dangling pointer
	std::vector<Trigger*> const& getTriggerList();

private:
	void TriggerCollisionManagement();

private:
	//TODO DANGLING POINTER
	std::vector<Contact*> contacts;
	//TODO DANGLING POINTER
	std::vector<Trigger*> triggers;

	//Array of pointer to functions that deal with collision depending on shape type
	bool (*CheckCollision[ShapeType::NUM][ShapeType::NUM])(Shape *shape1, Vector3D pos1, Shape *shape2, Vector3D pos2, 
		std::vector<Contact*>& contactList);
};



class Contact 
{
public:
	Contact(Shape *shp1, Shape *shp2);
	~Contact();

	Shape *getFirstShape();
	Shape *getSecondShape();

	Vector3D MTVector;

private:
	Shape *collidingShapes[2];
};

#endif;