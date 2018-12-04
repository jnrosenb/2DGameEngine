/* Start Header -------------------------------------------------------
*  Copyright (C) 2018 DigiPen Institute of Technology.
*  Reproduction or disclosure of this file or its contents without the prior
*  written consent of DigiPen Institute of Technology is prohibited.

*  File Name: GraphicsManager.cpp
*  Purpose: Handles the initialization of openGL and the draw calls.
*  Project: CS529_JRosenbluth_3
*  Author: Jose Rosenbluth Chiu, J.Rosenbluth, 60003418
*  Creation date: 10/14/2018
-  End Header --------------------------------------------------------*/

#include "SDL2/SDL_surface.h"
#include "GraphicsManager.h"
#include "Managers.h"
#include "Components/Renderer.h"
#include "Components/Transform.h"
#include "Components/Camera.h"
#include "Components/Trigger.h"
#include "Components/ParticleEmitter.h"
#include "GameObject.h"
#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;
extern Manager *pManager; /*EXPERIMENT*/


GraphicsManager::GraphicsManager() : 
	currentTexture(0), debugMode(false)
{
	cout << "Graphics manager constructor." << endl;

	//Initialize texture dict to -1
	for (auto node : texturesDict) 
	{
		node.second = -1;
	}
}


GraphicsManager::~GraphicsManager()
{
	//TODO: check if vector in this class and others need clearing
	/*Even though this class does not get rid of the components
	(GO does that), it may need to clear the vector*/

	glUseProgram(0);
	glDeleteProgram(spriteProgram);
	glDeleteProgram(instancingProgram);
	glDeleteProgram(debugProgram);

	glDeleteVertexArrays(1, &quadVao);
	glDeleteBuffers(2, vbo);	//For now 2, vertex and uv
	glDeleteBuffers(1, &ebo);
	
	///TODO****************************
	//Clear out the textures in the map
	//texturesDict
	glDeleteTextures(NUMTEXTURES, textures);

	//PARTICLE SYSTEM (move in the future to PSMgr)
	glDeleteTextures(1, &particleSystemTexture);

	//Clear vectors
	particleEmitters.clear();
	renderers.clear();

	cout << "Graphics manager destructor." << endl;
}


void GraphicsManager::Unload()
{
	//Only clear. GO will delete the components
	renderers.clear();
	texturesDict.clear();
}


