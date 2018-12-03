#include <iostream>
#include "../GameObject.h"
#include "Transform.h"
#include "Camera.h"
#include "ParticleEmitter.h"
#include "../Managers.h"

extern Manager *pManager;


ParticleEmitter::ParticleEmitter(GameObject *owner, COMPONENT_TYPE type) :
	Component(owner, type), maxNumberOfParticles(0), currentNumberOfParticles(0)
{
}

ParticleEmitter::~ParticleEmitter()
{
	glDeleteBuffers(3, buffers);

	//TODO : ParticleEmitter::~ParticleEmitter() -.
	//Have to see which containers we could need 
	//to delete elements or clear
}

void ParticleEmitter::Update(unsigned int deltaTime)
{
	float dt = deltaTime / 1000.0f;

	if (this->asd) //DELETE SOON//DELETE SOON//DELETE SOON//DELETE SOON
	{//DELETE SOON//DELETE SOON//DELETE SOON//DELETE SOON
		return;//DELETE SOON//DELETE SOON
	}//DELETE SOON//DELETE SOON//DELETE SOON//DELETE SOON

	//First, we need to update all the particles
	int index = 0;
	for (Particle& p : particlesContainer)
	{
		//skip a particle if it is not alive
		if (!p.isAlive()) continue;

		//Physics update for particle
		p.Update(dt, &currentNumberOfParticles);

		//Update the buffers to pass to GPU
		particlesCenterAndSizeArray[index] = p.getCenter().x;
		particlesCenterAndSizeArray[index + 1] = p.getCenter().y;
		particlesCenterAndSizeArray[index + 2] = p.getCenter().z;
		particlesCenterAndSizeArray[index + 3] = p.getSize();

		particlesColorArray[index] = p.color[0];
		particlesColorArray[index + 1] = p.color[1];
		particlesColorArray[index + 2] = p.color[2];
		particlesColorArray[index + 3] = p.color[3];

		//Get the particle vertex pos info and setup the whole thing
		Vector3D pos = p.getVertexPos();
		particlesVertexInfo[index] = pos.x;
		particlesVertexInfo[index + 1] = pos.y;
		particlesVertexInfo[index + 2] = pos.z;
		particlesVertexInfo[index + 3] = pos.z;


		//Index go up by 4
		index += 4;
	}

	//Now, on the openGL side of things, update buffers
	glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
	glBufferData(GL_ARRAY_BUFFER, maxNumberOfParticles * 4 * sizeof(GL_FLOAT), NULL, GL_STREAM_DRAW); //TEST ORPHANING
	glBufferSubData(GL_ARRAY_BUFFER, 0, currentNumberOfParticles * 4 * sizeof(GL_FLOAT), &particlesCenterAndSizeArray[0]);

	glBindBuffer(GL_ARRAY_BUFFER, buffers[2]);
	glBufferData(GL_ARRAY_BUFFER, maxNumberOfParticles * 4 * sizeof(GL_UNSIGNED_BYTE), NULL, GL_STREAM_DRAW); //TEST ORPHANING
	glBufferSubData(GL_ARRAY_BUFFER, 0, currentNumberOfParticles * 4 * sizeof(GL_UNSIGNED_BYTE), &particlesColorArray[0]);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	glBindBuffer(GL_ARRAY_BUFFER, buffers[3]);
	glBufferData(GL_ARRAY_BUFFER, maxNumberOfParticles * 4 * sizeof(GL_FLOAT), NULL, GL_STREAM_DRAW); //TEST ORPHANING
	glBufferSubData(GL_ARRAY_BUFFER, 0, currentNumberOfParticles * 4 * sizeof(GL_FLOAT), &particlesVertexInfo[0]);
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}


