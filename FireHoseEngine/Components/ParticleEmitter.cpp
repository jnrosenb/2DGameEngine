#include <iostream>
#include "../GameObject.h"
#include "Transform.h"
#include "Camera.h"
#include "ParticleEmitter.h"
#include "../Managers.h"
#include <string>

extern Manager *pManager;


ParticleEmitter::ParticleEmitter(GameObject *owner, COMPONENT_TYPE type) :
	Component(owner, type), maxNumberOfParticles(0), currentNumberOfParticles(0)
{
}


ParticleEmitter::~ParticleEmitter()
{
	glDeleteBuffers(5, buffers);

	//TODO : ParticleEmitter::~ParticleEmitter() -.
	//Have to see which containers we could need 
	//to delete elements or clear
}


void ParticleEmitter::Update(unsigned int deltaTime)
{
	float dt = deltaTime / 1000.0f;

	//First, we need to update all the particles
	int index = 0;
	for (Particle& p : particlesContainer)
	{
		//skip a particle if it is not alive
		if (!p.isAlive()) continue;

		//Physics update for particle
		p.Update(dt, &currentNumberOfParticles);

		//Update the buffers to pass to GPU
		particlesCenterAndSizeArray[index + 0] = p.getCenter().x;
		particlesCenterAndSizeArray[index + 1] = p.getCenter().y;
		particlesCenterAndSizeArray[index + 2] = p.getCenter().z; //+ (index / 4.0f) * 0.001f; //Cheap trick to get transparency
		particlesCenterAndSizeArray[index + 3] = p.getSize();

		particlesColorArray[index + 0] = p.color[0];
		particlesColorArray[index + 1] = p.color[1];
		particlesColorArray[index + 2] = p.color[2];
		particlesColorArray[index + 3] = p.color[3];

		//Get the particle vertex pos info and setup the whole thing
		Vector3D pos = p.getVertexPos();
		particlesVertexInfo[index + 0] = pos.x;
		particlesVertexInfo[index + 1] = pos.y;
		particlesVertexInfo[index + 2] = pos.z;
		particlesVertexInfo[index + 3] = p.GetAngleInRadian();

		//UV EXPERIMENT--begin-end-currFrame--
		particleUVDataArray[index + 0] = static_cast<float>( p.getBegin() );
		particleUVDataArray[index + 1] = static_cast<float>( p.getEnd() );
		particleUVDataArray[index + 2] = static_cast<float>( p.getCurrentFrame() );
		particleUVDataArray[index + 3] = static_cast<float>( p.getAnimationSheetDimention() ); //Empty slot
		//UV EXPERIMENT-----------------

		//Index go up by 4
		index += 4;
	}

	GLenum err = glGetError();
	if (err != GL_NO_ERROR)
	{
		switch (err)
		{
		case GL_INVALID_ENUM:
			std::cout << "(ParticleEmitter::Update)- ERROR: GL_INVALID_ENUM" << std::endl;
			break;
		case GL_INVALID_VALUE:
			std::cout << "(ParticleEmitter::Update)- ERROR: GL_INVALID_VALUE" << std::endl;
			break;
		case GL_INVALID_OPERATION:
			std::cout << "(ParticleEmitter::Update)- ERROR: GL_INVALID_OPERATION" << std::endl;
			break;
		default:
			std::cout << "(ParticleEmitter::Update)- ERROR: None of the previous" << std::endl;
			break;
		}
	}
	if (buffers[1] == buffers[2] && currentNumberOfParticles <= 0)
		std::cout << "WEIRD THING 2" << std::endl;

	//Now, on the openGL side of things, update buffers
	glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
	//glBufferData(GL_ARRAY_BUFFER, maxNumberOfParticles * 4 * sizeof(GL_FLOAT), NULL, GL_STREAM_DRAW); //TEST ORPHANING
	//glBufferSubData(GL_ARRAY_BUFFER, 0, currentNumberOfParticles * 4 * sizeof(particlesCenterAndSizeArray[0]), &particlesCenterAndSizeArray[0]);
	glBufferData(GL_ARRAY_BUFFER, maxNumberOfParticles * 4 * sizeof(particlesCenterAndSizeArray[0]), NULL, GL_STREAM_DRAW); //TEST ORPHANING
	glBufferSubData(GL_ARRAY_BUFFER, 0, currentNumberOfParticles * 4 * sizeof(particlesCenterAndSizeArray[0]), particlesCenterAndSizeArray.data());

	glBindBuffer(GL_ARRAY_BUFFER, buffers[2]);
	//glBufferData(GL_ARRAY_BUFFER, maxNumberOfParticles * 4 * sizeof(GL_UNSIGNED_BYTE), NULL, GL_STREAM_DRAW); //TEST ORPHANING
	//glBufferSubData(GL_ARRAY_BUFFER, 0, currentNumberOfParticles * 4 * sizeof(GL_UNSIGNED_BYTE), &particlesColorArray[0]);
	glBufferData(GL_ARRAY_BUFFER, maxNumberOfParticles * 4 * sizeof(particlesColorArray[0]), NULL, GL_STREAM_DRAW); //TEST ORPHANING
	glBufferSubData(GL_ARRAY_BUFFER, 0, currentNumberOfParticles * 4 * sizeof(particlesColorArray[0]), particlesColorArray.data());

	glBindBuffer(GL_ARRAY_BUFFER, buffers[3]);
	//glBufferData(GL_ARRAY_BUFFER, maxNumberOfParticles * 4 * sizeof(GL_FLOAT), NULL, GL_STREAM_DRAW); //TEST ORPHANING
	//glBufferSubData(GL_ARRAY_BUFFER, 0, currentNumberOfParticles * 4 * sizeof(GL_FLOAT), &particlesVertexInfo[0]);
	glBufferData(GL_ARRAY_BUFFER, maxNumberOfParticles * 4 * sizeof(particlesVertexInfo[0]), NULL, GL_STREAM_DRAW); //TEST ORPHANING
	glBufferSubData(GL_ARRAY_BUFFER, 0, currentNumberOfParticles * 4 * sizeof(particlesVertexInfo[0]), particlesVertexInfo.data());

	//UV EXPERIMENT-----------------
	glBindBuffer(GL_ARRAY_BUFFER, buffers[4]);
	//glBufferData(GL_ARRAY_BUFFER, maxNumberOfParticles * 4 * sizeof(GL_FLOAT), NULL, GL_STREAM_DRAW); //TEST ORPHANING
	//glBufferSubData(GL_ARRAY_BUFFER, 0, currentNumberOfParticles * 4 * sizeof(GL_FLOAT), &particleUVDataArray[0]);
	glBufferData(GL_ARRAY_BUFFER, maxNumberOfParticles * 4 * sizeof(particleUVDataArray[0]), NULL, GL_STREAM_DRAW); //TEST ORPHANING
	glBufferSubData(GL_ARRAY_BUFFER, 0, currentNumberOfParticles * 4 * sizeof(particleUVDataArray[0]), particleUVDataArray.data());
	//UV EXPERIMENT-----------------
}


