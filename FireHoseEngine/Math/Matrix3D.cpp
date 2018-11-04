/* Start Header -------------------------------------------------------
   Copyright (C) 2018 DigiPen Institute of Technology.
   Reproduction or disclosure of this file or its contents without the prior
   written consent of DigiPen Institute of Technology is prohibited.

   File Name:		Matrix3D.c
   Purpose:			Set of function designed to help with the operation of 2 dimentional homogeneous matrices.
   Language:		C programming language, Microsoft Visual C++ compiler
   Platform:		Microsoft Visual C++ 2017, Any machine running windows 10, Windows 10 OS>
   Project:			CS529_JRosenbluth_2
   Author:			Jose Rosenbluth Chiu, J.Rosenbluth, 60003418
   Creation date:	09/17/2018
-  End Header --------------------------------------------------------*/

#include <iostream>
#include <cstring>
#include <cmath>
#include "Matrix3D.h"

#define EPSILON 0.000001f
#define PI 3.1415926535897932384626433832795

/*
This function sets the matrix Result to the identity matrix
*/
void Matrix3DIdentity(Matrix3D *pResult)
{
	if (!pResult) 
	{
		std::cout << "(Matrix2DIdentity)- Error, one or more args are undefined" << std::endl;
	}

	memset((void*)pResult->m[0], 0, sizeof(pResult->m));
	pResult->m[0][0] = 1.0f;
	pResult->m[1][1] = 1.0f;
	pResult->m[2][2] = 1.0f;
	pResult->m[3][3] = 1.0f;
}

// ---------------------------------------------------------------------------

/*
This functions calculated the transpose matrix of Mtx and saves it in Result
*/
void Matrix3DTranspose(Matrix3D *pResult, Matrix3D *pMtx)
{
	if (!pResult || !pMtx)
	{
		std::cout << "(Matrix3DTranspose)- Error, one or more args are undefined" << std::endl;
	}

	Matrix3D temp;
	temp.m[0][0] = pMtx->m[0][0];
	temp.m[0][1] = pMtx->m[1][0];
	temp.m[0][2] = pMtx->m[2][0];
	temp.m[0][3] = pMtx->m[3][0];

	temp.m[1][0] = pMtx->m[0][1];
	temp.m[1][1] = pMtx->m[1][1];
	temp.m[1][2] = pMtx->m[2][1];
	temp.m[1][3] = pMtx->m[3][1];

	temp.m[2][0] = pMtx->m[0][2];
	temp.m[2][1] = pMtx->m[1][2];
	temp.m[2][2] = pMtx->m[2][2];
	temp.m[2][3] = pMtx->m[3][2];

	temp.m[3][0] = pMtx->m[0][3];
	temp.m[3][1] = pMtx->m[1][3];
	temp.m[3][2] = pMtx->m[2][3];
	temp.m[3][3] = pMtx->m[3][3];

	memcpy(pResult, &temp, sizeof(temp));
}

// ---------------------------------------------------------------------------

