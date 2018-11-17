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

bool Sprite::isAnimated()
{
	return animated;
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
	if (stream >> imagePath) 
	{
		fullPath += imagePath;

		SDL_Surface *surf = pManager->GetResourceManager()->loadSurface(fullPath);
		this->mTexture = pManager->GetGraphicManager()->generateTextureFromSurface(surf);

		bool anim;
		if (stream >> anim)
		{
			animated = anim;
			if (animated) 
			{
				unsigned int r, c;
				if (stream >> r >> c)
				{
					rows = r;
					cols = c;

					//Setup sprite uvs
					setupSpriteUvs();
				}
				else
				{
					std::cout << "STREAM ERROR when deserializing sprite rows and cols" << std::endl;
				}
			}
			else 
			{
				uvVBO = 0;
			}
		}
		else 
		{
			std::cout << "STREAM ERROR when deserializing sprite anim flag" << std::endl;
		}
	}
	else 
	{
		std::cout << "STREAM ERROR when deserializing sprite texture" << std::endl;
	}

	std::cout << "DESERIALIZING SPRITE END (ORDER IT ALL)" << std::endl;
}

std::vector<float> const& Sprite::getSpriteUvs()
{
	return spriteUvs;
}

void Sprite::setupSpriteUvs() 
{
	spriteUvs.reserve(rows * cols * 8);
	for (int i = 0; i < rows; ++i)
	{
		for (int j = 0; j < cols; ++j)
		{
			spriteUvs.push_back(j * (1.0f / cols));
			spriteUvs.push_back(i * (1.0f / rows));

			spriteUvs.push_back(j * (1.0f / cols) + 1.0f / cols);
			spriteUvs.push_back(i * (1.0f / rows));

			spriteUvs.push_back(j * (1.0f / cols));
			spriteUvs.push_back(i * (1.0f / rows) + 1.0f / rows);

			spriteUvs.push_back(j * (1.0f / cols) + 1.0f / cols);
			spriteUvs.push_back(i * (1.0f / rows) + 1.0f / rows);
		}
	}

	//Once the uvs are done, generate the new VBO
	glGenBuffers(1, &uvVBO);
	glBindBuffer(GL_ARRAY_BUFFER, uvVBO);
	unsigned int size = rows * cols * 8 * sizeof(GL_FLOAT);
	glBufferData(GL_ARRAY_BUFFER, size, &spriteUvs[0], GL_STATIC_DRAW);
}

unsigned int Sprite::getSpriteVboUv()
{
	return uvVBO;
}