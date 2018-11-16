/* Start Header -------------------------------------------------------
   Copyright (C) 2018 DigiPen Institute of Technology.
   Reproduction or disclosure of this file or its contents without the prior
   written consent of DigiPen Institute of Technology is prohibited.

   File Name:		Vector3D.c
   Purpose:			Manages all operations regarding vectors in 2 dimensions.
   Language:		C programming language, Microsoft Visual C++ compiler
   Platform:		Microsoft Visual C++ 2017, Any machine running windows 10, Windows 10 OS>
   Project:			CS529_JRosenbluth_2
   Author:			Jose Rosenbluth Chiu, J.Rosenbluth, 60003418
   Creation date:	09/17/2018
-  End Header --------------------------------------------------------*/

#include <iostream>
#include "Vector3D.h"
#include <cmath>

#define EPSILON 0.000001f
#define PI 3.1415926535897932384626433832795

// ---------------------------------------------------------------------------

void Vector3DZero(Vector3D *pResult)
{
	if (!pResult)
	{
		std::cout << "(Vector3DZero)- pResult is undefined." << std::endl;
		return;
	}

	pResult->x = 0;
	pResult->y = 0;
	pResult->z = 0;
}

// ---------------------------------------------------------------------------

void Vector3DSet(Vector3D *pResult, float x, float y, float z)
{
	if (!pResult)
	{
		std::cout << "(Vector3DSet)- pResult is undefined." << std::endl;
		return;
	}

	pResult->x = x;
	pResult->y = y;
	pResult->z = z;
}

// ---------------------------------------------------------------------------

void Vector3DNeg(Vector3D *pResult, Vector3D *pVec0)
{
	if (!pResult || !pVec0)
	{
		std::cout << "(Vector3DNeg)- one or more params are undefined." << std::endl;
		return;
	}

	pResult->x = -pVec0->x;
	pResult->y = -pVec0->y;
	pResult->z = -pVec0->z;
}

// ---------------------------------------------------------------------------

void Vector3DAdd(Vector3D *pResult, Vector3D *pVec0, Vector3D *pVec1)
{
	if (!pResult || !pVec0 || !pVec1)
	{
		std::cout << "(Vector3DAdd)- one or more params are undefined." << std::endl;
		return;
	}

	pResult->x = pVec0->x + pVec1->x;
	pResult->y = pVec0->y + pVec1->y;
	pResult->z = pVec0->z + pVec1->z;
}

// ---------------------------------------------------------------------------

void Vector3DSub(Vector3D *pResult, Vector3D *pVec0, Vector3D *pVec1)
{
	if (!pResult || !pVec0 || !pVec1)
	{
		std::cout << "(Vector3DSub)- one or more params are undefined." << std::endl;
		return;
	}

	pResult->x = pVec0->x - pVec1->x;
	pResult->y = pVec0->y - pVec1->y;
	pResult->z = pVec0->z - pVec1->z;
}

// ---------------------------------------------------------------------------

void Vector3DNormalize(Vector3D *pResult, Vector3D *pVec0)
{
	if (!pResult || !pVec0)
	{
		std::cout << "(Vector3DNormalize)- one or more params are undefined." << std::endl;
		return;
	}

	float length = Vector3DLength(pVec0);
	if (fabs(length) - EPSILON > 0) 
	{
		pResult->x = pVec0->x / length;
		pResult->y = pVec0->y / length;
		pResult->z = pVec0->z / length;
	}
	else 
	{
		pResult->x = 0.0f;
		pResult->y = 0.0f;
		pResult->z = 0.0f;
	}
}

// ---------------------------------------------------------------------------

void Vector3DScale(Vector3D *pResult, Vector3D *pVec0, float c)
{
	if (!pResult || !pVec0)
	{
		std::cout << "(Vector3DScale)- one or more params are undefined." << std::endl;
		return;
	}

	Vector3DSet(pResult, pVec0->x * c, pVec0->y * c, pVec0->z * c);
}

// ---------------------------------------------------------------------------

void Vector3DScaleAdd(Vector3D *pResult, Vector3D *pVec0, Vector3D *pVec1, float c)
{
	if (!pResult || !pVec0 || !pVec1)
	{
		std::cout << "(Vector3DScaleAdd)- one or more params are undefined." << std::endl;
		return;
	}

	Vector3D temp0;
	temp0.x = pVec0->x * c;
	temp0.y = pVec0->y * c;
	temp0.z = pVec0->z * c;

	Vector3DAdd(pResult, &temp0, pVec1);
}

// ---------------------------------------------------------------------------

void Vector3DScaleSub(Vector3D *pResult, Vector3D *pVec0, Vector3D *pVec1, float c)
{
	if (!pResult || !pVec0 || !pVec1)
	{
		std::cout << "(Vector3DScaleSub)- one or more params are undefined." << std::endl;
		return;
	}

	Vector3D temp0;
	temp0.x = pVec0->x * c;
	temp0.y = pVec0->y * c;
	temp0.z = pVec0->z * c;

	Vector3DSub(pResult, &temp0, pVec1);
}

//----------------------------------------------------------------------------

void Vector3DSubScale(Vector3D *pResult, Vector3D *pVec0, Vector3D *pVec1, float c) 
{
	if (!pResult || !pVec0 || !pVec1)
	{
		std::cout << "(Vector3DSubScaled)- one or more params are undefined." << std::endl;
		return;
	}

	Vector3D temp0;
	temp0.x = pVec1->x * c;
	temp0.y = pVec1->y * c;
	temp0.z = pVec1->z * c;

	Vector3DSub(pResult, pVec0, &temp0);
}

// ---------------------------------------------------------------------------

