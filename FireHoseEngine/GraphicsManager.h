#ifndef GRAPHICSMANAGER_H
#define GRAPHICSMANAGER_H

#include <string>				//CHANGE MAYBE TO FORWARD DECLARATION
#include "GL/glew.h"			//CHANGE MAYBE TO FORWARD DECLARATION
#include "GL/gl.h"				//CHANGE MAYBE TO FORWARD DECLARATION
#include "SDL2/SDL_surface.h"	//TOTALLY CHANGE TO FORWARD DECLARATION
#include "Math/Matrix3D.h"
#include "Math/Vector3D.h"
#include "Collision/Shapes.h"
#include "Components/RigidBody2D.h"
#include "Components/Renderer.h"
#include <vector>

#define NUMTEXTURES 64
#define MAX_INSTANCES 100000

class GraphicsManager 
{
public:
	/* Start Header -------------------------------------------------------
	*  Copyright (C) 2018 DigiPen Institute of Technology.
	*  Reproduction or disclosure of this file or its contents without the prior
	*  written consent of DigiPen Institute of Technology is prohibited.

	*  File Name: GraphicsManager.h
	*  Purpose: GraphicsManager.cpp header file.
	*  Project: CS529_JRosenbluth_3
	*  Author: Jose Rosenbluth Chiu, J.Rosenbluth, 60003418
	*  Creation date: 10/14/2018
	-  End Header --------------------------------------------------------*/
	
	GraphicsManager(void);
	~GraphicsManager(void);

	void init();
	void InstancingInit();
	void draw();
	GLuint generateTextureFromSurface(SDL_Surface *surface);
	void AddRendererComponent(Renderer* R);
	void ToggleDebugMode();

	GLuint getProgram(int instancing);
	GLuint getVao();

	//Later move to private
	int currModelIndex;
	float modelMatrices[MAX_INSTANCES * 16]; //Each instance has a 4x4 model matrix

private:
	void setupShaders(const char *vertexPath, const char *fragmentPath, GLuint program);
	std::string loadFile(const char *path);
	void DrawBoundingBox(RectangleShape *r);
	void DrawBoundingCircle(CircleShape *c);
	
	bool debugMode;

	//List of renderers to render
	std::vector<Renderer*> renderers;

	//Shaders
	GLuint spriteProgram;
	GLuint instancingProgram;
	GLuint debugProgram;

	//For now
	GLuint quadVao;
	GLuint vbo[2]; //0-vertex, 1-uv
	GLuint ebo;

	//View Proj matrix for second shader
	GLint uproj;
	GLint uview;
	//View Proj matrix for second shader
	GLint uproj2;
	GLint uview2;
	//View Proj matrix for third shader
	GLint uproj3;
	GLint uview3;
	GLint umodel3;

	//Instancing
	GLuint uModelMatrices;
	GLuint textures[NUMTEXTURES];
	int currentTexture;
};

#endif