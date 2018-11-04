#ifndef COLLISIONMANAGER_H
#define COLLISIONMANAGER_H

#include <vector>
#include "Collision/Shapes.h"
#include "Math/Vector3D.h"

class Contact;

class BoxCollider2D;

class CollisionManager 
{
public:
	CollisionManager();
	~CollisionManager();

	void InitCollisionFunctions();
	void AddContacts(Contact *c1);
	void ResetContacts();
	std::vector<Contact*>& GetContacts();
	bool checkCollision(Shape *shape1, Vector3D pos1, Shape *shape2, Vector3D pos2);

private:
	std::vector<Contact*> contacts;
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

	Vector3D penetrationVec;
private:
	Shape *collidingShapes[2];
};

#endif;