float Vector3DLength(Vector3D *pVec0)
{
	if (!pVec0)
	{
		std::cout << "(Vector3DLength)- one or more params are undefined." << std::endl;
		return -1;
	}

	return (float)sqrt(pVec0->x*pVec0->x + pVec0->y*pVec0->y + pVec0->z*pVec0->z);
}

// ---------------------------------------------------------------------------

float Vector3DSquareLength(Vector3D *pVec0)
{
	if (!pVec0)
	{
		std::cout << "(Vector3DSquareLength)- one or more params are undefined." << std::endl;
		return -1;
	}

	return pVec0->x*pVec0->x + pVec0->y*pVec0->y + pVec0->z*pVec0->z;
}

// ---------------------------------------------------------------------------

float Vector3DDistance(Vector3D *pVec0, Vector3D *pVec1)
{
	if (!pVec0 || !pVec1)
	{
		std::cout << "(Vector3DDistance)- one or more params are undefined." << std::endl;
		return -1;
	}

	return (float)sqrt((pVec0->x - pVec1->x)*(pVec0->x - pVec1->x) + (pVec0->y - pVec1->y)*(pVec0->y - pVec1->y) + (pVec0->z - pVec1->z)*(pVec0->z - pVec1->z));
}

// ---------------------------------------------------------------------------

float Vector3DSquareDistance2D(Vector3D *pVec0, Vector3D *pVec1) 
{
	if (!pVec0 || !pVec1)
	{
		std::cout << "(Vector3DSquareDistance2D)- one or more params are undefined." << std::endl;
		return -1;
	}

	return (pVec0->x - pVec1->x)*(pVec0->x - pVec1->x) + (pVec0->y - pVec1->y)*(pVec0->y - pVec1->y);
}

//-----------------------------------------------------------------------------

float Vector3DSquareDistance(Vector3D *pVec0, Vector3D *pVec1)
{
	if (!pVec0 || !pVec1)
	{
		std::cout << "(Vector3DSquareDistance)- one or more params are undefined." << std::endl;
		return -1;
	}

	return (pVec0->x - pVec1->x)*(pVec0->x - pVec1->x) + (pVec0->y - pVec1->y)*(pVec0->y - pVec1->y) + (pVec0->z - pVec1->z)*(pVec0->z - pVec1->z);
}

// ---------------------------------------------------------------------------

float Vector3DDotProduct(Vector3D *pVec0, Vector3D *pVec1)
{
	if (!pVec0 || !pVec1)
	{
		std::cout << "(Vector3DDotProduct)- one or more params are undefined." << std::endl;
		return -1;
	}

	return pVec0->x * pVec1->x + pVec0->y * pVec1->y + pVec0->z * pVec1->z;
}

// ---------------------------------------------------------------------------

void Vector3DFromAngle2DDeg(Vector3D *pResult, float angle)
{
	float rad = (float)(angle * PI) / 180;
	Vector3DFromAngle2DRad(pResult, rad);
}

// ---------------------------------------------------------------------------

void Vector3DFromAngle2DRad(Vector3D *pResult, float angle)
{
	if (!pResult)
	{
		std::cout << "(Vector3DFromAngle2DRad)- one or more params are undefined." << std::endl;
		return;
	}

	pResult->x = (float)cos(angle);
	pResult->y = (float)sin(angle);
	pResult->z = 0.0f;
}

// ---------------------------------------------------------------------------

float Vector3DAngle2DFromVec(Vector3D *vec) 
{
	if (!vec)
	{
		std::cout << "(Vector3DAngle2DFromVec3)- one or more params are undefined." << std::endl;
		return 0;
	}

	return (float)atan2(vec->y, vec->x);
}

// ---------------------------------------------------------------------------

void Vector3DLerp(Vector3D *res, Vector3D *origin, Vector3D *destination, float alpha)
{
	if (!res || !origin || !destination)
	{
		std::cout << "(Vector3DLerp)- one or more params are undefined." << std::endl;
		return;
	}

	//TODO***
	// 1- Check what happens if you give the same vector as origin and result, or smoething
	// 2- Check if its necessary to do this for 2D also

	float distanceSqr = Vector3DSquareDistance(origin, destination);
	if (distanceSqr < 0.1f) 
	{
		Vector3DSet(res, destination->x, destination->y, destination->z);
		return;
	}
	float t = (alpha * distanceSqr) / distanceSqr;

	Vector3D dir;
	Vector3DSub(&dir, destination, origin);
	Vector3DNormalize(&dir, &dir);
	Vector3DScaleAdd(res, &dir, origin, t);
}

// ---------------------------------------------------------------------------

void Vector3DLerp2D(Vector3D *res, Vector3D *origin, Vector3D *destination, float alpha)
{
	if (!res || !origin || !destination)
	{
		std::cout << "(Vector3DLerp)- one or more params are undefined." << std::endl;
		return;
	}

	//TODO***
	// 1- Check what happens if you give the same vector as origin and result, or smoething
	// 2- Check if its necessary to do this for 2D also

	float distanceSqr = Vector3DSquareDistance2D(origin, destination);
	float t = (alpha * distanceSqr) / distanceSqr;

	Vector3D dir;
	Vector3DSub(&dir, destination, origin);
	Vector3DSet(&dir, dir.x, dir.y, 0);
	Vector3DNormalize(&dir, &dir);
	Vector3DScaleAdd(res, &dir, origin, t);
}

// ---------------------------------------------------------------------------

void Vector3DPrint(Vector3D const *vec)
{
	if (!vec)
	{
		std::cout << "(Vector3DAngle2DFromVec3)- one or more params are undefined." << std::endl;
		return;
	}

	std::cout << " ( " << vec->x << ", " << vec->y << ", " << vec->z << " )" << std::endl;
}