void ParticleEmitter::Draw(GLuint *program)
{
	InstanceProgram = *program;
	glUseProgram(InstanceProgram);

	//TODO: Make efficient
	//Set the current MVP matrix
	GLuint uview = glGetUniformLocation(InstanceProgram, "view");
	GLuint uproj = glGetUniformLocation(InstanceProgram, "proj");
	MatricesSetup(uview, uproj);

	//Pass the particle vertex info
	glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	//Pass the particle center and size info
	glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);

	//Pass the particle color info
	glBindBuffer(GL_ARRAY_BUFFER, buffers[2]);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, (void*)0); //Normalized so it enters shader as a float vec4

	//Pass the particle color info/////////////////////////////////////////////////////////////////////////////////////////
	glBindBuffer(GL_ARRAY_BUFFER, buffers[3]);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//GLAttribDivisor
	glVertexAttribDivisor(0, 0);
	glVertexAttribDivisor(1, 1);
	glVertexAttribDivisor(2, 1);
	glVertexAttribDivisor(3, 1);

	//Draw
	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, currentNumberOfParticles);
	GLenum err = glGetError();
	if (err != GL_NO_ERROR)
	{
		switch (err)
		{
		case GL_INVALID_ENUM:
			std::cout << "(ParticleEmitter::Draw)- ERROR: GL_INVALID_ENUM" << std::endl;
			break;
		case GL_INVALID_VALUE:
			std::cout << "(ParticleEmitter::Draw)- ERROR: GL_INVALID_VALUE" << std::endl;
			break;
		case GL_INVALID_OPERATION:
			std::cout << "(ParticleEmitter::Draw)- ERROR: GL_INVALID_OPERATION" << std::endl;
			break;
		default:
			std::cout << "(ParticleEmitter::Draw)- ERROR: None of the previous" << std::endl;
			break;
		}
	}

	glUseProgram(0);
}


void ParticleEmitter::MatricesSetup(GLuint uview, GLuint uproj) 
{
	Camera *C = pManager->GetCameraManager()->GetMainCamera();
	if (C)
	{
		//TODO: maybe not necessary to call this for at least VIEW and PROJ
		glUniformMatrix4fv(uview, 1, GL_FALSE, &(C->GetViewMatrix().m[0][0]));
		glUniformMatrix4fv(uproj, 1, GL_FALSE, &(C->GetProjection().m[0][0]));
	}
}


void ParticleEmitter::InitEmitter()
{
	glGenBuffers(4, buffers);
	
	float vertices[] =
	{
		- 0.5f, + 0.5f, + 0.0f, // TL - 0
		+ 0.5f, + 0.5f, + 0.0f, // TR - 1
		- 0.5f, - 0.5f, + 0.0f, // BL - 2
		+ 0.5f, - 0.5f, + 0.0f, // BR - 0
	};

	///Vertex buffer (6 vertices, does not change)
	glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//Center and size of particles
	glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
	glBufferData(GL_ARRAY_BUFFER, maxNumberOfParticles * 4 * sizeof(GL_FLOAT), NULL, GL_STREAM_DRAW); //TEST WITH DYNAMIC AND STREAM

	//Center and size of particles
	glBindBuffer(GL_ARRAY_BUFFER, buffers[2]);
	glBufferData(GL_ARRAY_BUFFER, maxNumberOfParticles * 4 * sizeof(GL_UNSIGNED_BYTE), NULL, GL_STREAM_DRAW); //TEST WITH DYNAMIC AND STREAM

	///Vertex buffer (6 vertices, does not change)
	glBindBuffer(GL_ARRAY_BUFFER, buffers[3]);
	glBufferData(GL_ARRAY_BUFFER, maxNumberOfParticles * 4 * sizeof(GL_FLOAT), NULL, GL_STREAM_DRAW); //TEST WITH DYNAMIC AND STREAM
}


void ParticleEmitter::EmitOnce(int numberToEmit)
{
	//The current number of particles needs to add those
	//That are gonna be newly emitted now
	this->currentNumberOfParticles += numberToEmit;
	if (currentNumberOfParticles > maxNumberOfParticles)
		currentNumberOfParticles = maxNumberOfParticles;

	//DEBUG-ONLY///////////////////////////////////////////////////////////////////////////////////
	if (this->currentNumberOfParticles == 0) //////////////////////////////////////////////////////
	{//////////////////////////////////////////////////////////////////////////////////////////////
		std::cout << "(ParticleEmitter::EmitOnce)- ERROR ERROR ERROR ERROR ERROR" << std::endl;////
	}//////////////////////////////////////////////////////////////////////////////////////////////
	//DEBUG-ONLY///////////////////////////////////////////////////////////////////////////////////

	///Create new vertices and gl-stuff
	//initEmitterForNewWave(numberToEmit);

	//Get the new number of particles needed ready for updating
	for (int i = 0; i < numberToEmit; ++i) 
	{
		int index = getParticleIndexToEmit();
		Particle& p = particlesContainer[index];
		p.ResetParticle();

		Transform *T = static_cast<Transform*>(getOwner()->GetComponent(COMPONENT_TYPE::TRANSFORM));
		if (T)
		{
			Vector3D pos = T->getPosition();
			p.saveVertexPos(pos);
		}
	}

	if (asd) //DELETE SOON//DELETE SOON//DELETE SOON//DELETE SOON
	{//DELETE SOON//DELETE SOON//DELETE SOON//DELETE SOON
		asd = false;//DELETE SOON//DELETE SOON//DELETE SOON
		std::cout << "ASDASDASDASDASD" << std::endl;//DELETE SOON


		//For now, we can pass this to graphicManager 
		//so it draws it every frame starting now
		pManager->GetGraphicManager()->particleEmitters.push_back(this);


	}//DELETE SOON//DELETE SOON//DELETE SOON//DELETE SOON
}