void GraphicsManager::draw()
{
	Camera *C = pManager->GetCameraManager()->GetMainCamera();
	if (C)
	{
		//JUST IN DEBUG MODE
		if (debugMode) 
		{
			//std::cout << "Number of textures: " << texturesDict.size() << std::endl;
			//std::cout << "Number of sprites : " << renderers.size() << std::endl;

			glUseProgram(debugProgram);
			glUniformMatrix4fv(uview3, 1, GL_FALSE, &(C->GetViewMatrix().m[0][0]));
			glUniformMatrix4fv(uproj3, 1, GL_FALSE, &(C->GetProjection().m[0][0]));
		}

		glUseProgram(spriteProgram);
		glUniformMatrix4fv(uview, 1, GL_FALSE, &(C->GetViewMatrix().m[0][0]));
		glUniformMatrix4fv(uproj, 1, GL_FALSE, &(C->GetProjection().m[0][0]));

		glClearColor(0.0f, 0.0f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glClearDepth(1);
		glClear(GL_DEPTH_BUFFER_BIT);

		for (Renderer* R : renderers) 
		{
			//DRAW THE OBJECT
			R->Draw();
			
			//DRAW BOUNDING BOX OR CIRCLE (JUST IN DEBUG MODE)
			if (debugMode && R->isEnabled())
			{
				//COLLISION BOUNDING SHAPE
				RigidBody2D *rgbdy = static_cast<RigidBody2D*>(R->getOwner()->GetComponent(COMPONENT_TYPE::RIGIDBODY2D));
				if (rgbdy && rgbdy->isEnabled())
				{
					Shape *shape = rgbdy->GetShape();
					if (shape->GetType() == ShapeType::RECTANGLE)
					{
						RectangleShape *r = static_cast<RectangleShape*>(shape);
						DrawBoundingBox(r, DEBUGMODE::COLLISION);
					}
					else if (shape->GetType() == ShapeType::CIRCLE)
					{
						CircleShape *c = static_cast<CircleShape*>(shape);
						DrawBoundingCircle(c, DEBUGMODE::COLLISION);
					}
				}

				//TRIGGER BOUNDING SHAPE
				Trigger *trigger = static_cast<Trigger*>(R->getOwner()->GetComponent(COMPONENT_TYPE::TRIGGER));
				if (trigger && trigger->isEnabled()) 
				{
					Shape *shape = trigger->GetShape();
					if (shape->GetType() == ShapeType::RECTANGLE)
					{
						RectangleShape *r = static_cast<RectangleShape*>(shape);
						DrawBoundingBox(r, DEBUGMODE::TRIGGER);
					}
					else if (shape->GetType() == ShapeType::CIRCLE)
					{
						CircleShape *c = static_cast<CircleShape*>(shape);
						DrawBoundingCircle(c, DEBUGMODE::TRIGGER);
					}
				}
			}
		}

		for (ParticleEmitter* PE : particleEmitters) 
		{
			//DRAW EMITTERS, WHICH WILL IN TURN DRAW THEIR PARTICLES
			PE->Draw(&instancingProgram);
		}
	}
}


void GraphicsManager::AddRendererComponent(Renderer* R)
{
	if (R != 0)
		renderers.push_back(R);
}


void GraphicsManager::ToggleDebugMode()
{
	debugMode = !debugMode;
}


void GraphicsManager::init(int width, int height)
{
	cout << "Started initializing the graphics manager!" << endl;
	this->width = width;
	this->height = height;

	//Create one program per shader
	spriteProgram = glCreateProgram();
	instancingProgram = glCreateProgram();
	debugProgram = glCreateProgram();
	setupShaders("sprite.vert", "sprite.frag", spriteProgram);
	setupShaders("spriteInstancing.vert", "spriteInstancing.frag", instancingProgram);
	setupShaders("debug.vert", "debug.frag", debugProgram);

	uview = glGetUniformLocation(spriteProgram, "view");
	uproj = glGetUniformLocation(spriteProgram, "proj");
	
	uview2 = glGetUniformLocation(spriteProgram, "view");
	uproj2 = glGetUniformLocation(spriteProgram, "proj");
	
	uview3  = glGetUniformLocation(debugProgram, "view");
	uproj3  = glGetUniformLocation(debugProgram, "proj");
	umodel3 = glGetUniformLocation(debugProgram, "model");
	
	//Uniform for debug shader
	uDebugColor = glGetUniformLocation(debugProgram, "color");

	//Example-quad------------
	float vertices[] = {
		-0.5f,  0.5f, 0.0f,
		 0.5f,  0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f };
	int indices[] =
	{
		0, 1, 2,
		1, 2, 3
	};
	float uv[] =
	{
		0.0f, 0.0f,
		1.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 1.0f
	};
	//------------------------

	glGenVertexArrays(1, &quadVao); //Vertex array object
	glGenBuffers(2, vbo); //Vertices and uv coords
	glGenBuffers(1, &ebo); //indices

	glBindVertexArray(quadVao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(uv), uv, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	
	glBindVertexArray(0);

	PrepareParticleSystemData();

	cout << "Finished initializing the graphics manager!" << endl;
}


//TODO - ONLY USE THIS WHILE NOT HAVING A PARTICLE-SYSTEM-MGR
void GraphicsManager::PrepareParticleSystemData()
{
	std::cout << "SAVING PARTICLE SPRITE SHEET TEXTURE*********************" << std::endl;

	//Load and store the particleSystem texture
	glGenTextures(1, &particleSystemTexture);
	SDL_Surface *surf = pManager->GetResourceManager()->loadSurface("Resources/ParticleSheet.png");
	if (surf)
	{
		this->particleSystemTexture = generateTextureFromSurface(surf, "ParticleSheet");
		std::cout << "LOADED PARTICLE SPRITE SHEET TEXTURE INTO OPENGL*********" << std::endl;
	}
}


void GraphicsManager::setupShaders(const char *vertexPath, const char *fragmentPath, GLuint program)
{
	int status = 0;
	char infolog[512];

	std::string vertexString = loadFile(vertexPath);
	const char *vertexSource = vertexString.c_str();
	GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vShader, 1, &vertexSource, NULL);
	glCompileShader(vShader);
	glGetShaderiv(vShader, GL_COMPILE_STATUS, &status);
	if (!status) 
	{
		cout << "Problem when compiling vertex shader." << endl;
		glGetShaderInfoLog(vShader, 512, 0, infolog);
		cout << infolog << endl;
		return;
	}

	std::string fragmentString = loadFile(fragmentPath);
	const char *fragmentSource = fragmentString.c_str();
	GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fShader, 1, &fragmentSource, NULL);
	glCompileShader(fShader);
	glGetShaderiv(fShader, GL_COMPILE_STATUS, &status);
	if (!status)
	{
		cout << "Problem when compiling fragment shader." << endl;
		glGetShaderInfoLog(fShader, 512, 0, infolog);
		cout << infolog << endl;
		return;
	}

	glAttachShader(program, vShader);
	glAttachShader(program, fShader);
	glLinkProgram(program); 
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	if (!status)
	{
		cout << "Problem when linking program." << endl;
		glGetProgramInfoLog(program, 512, 0, infolog);
		cout << infolog << endl;
		return;
	}

	glDeleteShader(vShader);
	glDeleteShader(fShader);
}


