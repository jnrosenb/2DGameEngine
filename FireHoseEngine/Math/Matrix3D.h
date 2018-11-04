/* Start Header -------------------------------------------------------
   Copyright (C) 2018 DigiPen Institute of Technology.
   Reproduction or disclosure of this file or its contents without the prior
   written consent of DigiPen Institute of Technology is prohibited.

   File Name:		Matrix3D.h
   Purpose:			Header file for Matrix3D.c.
   Language:		C programming language, Microsoft Visual C++ compiler
   Platform:		Microsoft Visual C++ 2017, Any machine running windows 10, Windows 10 OS>
   Project:			CS529_JRosenbluth_2
   Author:			Jose Rosenbluth Chiu, J.Rosenbluth, 60003418
   Creation date:	09/17/2018
-  End Header --------------------------------------------------------*/

#ifndef MATRIX3D_H
#define MATRIX3D_H


#include "Vector3D.h"

typedef struct Matrix3D
{
	float m[4][4];
}Matrix3D;


/*
This function sets the matrix Result to the identity matrix
*/
void Matrix3DIdentity(Matrix3D *pResult);

/*
This functions calculated the transpose matrix of Mtx and saves it in Result
*/
void Matrix3DTranspose(Matrix3D *pResult, Matrix3D *pMtx);

/*
This function multiplies Mtx0 with Mtx1 and saves the result in Result
Result = Mtx0*Mtx1
*/
void Matrix3DConcat(Matrix3D *pResult, Matrix3D *pMtx0, Matrix3D *pMtx1);

/*
This function creates a translation matrix from x & y and saves it in Result
*/
void Matrix3DTranslate(Matrix3D *pResult, float x, float y, float z);

/*
This function creates a scaling matrix from x & y and saves it in Result
*/
void Matrix3DScale(Matrix3D *pResult, float x, float y, float z);

/*
This matrix creates a rotation matrix from "Angle" whose value is in degree.
Save the resultant matrix in Result
*/
void Matrix3DRotAxisDeg(Matrix3D *pResult, Vector3D *v, float angle);

/*
This matrix creates a rotation matrix from "Angle" whose value is in radian.
Save the resultant matrix in Result
*/
void Matrix3DRotAxisRad(Matrix3D *pResult, Vector3D *axis, float radian);

/*
This function multiplies the matrix Mtx with the vector Vec and saves the result in Result
Result = Mtx * Vec
*/
void Matrix3DMultVec(Vector3D *pResult, Matrix3D *pMtx, Vector3D *pVec);

/*
This multiplies the non-homogeneous part of the matrix by a scalar
*/
void Matrix3DScalarMult(Matrix3D *pResult, Matrix3D *pMtx0, float scalar);

/*
This adds two non-homogeneous part of two matrices and modifies the return function
*/
void Matrix3DAdd(Matrix3D *pResult, Matrix3D *R1, Matrix3D *R2);

/*
Prints Matrix
*/
void Matrix3DPrint(Matrix3D *mat, const char *name);

#endif