int ParticleEmitter::getParticleIndexToEmit()
{
	//First search from the last point checked to the end of the container
	for (int i = currentParticlePtr; i < maxNumberOfParticles; ++i) 
	{
		Particle& p = particlesContainer[i];
		if (!p.isAlive()) 
		{
			currentParticlePtr = i;
			return i;
		}
	}

	//If nothing there is free, check from beggining
	for (int i = 0; i < currentParticlePtr; ++i)
	{
		Particle& p = particlesContainer[i];
		if (!p.isAlive())
		{
			currentParticlePtr = i;
			return i;
		}
	}

	//If nothing is free in all container, override first element
	return 0;
}


Component *ParticleEmitter::createNew(GameObject *owner)
{
	return new ParticleEmitter(owner, COMPONENT_TYPE::PARTICLE_EMITTER);
}


void ParticleEmitter::serialize(std::fstream& stream)
{
}


void ParticleEmitter::deserialize(std::fstream& stream)
{
	std::cout << "DESERIALIZING PARTICLE_EMITTER BEGIN" << std::endl;
	
	//DELETE SUPER SOON//DELETE SOON//DELETE SOON//DELETE SOON
	asd = true;//DELETE SOON//DELETE SOON//DELETE SOON//DELETE SOON
	//DELETE SOON//DELETE SOON//DELETE SOON//DELETE SOON

	int maxNum;
	if (stream >> maxNum)
	{
		maxNumberOfParticles = maxNum;
		currentNumberOfParticles = 0;
		currentParticlePtr = 0;

		//Reserve the space for the particles
		particlesCenterAndSizeArray.reserve( 4 * maxNumberOfParticles );
		particlesColorArray.reserve( 4 * maxNumberOfParticles ); 
		particlesVertexInfo.reserve( 4 * maxNumberOfParticles ); //VERTEX EXPERIMENT
		
		//TODO check if this is really an efficient way of filling vector
		particlesContainer.reserve(maxNumberOfParticles);
		for (int i = 0; i < maxNumberOfParticles; ++i) 
		{
			//For now, lets hardcode this here////////////
			float timeToLive = 1.0f * (rand()%5);
			float size = (rand() % 2) / 4.0f, angle = 0.0f, mass = 10.0f, speed = (rand() % 4) / 1.0f;
			char r = rand()%256, g = rand() % 256, b = rand() % 256, a = 255;
			Vector3D position, vel;
			Vector3DSet(&position, 0, 0, 0);
			Vector3DSet(&vel, 0, 10, 0);
			//For now, lets hardcode this here////////////

			//Create a new particle, push into vector
			Particle p(timeToLive, size, angle, mass, speed, 
				r, g, b, a, position, vel);
			particlesContainer.push_back(p);

			//Fill vertex buffer with initial values
			for (int i = 0; i < 4; ++i)
				particlesVertexInfo.push_back(0.0f);

			//Fill center buffer with initial values
			for (int i = 0; i < 4; ++i)
				particlesCenterAndSizeArray.push_back(0.0f);

			//Also color initial values
			for (int i = 0; i < 4; ++i)
				particlesColorArray.push_back(0);
		}

		//TODO : move this to other place later
		// The idea is that maybe it is not necessary to init this for every different emitter
		// (But maybe it is. Find out)
		InitEmitter();
	}
	else
	{
		std::cout << "(ParticleEmitter::deserialize)- Error reading the stream" << std::endl;
	}

	std::cout << "DESERIALIZING PARTICLE_EMITTER END" << std::endl;
}

void ParticleEmitter::handleEvent(Event *pEvent)
{
}