/*
This function multiplies Mtx0 with Mtx1 and saves the result in Result
Result = Mtx0*Mtx1
*/
void Matrix3DConcat(Matrix3D *pResult, Matrix3D *pMtx0, Matrix3D *pMtx1)
{
	if (!pResult || !pMtx0 || !pMtx1)
	{
		std::cout << "(Matrix3DConcat)- Error, one or more args are undefined" << std::endl;
	}

	Matrix3D temp;

	temp.m[0][0] = pMtx0->m[0][0] * pMtx1->m[0][0] + pMtx0->m[0][1] * pMtx1->m[1][0] + pMtx0->m[0][2] * pMtx1->m[2][0] + pMtx0->m[0][3] * pMtx1->m[3][0];
	temp.m[0][1] = pMtx0->m[0][0] * pMtx1->m[0][1] + pMtx0->m[0][1] * pMtx1->m[1][1] + pMtx0->m[0][2] * pMtx1->m[2][1] + pMtx0->m[0][3] * pMtx1->m[3][1];
	temp.m[0][2] = pMtx0->m[0][0] * pMtx1->m[0][2] + pMtx0->m[0][1] * pMtx1->m[1][2] + pMtx0->m[0][2] * pMtx1->m[2][2] + pMtx0->m[0][3] * pMtx1->m[3][2];
	temp.m[0][3] = pMtx0->m[0][0] * pMtx1->m[0][3] + pMtx0->m[0][1] * pMtx1->m[1][3] + pMtx0->m[0][2] * pMtx1->m[2][3] + pMtx0->m[0][3] * pMtx1->m[3][3];

	temp.m[1][0] = pMtx0->m[1][0] * pMtx1->m[0][0] + pMtx0->m[1][1] * pMtx1->m[1][0] + pMtx0->m[1][2] * pMtx1->m[2][0] + pMtx0->m[1][3] * pMtx1->m[3][0];
	temp.m[1][1] = pMtx0->m[1][0] * pMtx1->m[0][1] + pMtx0->m[1][1] * pMtx1->m[1][1] + pMtx0->m[1][2] * pMtx1->m[2][1] + pMtx0->m[1][3] * pMtx1->m[3][1];
	temp.m[1][2] = pMtx0->m[1][0] * pMtx1->m[0][2] + pMtx0->m[1][1] * pMtx1->m[1][2] + pMtx0->m[1][2] * pMtx1->m[2][2] + pMtx0->m[1][3] * pMtx1->m[3][2];
	temp.m[1][3] = pMtx0->m[1][0] * pMtx1->m[0][3] + pMtx0->m[1][1] * pMtx1->m[1][3] + pMtx0->m[1][2] * pMtx1->m[2][3] + pMtx0->m[1][3] * pMtx1->m[3][3];
	
	temp.m[2][0] = pMtx0->m[2][0] * pMtx1->m[0][0] + pMtx0->m[2][1] * pMtx1->m[1][0] + pMtx0->m[2][2] * pMtx1->m[2][0] + pMtx0->m[2][3] * pMtx1->m[3][0];
	temp.m[2][1] = pMtx0->m[2][0] * pMtx1->m[0][1] + pMtx0->m[2][1] * pMtx1->m[1][1] + pMtx0->m[2][2] * pMtx1->m[2][1] + pMtx0->m[2][3] * pMtx1->m[3][1];
	temp.m[2][2] = pMtx0->m[2][0] * pMtx1->m[0][2] + pMtx0->m[2][1] * pMtx1->m[1][2] + pMtx0->m[2][2] * pMtx1->m[2][2] + pMtx0->m[2][3] * pMtx1->m[3][2];
	temp.m[2][3] = pMtx0->m[2][0] * pMtx1->m[0][3] + pMtx0->m[2][1] * pMtx1->m[1][3] + pMtx0->m[2][2] * pMtx1->m[2][3] + pMtx0->m[2][3] * pMtx1->m[3][3];
	
	temp.m[3][0] = pMtx0->m[3][0] * pMtx1->m[0][0] + pMtx0->m[3][1] * pMtx1->m[1][0] + pMtx0->m[3][2] * pMtx1->m[2][0] + pMtx0->m[3][3] * pMtx1->m[3][0];
	temp.m[3][1] = pMtx0->m[3][0] * pMtx1->m[0][1] + pMtx0->m[3][1] * pMtx1->m[1][1] + pMtx0->m[3][2] * pMtx1->m[2][1] + pMtx0->m[3][3] * pMtx1->m[3][1];
	temp.m[3][2] = pMtx0->m[3][0] * pMtx1->m[0][2] + pMtx0->m[3][1] * pMtx1->m[1][2] + pMtx0->m[3][2] * pMtx1->m[2][2] + pMtx0->m[3][3] * pMtx1->m[3][2];
	temp.m[3][3] = pMtx0->m[3][0] * pMtx1->m[0][3] + pMtx0->m[3][1] * pMtx1->m[1][3] + pMtx0->m[3][2] * pMtx1->m[2][3] + pMtx0->m[3][3] * pMtx1->m[3][3];

	memcpy(pResult, &temp, sizeof(temp));
}

// ---------------------------------------------------------------------------

/*
This function creates a translation matrix from x & y and saves it in Result
*/
void Matrix3DTranslate(Matrix3D *pResult, float x, float y, float z)
{
	if (!pResult)
	{
		std::cout << "(Matrix3DTranslate)- Error, one or more args are undefined" << std::endl;
	}

	//TODO ask if it has to be a pure translation matrix
	Matrix3DIdentity(pResult);

	pResult->m[0][3] = x;
	pResult->m[1][3] = y;
	pResult->m[2][3] = z;
}

