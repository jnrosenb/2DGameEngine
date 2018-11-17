#ifndef SPRITE_H
#define SPRITE_H

#include "GL/glew.h"
#include "GL/gl.h"
#include "Component.h"
#include <vector>

class Sprite : public Component
{
public:
	Sprite(GameObject *owner, COMPONENT_TYPE type);
	~Sprite();

	void Update(unsigned int deltaTime); 
	void SetGlParams(GLuint text);

	virtual Component *createNew(GameObject *owner);
	virtual void serialize(std::fstream& stream);
	virtual void deserialize(std::fstream& stream);

	void setupSpriteUvs();
	bool isAnimated();

	unsigned int getSpriteVboUv();
	std::vector<float> const& getSpriteUvs();

	//This should later be made private
	GLuint mTexture;

private:
	//EXPERIMENT
	bool animated;
	unsigned int rows;
	unsigned int cols;
	unsigned int uvVBO;

	std::vector<float> spriteUvs;
};

#endif