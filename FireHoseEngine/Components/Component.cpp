#include "Component.h"
#include <iostream>


Component::Component(GameObject *owner, COMPONENT_TYPE type) :
	mType(type), mOwner(owner)
{
	//std::cout << "Calling Component's Constructor. Type: " << mType << std::endl;
}

Component::~Component()
{
	//std::cout << "Calling Component's Destructor. Type: " << mType << std::endl;
}

bool Component::isEnabled()
{
	return enabled;
}

void Component::setEnabled(bool flag)
{
	enabled = flag;
}