// ---------------------------------------------------------------------------

/*
This function creates a scaling matrix from x & y and saves it in Result
*/
void Matrix3DScale(Matrix3D *pResult, float x, float y, float z)
{
	if (!pResult)
	{
		std::cout << "(Matrix3DScale)- Error, one or more args are undefined" << std::endl;
	}

	//TODO ask if it has to be a pure translation matrix
	Matrix3DIdentity(pResult);

	pResult->m[0][0] = x;
	pResult->m[1][1] = y;
	pResult->m[2][2] = z;
}

// ---------------------------------------------------------------------------

/*
This matrix creates a rotation matrix from "Angle" whose value is in degree.
Save the resultant matrix in Result
*/
void Matrix3DRotAxisDeg(Matrix3D *pResult, Vector3D *axis, float Angle)
{
	float rad = (float)((Angle / 180) * PI);
	Matrix3DRotAxisRad(pResult, axis, rad);
}

// ---------------------------------------------------------------------------

/*
This matrix creates a rotation matrix from "Angle" whose value is in radian.
Save the resultant matrix in Result
*/
void Matrix3DRotAxisRad(Matrix3D *pResult, Vector3D *v, float Angle)
{
	if (!pResult || !v)
	{
		std::cout << "(Matrix3DRotAxisRad)- Error, one or more args are undefined" << std::endl;
	}

	//TODO ask if it has to be a pure translation matrix
	Matrix3DIdentity(pResult);

	//check if angle is zero or if axis is zero vector
	if (fabs(Angle) <= EPSILON || (fabs(v->x) <= EPSILON && fabs(v->y) <= EPSILON && fabs(v->z) <= EPSILON))
	{
		//std::cout << "(Matrix3DRotAxisRad)- Angle is zero or axis is zero vector!\n" << std::endl;
		return;
	}

	//Calculations to derive the rotation matrix
	Matrix3D I;
	Matrix3DIdentity(&I);

	Matrix3D VVT;
	Matrix3DIdentity(&VVT);
	VVT.m[0][0] = v->x*v->x;
	VVT.m[0][1] = v->x*v->y;
	VVT.m[0][2] = v->x*v->z;
	VVT.m[1][0] = v->y*v->x;
	VVT.m[1][1] = v->y*v->y;
	VVT.m[1][2] = v->y*v->z;
	VVT.m[2][0] = v->z*v->x;
	VVT.m[2][1] = v->z*v->y;
	VVT.m[2][2] = v->z*v->z;

	Matrix3D CrossProductM;
	Matrix3DIdentity(&CrossProductM);
	CrossProductM.m[0][0] = 0.0f;
	CrossProductM.m[0][1] = -v->z;
	CrossProductM.m[0][2] =  v->y;
	CrossProductM.m[1][0] =  v->z;
	CrossProductM.m[1][1] = 0.0f;
	CrossProductM.m[1][2] = -v->x;
	CrossProductM.m[2][0] = -v->y;
	CrossProductM.m[2][1] =  v->x;
	CrossProductM.m[2][2] = 0.0f;

	Matrix3D R1;
	Matrix3DIdentity(&R1);
	Matrix3DScalarMult(&R1, &R1, cos(Angle));
	Matrix3D R2;
	Matrix3DIdentity(&R2);
	float scalar = ((1 - cos(Angle)) / (v->x*v->x + v->y*v->y + v->z*v->z));
	Matrix3DScalarMult(&R2, &VVT, scalar);
	Matrix3D R3;
	Matrix3DIdentity(&R3);
	scalar = sin(Angle) / (sqrt(v->x*v->x + v->y*v->y + v->z*v->z));
	Matrix3DScalarMult(&R3, &CrossProductM, scalar);

	Matrix3D R;
	Matrix3DAdd(&R, &R1, &R2);
	Matrix3DAdd(&R, &R, &R3);

	memcpy(pResult, &R, sizeof(R));
}

// ---------------------------------------------------------------------------