GLuint GraphicsManager::generateTextureFromSurface(SDL_Surface *surface, std::string key)
{
	//First check if the map has the texture
	GLint mTexture = texturesDict[key];

	if (mTexture == 0) 
	{
		//Textures generation
		glGenTextures(1, textures + currentTexture);
		glBindTexture(GL_TEXTURE_2D, textures[currentTexture]);
		int mode = GL_RGB;
		if (surface->format->BytesPerPixel == 4)
		{
			mode = GL_RGBA;
			cout << "LOADING TEXTURE IS RGBA MODE" << endl;
		}

		glTexImage2D(GL_TEXTURE_2D, 0, mode, (*surface).w, (*surface).h, 0, mode, GL_UNSIGNED_BYTE, (*surface).pixels);
		if (glGetError() != GL_NO_ERROR)
		{
			cout << "PROBLEM IN LOADING TEXTURE" << endl;
		}
		else
		{
			cout << "LOADING TEXTURE OK. surface: " << surface << ". Surface bytes per pixel: " << static_cast<int>(surface->format->BytesPerPixel) << endl;
		}

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		++currentTexture;
		//return textures[currentTexture - 1];

		//New code
		texturesDict[key] = textures[currentTexture - 1];
		mTexture = texturesDict[key];
	}

	return mTexture;
}


std::string GraphicsManager::loadFile(const char *path)
{
	string out, line;

	ifstream in(path);
	if (in.is_open())
	{
		getline(in, line);
		while (in)
		{
			out += line + "\n";
			getline(in, line);
		}
		in.close();
		return out;
	}
	else
	{
		cout << "Failed to open file!" << endl;
		return "";
	}
}


GLuint GraphicsManager::getProgram(int instancing) 
{
	if (instancing)
		return instancingProgram;
	return spriteProgram;
}


GLuint GraphicsManager::getVao()
{
	return quadVao;
}


////////////////////////////////////////////////////////////////////////
/////////   DEBUGGING BOUNDING SHAPES   ////////////////////////////////
////////////////////////////////////////////////////////////////////////
void GraphicsManager::DrawBoundingBox(RectangleShape *r, DEBUGMODE mode)
{
	Component *ownerComp = r->GetShapeOwner();
	GameObject *owner = ownerComp->getOwner();
	Transform *T = static_cast<Transform*>(owner->GetComponent(COMPONENT_TYPE::TRANSFORM));
	if (T == 0)
		return;

	//COLOR OF THE DEBUG LINES
	if (mode == DEBUGMODE::COLLISION)
	{
		debugColor[0] = 0;
		debugColor[1] = 1;
		debugColor[2] = 0;
	}
	else if (mode == DEBUGMODE::TRIGGER)
	{
		debugColor[0] = 0;
		debugColor[1] = 0;
		debugColor[2] = 1;
	}

	//Send uniform data to opengl
	glUseProgram(debugProgram);
	Matrix3D ShapeModel; 
	T->getShapeModel(&ShapeModel, r->getCenter());
	glUniformMatrix4fv(umodel3, 1, GL_FALSE, &(ShapeModel.m[0][0]));
	glUniform3f(uDebugColor, debugColor[0], debugColor[1], debugColor[2]);

	float vertices[24];
	float width = r->getSize().x;
	float height = r->getSize().y;

	// TR, TL, BR, BL;
	Vector3D vertices1[4];
	Vector3DSet(&vertices1[0],  width / 2.0f,  height / 2.0f, 0);
	Vector3DSet(&vertices1[1], -width / 2.0f,  height / 2.0f, 0);
	Vector3DSet(&vertices1[2],  width / 2.0f, -height / 2.0f, 0);
	Vector3DSet(&vertices1[3], -width / 2.0f, -height / 2.0f, 0);
	vertices[0] = vertices1[1].x;   vertices[1] = vertices1[1].y;   vertices[2] = vertices1[1].z;
	vertices[3] = vertices1[0].x;   vertices[4] = vertices1[0].y;   vertices[5] = vertices1[0].z;
	vertices[6] = vertices1[0].x;   vertices[7] = vertices1[0].y;   vertices[8] = vertices1[0].z;
	vertices[9] = vertices1[2].x;   vertices[10] = vertices1[2].y;  vertices[11] = vertices1[2].z;
	vertices[12] = vertices1[2].x;  vertices[13] = vertices1[2].y;  vertices[14] = vertices1[2].z;
	vertices[15] = vertices1[3].x;  vertices[16] = vertices1[3].y;  vertices[17] = vertices1[3].z;
	vertices[18] = vertices1[3].x;  vertices[19] = vertices1[3].y;  vertices[20] = vertices1[3].z;
	vertices[21] = vertices1[1].x;  vertices[22] = vertices1[1].y;  vertices[23] = vertices1[1].z;

	//USE PROGRAM THAT DRAWS RED LINES AND USES NO MODEL MATRIX
	glLineWidth(2.0f);
	unsigned int tempvbo;
	glGenBuffers(1, &tempvbo);
	glBindBuffer(GL_ARRAY_BUFFER, tempvbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);
	glDrawArrays(GL_LINES, 0, 8);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &tempvbo);
	glUseProgram(0);
	glLineWidth(1.0f);
}


