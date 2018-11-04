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

#include "GraphicsManager.h"
#include "Managers.h"
#include "Components/Renderer.h"
#include "Components/Transform.h"
#include "Components/Camera.h"
#include "GameObject.h"
#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;

extern Manager *pManager; /*EXPERIMENT*/

GraphicsManager::GraphicsManager() : 
	currModelIndex(0), currentTexture(0), debugMode(false)
{
	cout << "Graphics manager constructor." << endl;
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
	glDeleteBuffers(1, &uModelMatrices);

	glDeleteTextures(NUMTEXTURES, textures);

	cout << "Graphics manager destructor." << endl;
}

void GraphicsManager::draw()
{
	Camera *C = pManager->GetCameraManager()->GetMainCamera();
	if (C)
	{
		//JUST IN DEBUG MODE
		if (debugMode) 
		{
			glUseProgram(debugProgram);
			glUniformMatrix4fv(uview3, 1, GL_FALSE, &(C->GetViewMatrix().m[0][0]));
			glUniformMatrix4fv(uproj3, 1, GL_FALSE, &(C->GetProjection().m[0][0]));
		}

		glUseProgram(spriteProgram);
		glUniformMatrix4fv(uview, 1, GL_FALSE, &(C->GetViewMatrix().m[0][0]));
		glUniformMatrix4fv(uproj, 1, GL_FALSE, &(C->GetProjection().m[0][0]));

		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glClearDepth(1);
		glClear(GL_DEPTH_BUFFER_BIT);

		for (Renderer* R : renderers) 
		{
			//DRAW THE OBJECT
			R->Draw();
			
			//DRAW BOUNDING BOX OR CIRCLE (JUST IN DEBUG MODE)
			if (debugMode)
			{
				RigidBody2D *rgbdy = static_cast<RigidBody2D*>(R->getOwner()->GetComponent(COMPONENT_TYPE::RIGIDBODY2D));
				if (rgbdy)
				{
					Shape *shape = rgbdy->GetShape();
					if (shape->GetType() == ShapeType::RECTANGLE)
					{
						RectangleShape *r = static_cast<RectangleShape*>(shape);
						DrawBoundingBox(r);
					}
				}
			}
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


void GraphicsManager::init()
{
	cout << "Started initializing the graphics manager!" << endl;

	//Create one program per shader
	spriteProgram = glCreateProgram();
	instancingProgram = glCreateProgram();
	debugProgram = glCreateProgram();
	setupShaders("sprite.vert", "sprite.frag", spriteProgram);
	setupShaders("sprite.vert", "sprite.frag", instancingProgram);
	setupShaders("debug.vert", "debug.frag", debugProgram);

	uview = glGetUniformLocation(spriteProgram, "view");
	uproj = glGetUniformLocation(spriteProgram, "proj");
	
	uview2 = glGetUniformLocation(spriteProgram, "view");
	uproj2 = glGetUniformLocation(spriteProgram, "proj");
	
	uview3  = glGetUniformLocation(debugProgram, "view");
	uproj3  = glGetUniformLocation(debugProgram, "proj");
	umodel3 = glGetUniformLocation(debugProgram, "model");

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
	glGenBuffers(1, &uModelMatrices); //Instancing

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

	cout << "Finished initializing the graphics manager!" << endl;
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


GLuint GraphicsManager::generateTextureFromSurface(SDL_Surface *surface)
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

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	++currentTexture;

	return textures[currentTexture - 1];
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


void GraphicsManager::InstancingInit() 
{
	currModelIndex = 0;
	unsigned int sizeVec4 = 4 * sizeof(float);

	glBindVertexArray(quadVao);

	glBindBuffer(GL_ARRAY_BUFFER, uModelMatrices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(modelMatrices), &modelMatrices[0], GL_STATIC_DRAW);

	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 4 * sizeVec4, (void*)(0 * sizeVec4));
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * sizeVec4, (void*)(1 * sizeVec4));
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * sizeVec4, (void*)(2 * sizeVec4));
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * sizeVec4, (void*)(3 * sizeVec4));

	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);
	glEnableVertexAttribArray(5);

	glVertexAttribDivisor(2, 1);
	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);

	glBindVertexArray(0);
}

////////////////////////////////////////////////////////////////////////
/////////   DEBUGGING BOUNDING SHAPES   ////////////////////////////////
////////////////////////////////////////////////////////////////////////
void GraphicsManager::DrawBoundingBox(RectangleShape *r)
{
	RigidBody2D *rgbdy = r->GetShapeOwner();
	GameObject *owner = rgbdy->getOwner();
	Transform *T = static_cast<Transform*>(owner->GetComponent(COMPONENT_TYPE::TRANSFORM));
	if (T == 0)
		return;

	//Send uniform data to opengl
	glUseProgram(debugProgram);
	Matrix3D UnscaledModel; 
	T->getUnscaledModel(&UnscaledModel);
	glUniformMatrix4fv(umodel3, 1, GL_FALSE, &(UnscaledModel.m[0][0]));
	//glUniformMatrix4fv(umodel3, 1, GL_FALSE, &(Model.m[0][0]));

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

void GraphicsManager::DrawBoundingCircle(CircleShape *c)
{
	RigidBody2D *rgbdy = c->GetShapeOwner();
	if (rgbdy == 0)
		return;

	GameObject *owner = rgbdy->getOwner();
	Transform *T = static_cast<Transform*>(owner->GetComponent(COMPONENT_TYPE::TRANSFORM));
	if (T == 0)
		return;

	//TODO: Open gl draw circle function
}