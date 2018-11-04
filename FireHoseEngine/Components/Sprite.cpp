#include "Sprite.h"
#include "../Managers.h"
#include "SDL2/SDL_surface.h"
#include <string>

extern Manager *pManager;

Sprite::Sprite(GameObject *owner, COMPONENT_TYPE type) :
	Component(owner, type)
{
}

Sprite::~Sprite()
{
}

void Sprite::Update(unsigned int deltaTime)
{
	//TODO
}

void Sprite::SetGlParams(GLuint text)
{
	mTexture = text;
}

Component *Sprite::createNew(GameObject *owner)
{
	return new Sprite(owner, COMPONENT_TYPE::SPRITE);
}

void Sprite::serialize(std::fstream& stream)
{
}

void Sprite::deserialize(std::fstream& stream)
{
	std::cout << "DESERIALIZING SPRITE BEGIN" << std::endl;

	//TODO: Replace with safe way of doing

	std::string fullPath = "Resources/";
	std::string imagePath;
	stream >> imagePath;
	fullPath += imagePath;

	SDL_Surface *surf = pManager->GetResourceManager()->loadSurface(fullPath);
	this->mTexture = pManager->GetGraphicManager()->generateTextureFromSurface(surf);

	std::cout << "DESERIALIZING SPRITE END (ORDER IT ALL)" << std::endl;
}