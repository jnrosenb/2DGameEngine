/* Start Header -------------------------------------------------------
   Copyright (C) 2018 DigiPen Institute of Technology.
   Reproduction or disclosure of this file or its contents without the prior
   written consent of DigiPen Institute of Technology is prohibited.

   File Name:		Vector3D.h
   Purpose:			Header file for Vector3D.c
   Language:		C programming language, Microsoft Visual C++ compiler
   Platform:		Microsoft Visual C++ 2017, Any machine running windows 10, Windows 10 OS>
   Project:			CS529_JRosenbluth_2
   Author:			Jose Rosenbluth Chiu, J.Rosenbluth, 60003418
   Creation date:	09/17/2018
-  End Header --------------------------------------------------------*/

#ifndef VECTOR3D_H
#define VECTOR3D_H

#include "math.h"


typedef struct Vector3D
{
	float x, y, z;
}Vector3D;

/*
This function sets the coordinates of the 2D vector (pResult) to 0
*/
void Vector3DZero(Vector3D *pResult);

/*
This function sets the coordinates of the 2D vector (pResult) to x &y
*/
void Vector3DSet(Vector3D *pResult, float x, float y, float z);

/*
In this function, pResult will be set to the opposite of pVec0
*/
void Vector3DNeg(Vector3D *pResult, Vector3D *pVec0);

/*
In this function, pResult will be the sum of pVec0 and pVec1
*/
void Vector3DAdd(Vector3D *pResult, Vector3D *pVec0, Vector3D *pVec1);

/*
In this function, pResult will be the difference between pVec0 *pVec1: pVec0 - pVec1
*/
void Vector3DSub(Vector3D *pResult, Vector3D *pVec0, Vector3D *pVec1);

/*
In this function, pResult will be the unit vector of pVec0
*/
//void Vector2DNormalize(Vector3D *pResult, Vector3D *pVec0);

void Vector3DNormalize(Vector3D *pResult, Vector3D *pVec0);

/*
In this function, pResult will be the vector pVec0 scaled by the value c
*/
void Vector3DScale(Vector3D *pResult, Vector3D *pVec0, float c);

/*
In this function, pResult will be the vector pVec0 scaled by c and added to pVec1 
*/
void Vector3DScaleAdd(Vector3D *pResult, Vector3D *pVec0, Vector3D *pVec1, float c);

/*
In this function, pResult will be the vector pVec0 scaled by c and pVec1 will be subtracted from it 
*/
void Vector3DScaleSub(Vector3D *pResult, Vector3D *pVec0, Vector3D *pVec1, float c);

/*
In this function, pResult will be the vector pVec0 minus pVec1 scaled by c
*/
void Vector3DSubScale(Vector3D *pResult, Vector3D *pVec0, Vector3D *pVec1, float c);

/*
This function returns the length of the vector pVec0
*/
float Vector3DLength(Vector3D *pVec0);

/*
This function returns the square of pVec0's length. Avoid the square root
*/
float Vector3DSquareLength(Vector3D *pVec0);

/*
In this function, pVec0 and pVec1 are considered as 2D points.
The distance between these 2 2D points is returned
*/
float Vector3DDistance(Vector3D *pVec0, Vector3D *pVec1);

/*
In this function, pVec0 and pVec1 are considered as 2D points.
The squared distance between these 2 2D points is returned. Avoid the square root
*/
float Vector3DSquareDistance(Vector3D *pVec0, Vector3D *pVec1);

float Vector3DSquareDistance2D(Vector3D *pVec0, Vector3D *pVec1);

/*
This function returns the dot product between pVec0 and pVec1
*/
//float Vector2DDotProduct(Vector3D *pVec0, Vector3D *pVec1);

float Vector3DDotProduct(Vector3D *pVec0, Vector3D *pVec1);

/*
This function computes the coordinates of the vector represented by the angle "angle", which is in Degrees
Useful for getting direction vectors given an angle
*/
void Vector3DFromAngle2DDeg(Vector3D *pResult, float angle);

/*
This function computes the coordinates of the vector represented by the angle "angle", which is in Radian
*/
void Vector3DFromAngle2DRad(Vector3D *pResult, float angle);

/*
This function takes a vector and extracts the angle it forms withx axis
*/
float Vector3DAngle2DFromVec(Vector3D *vec);

/*
Prints Vector
*/
void Vector3DPrint(Vector3D const *vec);

#endif