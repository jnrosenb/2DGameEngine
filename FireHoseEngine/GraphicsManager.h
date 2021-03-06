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
#ifndef GRAPHICSMANAGER_H
#define GRAPHICSMANAGER_H

#include <string>				//CHANGE MAYBE TO FORWARD DECLARATION
#include "GL/glew.h"			//CHANGE MAYBE TO FORWARD DECLARATION
#include "GL/gl.h"				//CHANGE MAYBE TO FORWARD DECLARATION
#include "Math/Matrix3D.h"
#include "Math/Vector3D.h"
#include "Collision/Shapes.h"
#include "Components/RigidBody2D.h"
#include "Components/Renderer.h"
#include <vector>
#include <unordered_map>
#include <string>

//TODO change this and others to struct
struct SDL_Surface;
class ParticleEmitter;

#define NUMTEXTURES 256
#define MAX_INSTANCES 100000


//For distinguishing debug draw mode
enum class DEBUGMODE 
{
	COLLISION,
	TRIGGER,
	NUM
};


class GraphicsManager 
{
public:
	GraphicsManager(void);
	~GraphicsManager(void);
	
	void init(int width, int height);
	///void InstancingInit();
	void draw();
	GLuint generateTextureFromSurface(SDL_Surface *surface, std::string key);
	void AddRendererComponent(Renderer* R);
	void ToggleDebugMode();
	bool isInDebugMode();
	int GetRenderersSize();
	void Unload();
	void Unload(int numOfRenderersToPop);

	GLuint getProgram(int instancing);
	GLuint getVao();


public:
	//TODO: PUBLIC JUST FOR NOW AS A TESTING MEASURE
	GLuint vbo[2]; //0-vertex, 1-uv

	//TEMP public
	GLuint particleSystemTexture;

	//TEMP
	GLuint UIProgram;

	//TODO temporarily public
	int width;
	int height;

	//TODO temporarily public
	std::vector<ParticleEmitter*> particleEmitters;

private:
	void setupShaders(const char *vertexPath, const char *fragmentPath, GLuint program);
	std::string loadFile(const char *path);
	void DrawBoundingBox(RectangleShape *r, DEBUGMODE mode);
	void DrawBoundingCircle(CircleShape *c, DEBUGMODE mode);
	void PrepareParticleSystemData();

private:
	//List of renderers to render
	std::vector<Renderer*> renderers;
	//Map that pairs surface to glTexture
	std::unordered_map<std::string, GLint> texturesDict;

	//Shaders
	GLuint spriteProgram;
	GLuint instancingProgram;
	GLuint debugProgram;

	//Debug drawing
	bool debugMode;
	float debugColor[3];
	GLuint uDebugColor;

	//For now
	GLuint quadVao;
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

	//Textures
	GLuint textures[NUMTEXTURES];
	int currentTexture;
};

#endif