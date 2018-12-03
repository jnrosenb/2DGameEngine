#ifndef PARTICLEEMITTER_H
#define PARTICLEEMITTER_H

#include "Component.h"
#include "../Particles/Particles.h"
#include <fstream>
#include "../Events.h"
#include <vector>
#include "GL/glew.h"			//TODO CHANGE MAYBE TO FORWARD DECLARATION
#include "GL/gl.h"				//TODO CHANGE MAYBE TO FORWARD DECLARATION

class GameObject;

class ParticleEmitter : public Component
{
public:
	ParticleEmitter(GameObject *owner, COMPONENT_TYPE type);
	virtual ~ParticleEmitter();

	virtual void Update(unsigned int deltaTime);
	void Draw(GLuint *program);
	void InitEmitter();
	void MatricesSetup(GLuint uview, GLuint uproj);
	void EmitOnce(int numberToEmit);
	int getParticleIndexToEmit();

	virtual Component *createNew(GameObject *owner);
	virtual void serialize(std::fstream& stream);
	virtual void deserialize(std::fstream& stream);
	virtual void handleEvent(Event *pEvent);

private:
	int maxNumberOfParticles;
	int currentNumberOfParticles;
	int currentParticlePtr;

	//DELETE SOON
	bool asd;//DELETE SOON
	//DELETE SOON

	//GL buffer names
	GLuint buffers[4]; //CHANGE TO 3 IF IT FAILS
	GLuint InstanceProgram;

	//Not sure if here I should use pointers or values
	std::vector<Particle> particlesContainer;

	//Apart from the particles, I need two containers 
	//to pass the data to openGl buffers
	std::vector<float> particlesVertexInfo; //VERTEX EXPERIMENT
	std::vector<float> particlesCenterAndSizeArray;
	std::vector<unsigned char> particlesColorArray;

};


class EmitterCallback : public callbackEvent
{
public:
	//AnimatorCallback() : callbackEvent()
	EmitterCallback(ParticleEmitter *obj, void (ParticleEmitter::*mthd)())
	{
		object = obj;
		method = mthd;
	}

	virtual ~EmitterCallback() { }

	virtual void callback()
	{
		(object->*method)();
	}

private:
	ParticleEmitter *object;
	void (ParticleEmitter::*method)();

private:
	EmitterCallback(EmitterCallback const& rhs);
	EmitterCallback& operator=(EmitterCallback const& rhs);
};


#endif