void GraphicsManager::DrawBoundingCircle(CircleShape *c, DEBUGMODE mode)
{
	Component *ownerComp = c->GetShapeOwner();
	GameObject *owner = ownerComp->getOwner();
	Transform *T = static_cast<Transform*>(owner->GetComponent(COMPONENT_TYPE::TRANSFORM));
	if (T == 0)
		return;

	//COLOR OF THE DEBUG LINES
	if (mode == DEBUGMODE::COLLISION)
	{
		debugColor[0] = 0;
		debugColor[1] = 1;
		debugColor[2] = 0;
	}
	else if (mode == DEBUGMODE::TRIGGER)
	{
		debugColor[0] = 0;
		debugColor[1] = 0;
		debugColor[2] = 1;
	}

	//Send uniform data to opengl
	glUseProgram(debugProgram);
	Matrix3D ShapeModel;
	T->getShapeModel(&ShapeModel, c->getCenter());
	glUniformMatrix4fv(umodel3, 1, GL_FALSE, &(ShapeModel.m[0][0]));
	glUniform3f(uDebugColor, debugColor[0], debugColor[1], debugColor[2]);

	float vertices[36];
	float radius = c->getRadius();

	float const pi = 3.14159265f;
	Vector3D vertices1[12];
	Vector3DSet(&vertices1[0],  radius,  0.0f, 0);
	Vector3DSet(&vertices1[1],  radius * cos(pi / 6.0f), radius * sin(pi / 6.0f), 0);
	Vector3DSet(&vertices1[2],  radius * cos(pi / 3.0f), radius * sin(pi / 3.0f), 0);
	Vector3DSet(&vertices1[3],  0.0f, radius, 0);
	Vector3DSet(&vertices1[4],  radius * -sin(pi / 6.0f), radius * cos(pi / 6.0f), 0);
	Vector3DSet(&vertices1[5],  radius * -sin(pi / 3.0f), radius * cos(pi / 3.0f), 0);
	Vector3DSet(&vertices1[6],  -radius, 0.0f, 0);
	Vector3DSet(&vertices1[7],  radius * -cos(pi / 6.0f), radius * -sin(pi / 6.0f), 0);
	Vector3DSet(&vertices1[8],  radius * -cos(pi / 3.0f), radius * -sin(pi / 3.0f), 0);
	Vector3DSet(&vertices1[9],  0.0f, -radius, 0);
	Vector3DSet(&vertices1[10], radius * cos(pi / 3.0f), radius * -sin(pi / 3.0f), 0);
	Vector3DSet(&vertices1[11], radius * cos(pi / 6.0f), radius * -sin(pi / 6.0f), 0);
	for (int i = 0; i < 36; i += 3) 
	{
		vertices[i]   = vertices1[i/3].x;
		vertices[i+1] = vertices1[i/3].y;
		vertices[i+2] = vertices1[i/3].z;
	}

	//USE PROGRAM THAT DRAWS RED LINES AND USES NO MODEL MATRIX
	glLineWidth(2.0f);
	unsigned int tempvbo;
	glGenBuffers(1, &tempvbo);
	glBindBuffer(GL_ARRAY_BUFFER, tempvbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);
	glDrawArrays(GL_LINE_LOOP, 0, 12);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &tempvbo);
	glUseProgram(0);
	glLineWidth(1.0f);
}


bool GraphicsManager::isInDebugMode() 
{
	return debugMode;
}