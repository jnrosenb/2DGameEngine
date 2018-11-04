#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <vector>

class Component; /*Forward declaration*/

class GameObject 
{
public:
	GameObject();
	~GameObject();
	
	void Update(unsigned int deltaTime);
	void Draw();

	void AddComponent(unsigned int type);
	void AddComponent(Component *c);
	Component *GetComponent(unsigned int type);

private:
	std::vector<Component*> goComponents;

};

#endif