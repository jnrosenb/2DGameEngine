/* Start Header -------------------------------------------------------
*  Copyright (C) 2018 DigiPen Institute of Technology.
*  Reproduction or disclosure of this file or its contents without the prior
*  written consent of DigiPen Institute of Technology is prohibited.

*  File Name: Actor.cpp
*  Purpose: Actor class. Every object in the game should be an actor. Can be
*  appended different components for multiple behaviours.
*  Project: CS529_JRosenbluth_3
*  Author: Jose Rosenbluth Chiu, J.Rosenbluth, 60003418
*  Creation date: 10/14/2018
-  End Header --------------------------------------------------------*/

#include "Actor.h"
#include <iostream>
#include <cmath>
#include <cstdlib>

using namespace std;

Actor::Actor(float scalex, float scaley, float angle0,
	GLuint prgm, GLuint vao, GLuint tex) :
	pAngle(angle0), texture(tex), program(prgm), quadVao(vao)
{
	pVelocity[0] = 0.f;
	pVelocity[1] = 1.f;
	pPosition[0] = 0.0f;
	pPosition[1] = 0.0f;
	pScale[0] = scalex;
	pScale[1] = scaley;

	memset(M, 0, 16 * sizeof(float));
	M[0] = 1.0f;
	M[5] = 1.0f;
	M[10] = 1.0f;
	M[15] = 1.0f;

	cout << "ACTOR HAS: program: " << program << ", Vao: " << quadVao << ", Texture: " << texture << endl;

	umodel = glGetUniformLocation(program, "model");
	glUseProgram(program);
	glUniformMatrix4fv(umodel, 1, GL_FALSE, M);

	cout << "Actor constructor called" << endl;
}

Actor::~Actor()
{
	cout << "Actor destructor called" << endl;
}

void Actor::update()
{
	//Translation;
	M[12] = pPosition[0];
	M[13] = pPosition[1];

	//Scale
	M[0] = pScale[0];
	M[5] = pScale[1];

	//Rotation
	//TODO
}

void Actor::draw()
{
	//Passs the matrix information
	glUseProgram(program);
	glUniformMatrix4fv(umodel, 1, GL_FALSE, M);

	//Texture information and blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	//Bid VAO
	glBindVertexArray(quadVao);

	//Draw the quad and get errors
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	GLenum err = glGetError();
	if (err != GL_NO_ERROR)
	{
		switch (err)
		{
		case GL_INVALID_ENUM:
			cout << "ERROR: GL_INVALID_ENUM" << endl;
			break;
		case GL_INVALID_VALUE:
			cout << "ERROR: GL_INVALID_VALUE" << endl;
			break;
		case GL_INVALID_OPERATION:
			cout << "ERROR: GL_INVALID_OPERATION" << endl;
			break;
		default:
			cout << "ERROR: None of the previous" << endl;
			break;
		}
	}

	//Unbind
	glBindVertexArray(0);
	glUseProgram(0);
}

void Actor::rotate(float angle)
{
	this->pAngle += angle;
}

void Actor::translate(float dx, float dy)
{
	pPosition[0] += dx;
	pPosition[1] += dy;
}

void Actor::scale(float x, float y)
{
	pScale[0] = x;
	pScale[1] = y;
}