/*
This function multiplies the matrix Mtx with the vector Vec and saves the result in Result
Result = Mtx * Vec
*/
void Matrix3DMultVec(Vector3D *pResult, Matrix3D *pMtx, Vector3D *pVec)
{
	if (!pMtx || !pResult || !pVec)
	{
		std::cout << "(Matrix3DMultVec)- Error, one or more args are undefined" << std::endl;
	}

	Vector3D temp;
	temp.x = pMtx->m[0][0] * pVec->x + pMtx->m[0][1] * pVec->y + pMtx->m[0][2] * pVec->z + pMtx->m[0][3]; //TEMP-FIX
	temp.y = pMtx->m[1][0] * pVec->x + pMtx->m[1][1] * pVec->y + pMtx->m[1][2] * pVec->z + pMtx->m[1][3]; //TEMP-FIX
	temp.z = pMtx->m[2][0] * pVec->x + pMtx->m[2][1] * pVec->y + pMtx->m[2][2] * pVec->z + pMtx->m[2][3]; //TEMP-FIX

	pResult->x = temp.x;
	pResult->y = temp.y;
	pResult->z = temp.z;
}

// ---------------------------------------------------------------------------

/*
This multiplies the whole matrix by a scalar
*/
void Matrix3DScalarMult(Matrix3D *pResult, Matrix3D *pMtx0, float scalar)
{
	if (!pResult || !pMtx0)
	{
		std::cout << "(Matrix3DScalarMult)- Error, one or more args are undefined" << std::endl;
	}

	Matrix3D temp;
	Matrix3DIdentity(&temp);

	temp.m[0][0] = pMtx0->m[0][0] * scalar;
	temp.m[0][1] = pMtx0->m[0][1] * scalar;
	temp.m[0][2] = pMtx0->m[0][2] * scalar;

	temp.m[1][0] = pMtx0->m[1][0] * scalar;
	temp.m[1][1] = pMtx0->m[1][1] * scalar;
	temp.m[1][2] = pMtx0->m[1][2] * scalar;
	
	temp.m[2][0] = pMtx0->m[2][0] * scalar;
	temp.m[2][1] = pMtx0->m[2][1] * scalar;
	temp.m[2][2] = pMtx0->m[2][2] * scalar;

	memcpy(pResult, &temp, sizeof(temp));
}

//-----------------------------------------------------------------------------

/*
This adds two matrices and modifies the return function
*/
void Matrix3DAdd(Matrix3D *pResult, Matrix3D *pMtx0, Matrix3D *pMtx1)
{
	if (!pResult || !pMtx0 || !pMtx1)
	{
		std::cout << "(Matrix3DAdd)- Error, one or more args are undefined" << std::endl;
	}

	Matrix3D temp;
	Matrix3DIdentity(&temp);

	temp.m[0][0] = pMtx0->m[0][0] + pMtx1->m[0][0];
	temp.m[0][1] = pMtx0->m[0][1] + pMtx1->m[0][1];
	temp.m[0][2] = pMtx0->m[0][2] + pMtx1->m[0][2];

	temp.m[1][0] = pMtx0->m[1][0] + pMtx1->m[1][0];
	temp.m[1][1] = pMtx0->m[1][1] + pMtx1->m[1][1];
	temp.m[1][2] = pMtx0->m[1][2] + pMtx1->m[1][2];
	
	temp.m[2][0] = pMtx0->m[2][0] + pMtx1->m[2][0];
	temp.m[2][1] = pMtx0->m[2][1] + pMtx1->m[2][1];
	temp.m[2][2] = pMtx0->m[2][2] + pMtx1->m[2][2];

	memcpy(pResult, &temp, sizeof(temp));
}

/*
Prints Vector
*/
void Matrix3DPrint(Matrix3D *mat, char const *name) 
{
	if (!mat)
	{
		std::cout << "(Matrix3DPrint)- one or more params are undefined." << std::endl;
		return;
	}

	std::cout << name << std::endl;
	std::cout << " | " << mat->m[0][0] << ", " << mat->m[0][1] << ", " << mat->m[0][2] << ", " << mat->m[0][3] << " |" << std::endl;
	std::cout << " | " << mat->m[1][0] << ", " << mat->m[1][1] << ", " << mat->m[1][2] << ", " << mat->m[1][3] << " |" << std::endl;
	std::cout << " | " << mat->m[2][0] << ", " << mat->m[2][1] << ", " << mat->m[2][2] << ", " << mat->m[2][3] << " |" << std::endl;
	std::cout << " | " << mat->m[3][0] << ", " << mat->m[3][1] << ", " << mat->m[3][2] << ", " << mat->m[3][3] << " |" << std::endl;
}