void ParticleEmitter::Draw(GLuint *program)
{
	InstanceProgram = *program;
	glUseProgram(InstanceProgram);

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
	glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, (void*)0);

	//Pass the particle vertex position info
	glBindBuffer(GL_ARRAY_BUFFER, buffers[3]);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
	
	//UV EXPERIMENT-----------------
	glBindBuffer(GL_ARRAY_BUFFER, buffers[4]);
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
	//UV EXPERIMENT-----------------

	//GLAttribDivisor
	glVertexAttribDivisor(0, 0);
	glVertexAttribDivisor(1, 1);
	glVertexAttribDivisor(2, 1);
	glVertexAttribDivisor(3, 1);
	//UV EXPERIMENT-----------------
	glVertexAttribDivisor(4, 1);
	//UV EXPERIMENT-----------------


	//DRAW THE INSTANCES------------------------
	uParticleSheet = pManager->GetGraphicManager()->particleSystemTexture;
	glBindTexture(GL_TEXTURE_2D, uParticleSheet);

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

	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);
}


void ParticleEmitter::MatricesSetup(GLuint uview, GLuint uproj) 
{
	Camera *C = pManager->GetCameraManager()->GetMainCamera();
	if (C)
	{
		glUniformMatrix4fv(uview, 1, GL_FALSE, &(C->GetViewMatrix().m[0][0]));
		glUniformMatrix4fv(uproj, 1, GL_FALSE, &(C->GetProjection().m[0][0]));
	}
}


