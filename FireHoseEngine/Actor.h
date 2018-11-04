/* Start Header -------------------------------------------------------
*  Copyright (C) 2018 DigiPen Institute of Technology.
*  Reproduction or disclosure of this file or its contents without the prior
*  written consent of DigiPen Institute of Technology is prohibited.

*  File Name: Actor.h
*  Purpose: Actor.cpp header file.
*  Project: CS529_JRosenbluth_3
*  Author: Jose Rosenbluth Chiu, J.Rosenbluth, 60003418
*  Creation date: 10/14/2018
-  End Header --------------------------------------------------------*/

#ifndef ACTOR_H
#define ACTOR_H

#include "GL/glew.h"
#include "GL/gl.h"

class Actor
{
public:
	Actor(float scalex, float scaley, float angle0, 
		GLuint prgm, GLuint vao, GLuint tex);
	~Actor(void);

	//Temporary methods
	void rotate(float angle);
	void translate(float dx, float dy);
	void scale(float x, float y);

	void update();
	void draw();

private:
	float pPosition[2];
	float pScale[2];
	float pAngle; //degree
	float pVelocity[2];

	GLuint program;
	GLuint quadVao;
	GLuint texture;

	float T[16];
	float R[16];
	float H[16];
	float M[16];
	GLint umodel;
};

#endif