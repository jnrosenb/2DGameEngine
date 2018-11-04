#ifndef BOXCOLLIDER2D_H
#define BOXCOLLIDER2D_H

#include "Component.h"
#include "../Math/Vector3D.h"

class GameObject;

class BoxCollider2D : public Component
{
public:
	BoxCollider2D(GameObject *owner, COMPONENT_TYPE type);
	~BoxCollider2D();

	virtual void Update(unsigned int deltaTime);
	void DrawBoundingBox();

	virtual Component *createNew(GameObject *owner);
	virtual void serialize(std::fstream& stream);
	virtual void deserialize(std::fstream& stream);

private:
	Vector3D center;
	float width, height;
	float angle;

	//FOR DEBUGGING
	float vertices[24];
};

#endif 