void ParticleEmitter::InitEmitter()
{
	glGenBuffers(5, buffers);
	
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

	//Vertex position info, to make particle stay in place
	glBindBuffer(GL_ARRAY_BUFFER, buffers[3]);
	glBufferData(GL_ARRAY_BUFFER, maxNumberOfParticles * 4 * sizeof(GL_FLOAT), NULL, GL_STREAM_DRAW); //TEST WITH DYNAMIC AND STREAM

	//UV EXPERIMENT-----------------
	glBindBuffer(GL_ARRAY_BUFFER, buffers[4]);
	glBufferData(GL_ARRAY_BUFFER, maxNumberOfParticles * 4 * sizeof(GL_FLOAT), NULL, GL_STREAM_DRAW); //TEST WITH DYNAMIC AND STREAM
	//UV EXPERIMENT-----------------

	//Call manager and give it this particle emitter
	pManager->GetGraphicManager()->particleEmitters.push_back(this);
}


void ParticleEmitter::EmitOnce(int numberToEmit)
{
	//The current number of particles needs to grow when wmitting
	this->currentNumberOfParticles += numberToEmit;
	if (currentNumberOfParticles > maxNumberOfParticles)
		currentNumberOfParticles = maxNumberOfParticles;

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

	//Deserialize info about particle animation
	std::string line;

	//PARTICLE REGULAR PARAMETERS
	int maxNum;
	bool gravity;
	float spread, size, speed, rotSpeed, mass, angle;
	if (stream >> line)
	{
		while (stream >> line)
		{
			if (line == "PARAMS_END")
			{
				break;
			}
			else if (line == "number")
			{
				if (stream >> maxNum)
				{
					maxNumberOfParticles = maxNum;
					currentNumberOfParticles = 0;
					currentParticlePtr = 0;
				}
				else
					std::cout << "(ParticleEmitter::deserialize)- Error in stream - number" << std::endl;
			}
			else if (line == "gravity")
			{
				if (stream >> gravity)
					std::cout << "Deserialized gravity parameter" << std::endl;
				else
					std::cout << "(ParticleEmitter::deserialize)- Error in stream - gravity" << std::endl;
			}
			else if (line == "spread")
			{
				if (stream >> spread)
					std::cout << "Deserialized spread parameters" << std::endl;
				else
					std::cout << "(ParticleEmitter::deserialize)- Error in stream - spread" << std::endl;
			}
			else if (line == "size")
			{
				if (stream >> size)
					std::cout << "Deserialized size parameters" << std::endl;
				else
					std::cout << "(ParticleEmitter::deserialize)- Error in stream - size" << std::endl;
			}
			else if (line == "speed")
			{
				if (stream >> speed)
					std::cout << "Deserialized speed parameters" << std::endl;
				else
					std::cout << "(ParticleEmitter::deserialize)- Error in stream - speed" << std::endl;
			}
			else if (line == "rotSpeed")
			{
				if (stream >> rotSpeed)
					std::cout << "Deserialized rotSpeed parameters" << std::endl;
				else
					std::cout << "(ParticleEmitter::deserialize)- Error in stream - rotSpeed" << std::endl;
			}
			else if (line == "mass")
			{
				if (stream >> mass)
					std::cout << "Deserialized mass parameters" << std::endl;
				else
					std::cout << "(ParticleEmitter::deserialize)- Error in stream - mass" << std::endl;
			}
			else if (line == "angle")
			{
				if (stream >> angle)
					std::cout << "Deserialized angle parameters" << std::endl;
				else
					std::cout << "(ParticleEmitter::deserialize)- Error in stream - angle" << std::endl;
			}
		}
	}
	else
	{
		std::cout << "(ParticleEmitter::deserialize)- Error in stream - STRING LINE" << std::endl;
	}

	//ANIMATION PARAMETERS
	bool anim = false, loops;
	int dimR, dimC, r_beg, r_end, fps;
	if (stream >> line)
	{
		std::cout << "Initial line read, should be ANIMATED: " << line << std::endl;
		while (stream >> line) 
		{
			if (line == "ANIMATED_END") 
			{
				break;
			}
			else if (line == "animated") 
			{
				if (stream >> anim)
					std::cout << "Deserialized animated parameter" << std::endl;
				else 
					std::cout << "(ParticleEmitter::deserialize)- Error in stream - animated" << std::endl;
			}
			else if (line == "dimentions")
			{
				if (stream >> dimR >> dimC)
					std::cout << "Deserialized dimentions parameters" << std::endl;
				else
					std::cout << "(ParticleEmitter::deserialize)- Error in stream - dimentions" << std::endl;
			}
			else if (line == "range")
			{
				if (stream >> r_beg >> r_end)
					std::cout << "Deserialized range parameters" << std::endl;
				else
					std::cout << "(ParticleEmitter::deserialize)- Error in stream - range" << std::endl;
			}
			else if (line == "fps")
			{
				if (stream >> fps)
					std::cout << "Deserialized fps parameters" << std::endl;
				else
					std::cout << "(ParticleEmitter::deserialize)- Error in stream - fps" << std::endl;
			}
			else if (line == "loops")
			{
				if (stream >> loops)
					std::cout << "Deserialized loops parameters" << std::endl;
				else
					std::cout << "(ParticleEmitter::deserialize)- Error in stream - loops" << std::endl;
			}
		}
	}
	else 
	{
		std::cout << "(ParticleEmitter::deserialize)- Error in stream - STRING LINE" << std::endl;
	}

	//Reserve the space for the particles
	particlesCenterAndSizeArray.reserve( 4 * maxNumberOfParticles );
	particlesColorArray.reserve( 4 * maxNumberOfParticles ); 
	particlesVertexInfo.reserve( 4 * maxNumberOfParticles );
	particleUVDataArray.reserve( 4 * maxNumberOfParticles );
		
	//TODO check if this is really an efficient way of filling vector
	particlesContainer.reserve(maxNumberOfParticles);
	for (int i = 0; i < maxNumberOfParticles; ++i)
	{
		//Create a new particle, push into vector
		Particle p(size, angle, mass, speed, rotSpeed, spread, gravity);
		if (anim)
			p.SetAnimationParameters(anim, dimR, dimC, r_beg, r_end, fps, loops);
		particlesContainer.push_back(p);

		//Fill vertex buffer with initial values
		for (int i = 0; i < 4; ++i)
		{
			particlesVertexInfo.push_back(0.0f);
			particlesCenterAndSizeArray.push_back(0.0f);
			particlesColorArray.push_back(0);
			particleUVDataArray.push_back(0.0f);
		}
	}

	//TODO : move this to other place later
	// The idea is that maybe it is not necessary to init this for every different emitter
	// (But maybe it is. Find out)
	InitEmitter();

	std::cout << "DESERIALIZING PARTICLE_EMITTER END" << std::endl;
}


void ParticleEmitter::handleEvent(Event *pEvent)
{
}


