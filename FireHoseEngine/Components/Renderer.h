#ifndef RENDERER_H
#define RENDERER_H

#include "GL/glew.h"
#include "GL/gl.h"
#include "Component.h"

class Camera;
class Sprite;
class Transform;

class Renderer : public Component
{
public:
	Renderer(GameObject *owner, COMPONENT_TYPE type);
	~Renderer();

	void Update(unsigned int deltaTime);
	void Draw();
	void SetGlParams(GLuint pgm, GLuint vao, bool isInstancing);
	void SetGlParams();
	void SendOpenGLInstanceData();

	virtual Component *createNew(GameObject *owner);
	virtual void serialize(std::fstream& stream);
	virtual void deserialize(std::fstream& stream);
	
	bool isEnabled();
	void setEnabled(bool flag);

private:
	bool enabled;
	bool isInstancing;
	GLuint mProgram;
	GLuint mVao;
	GLint umodel;
};

#endif