/* Start Header -------------------------------------------------------
   Copyright (C) 2018 DigiPen Institute of Technology.
   Reproduction or disclosure of this file or its contents without the prior
   written consent of DigiPen Institute of Technology is prohibited.

   File Name:		Math2D.c
   Purpose:			Contains functions for resolving collisions between objects such as rectangles, points and circles.
   Language:		C programming language, Microsoft Visual C++ compiler
   Platform:		Microsoft Visual C++ 2017, Any machine running windows 10, Windows 10 OS>
   Project:			CS529_JRosenbluth_2
   Author:			Jose Rosenbluth Chiu, J.Rosenbluth, 60003418
   Creation date:	10/08/2018
-  End Header --------------------------------------------------------*/

#include <stdio.h>
#include "Math2D.h"
#include "Vector2D.h"
#include "stdio.h"

#include <windows.h>


/*
This function checks if the point P is colliding with the circle whose
center is "Center" and radius is "Radius"
*/
int StaticPointToStaticCircle(Vector2D *pP, Vector2D *pCenter, float Radius)
{
	if (!pP || !pCenter) 
	{
		printf("(StaticPointToStaticCircle)- One or more params are undefined");
	}

	return Vector2DSquareDistance(pP, pCenter) <= Radius * Radius;
}

/*
This function checks if the point Pos is colliding with the rectangle
whose center is Rect, width is "Width" and height is Height
*/
int StaticPointToStaticRect(Vector2D *pPos, Vector2D *pRect, float Width, float Height)
{
	if (!pPos || !pRect)
	{
		printf("(StaticPointToStaticRect)- One or more params are undefined");
	}

	float top    = pRect->y + Height / 2;
	float bottom = pRect->y - Height / 2;
	float right  = pRect->x + Width / 2;
	float left   = pRect->x - Width / 2;

	return (pPos->x >= left && pPos->x <= right && pPos->y <= top && pPos->y >= bottom);
}

/*
This function checks for collision between 2 circles.
Circle0: Center is Center0, radius is "Radius0"
Circle1: Center is Center1, radius is "Radius1"
*/
int StaticCircleToStaticCircle(Vector2D *pCenter0, float Radius0, Vector2D *pCenter1, float Radius1)
{
	if (!pCenter0 || !pCenter1)
	{
		printf("(StaticCircleToStaticCircle)- One or more params are undefined");
	}

	return Vector2DSquareDistance(pCenter0, pCenter1) <= (Radius0 + Radius1) * (Radius0 + Radius1);
}

/*
This functions checks if 2 rectangles are colliding
Rectangle0: Center is pRect0, width is "Width0" and height is "Height0"
Rectangle1: Center is pRect1, width is "Width1" and height is "Height1"
*/
int StaticRectToStaticRect(Vector2D *pRect0, float Width0, float Height0, Vector2D *pRect1, float Width1, float Height1)
{
	if (!pRect0 || !pRect1)
	{
		printf("(StaticRectToStaticRect)- One or more params are undefined");
	}

	float top0 = pRect0->y + Height0 / 2;
	float bottom0 = pRect0->y - Height0 / 2;
	float right0 = pRect0->x + Width0 / 2;
	float left0 = pRect0->x - Width0 / 2;

	float top1 = pRect1->y + Height1 / 2;
	float bottom1 = pRect1->y - Height1 / 2;
	float right1 = pRect1->x + Width1 / 2;
	float left1 = pRect1->x - Width1 / 2;

	short a = right0 < left1 || left0 > right1 || bottom0 > top1 || top0 < bottom1;
	return !a;
}


//////////////////////
// New to project 2 //
//////////////////////


/*
This function determines the distance separating a point from a line

 - Parameters
	- P:		The point whose location should be determined
	- LS:		The line segment

 - Returned value: The distance. Note that the returned value should be:
	- Negative if the point is in the line's inside half plane
	- Positive if the point is in the line's outside half plane
	- Zero if the point is on the line
*/
float StaticPointToStaticLineSegment(Vector2D *P, LineSegment2D *LS)
{
	if (!P || !LS) 
	{
		printf("(StaticPointToStaticLineSegment)- Error, one or more params undefined.");
		return 0.0f;
	}

	float dot = Vector2DDotProduct(P, &LS->mN);

	/*CHECK CASE IN WHICH BOTH ARE ZERO*/

	float epsilon = 0.000001f;
	if (fabs(dot - LS->mNdotP0) <= epsilon)
	{
		return 0.0f;
	}
	else 
	{
		return dot - LS->mNdotP0;
	}
}

/*
This function checks whether an animated point is colliding with a line segment

 - Parameters
	- Ps:		The point's starting location
	- Pe:		The point's ending location
	- LS:		The line segment
	- Pi:		This will be used to store the intersection point's coordinates (In case there's an intersection)

 - Returned value: Intersection time t
	- -1.0f:				If there's no intersection
	- Intersection time:	If there's an intersection
*/
float AnimatedPointToStaticLineSegment(Vector2D *Ps, Vector2D *Pe, LineSegment2D *LS, Vector2D *Pi)
{
	if (!Ps || !Pe || !LS || !Pi)
	{
		printf("(AnimatedPointToStaticLineSegment)- Error, one or more params undefined.");
		return -1.0f;
	}

	float nPs = Vector2DDotProduct(&LS->mN, Ps);

	Vector2D V;
	Vector2DSub(&V, Pe, Ps);
	float nV = Vector2DDotProduct(&LS->mN, &V);

	float epsilon = 0.000001f;
	if (fabs(nV) <= epsilon) 
	{
		return -1.0f;
	}

	float ti = (LS->mNdotP0 - nPs) / nV;
	Vector2DScaleAdd(Pi, &V, Ps, ti);

	if (ti > 0 && ti <= 1.0f) 
	{
		Vector2D P0toPi, P1toPi;
		Vector2DSub(&P0toPi, Pi, &LS->mP0);
		Vector2DSub(&P1toPi, Pi, &LS->mP1);
		float dot = Vector2DDotProduct(&P0toPi, &P1toPi);
		if (dot > 0.0f)
		{
			return -1.0f;
		}

		/*CHECK NO HEAP STACK PROBLEMS*/

		return ti;
	}

	return -1.0f;
}

/*
This function checks whether an animated circle is colliding with a line segment

 - Parameters
	- Ps:		The center's starting location
	- Pe:		The center's ending location
	- Radius:	The circle's radius
	- LS:		The line segment
	- Pi:		This will be used to store the intersection point's coordinates (In case there's an intersection)

 - Returned value: Intersection time t
	- -1.0f:				If there's no intersection
	- Intersection time:	If there's an intersection
*/
float AnimatedCircleToStaticLineSegment(Vector2D *Ps, Vector2D *Pe, float Radius, LineSegment2D *LS, Vector2D *Pi)
{
	if (!Ps || !Pe || !LS || !Pi)
	{
		printf("(AnimatedCircleToStaticLineSegment)- Error, one or more params undefined.");
		return -1.0f;
	}

	float distToLS = StaticPointToStaticLineSegment(Ps, LS);
	float r = sign(distToLS) * Radius;

	float NdotPs = Vector2DDotProduct(&LS->mN, Ps);
	Vector2D V;
	Vector2DSub(&V, Pe, Ps);
	float NdotV = Vector2DDotProduct(&LS->mN, &V);

	float epsilon = 0.000001f;
	if (fabs(NdotV) <= epsilon)
	{
		return -1.0f;
	}

	float ti = (LS->mNdotP0 - NdotPs + r) / NdotV;
	Vector2DScaleAdd(Pi, &V, Ps, ti);

	//char outTest[512];
	//sprintf(outTest, "\n>>> ti: %f, NdotV: %f. Rad= %f, sign= %d, r= %f", ti, NdotV, Radius, sign(distToLS), r);
	//OutputDebugString(outTest);

	if (ti > 0 && ti <= 1.0f)
	{
		Vector2D P0toPi, P1toPi;
		Vector2DSub(&P0toPi, Pi, &LS->mP0);
		Vector2DSub(&P1toPi, Pi, &LS->mP1);
		float dot = Vector2DDotProduct(&P0toPi, &P1toPi);
		if (dot > 0.0f)
		{
			return -1.0f;
		}

		/*CHECK NO HEAP STACK PROBLEMS*/

		return ti;
	}

	return -1.0f;
}

/*
This function reflects an animated point on a line segment.
It should first make sure that the animated point is intersecting with the line 

 - Parameters
	- Ps:		The point's starting location
	- Pe:		The point's ending location
	- LS:		The line segment
	- Pi:		This will be used to store the intersection point's coordinates (In case there's an intersection)
	- R:		Reflected vector R

 - Returned value: Intersection time t
	- -1.0f:				If there's no intersection
	- Intersection time:	If there's an intersection
*/
float ReflectAnimatedPointOnStaticLineSegment(Vector2D *Ps, Vector2D *Pe, LineSegment2D *LS, Vector2D *Pi, Vector2D *R)
{
	if (!Ps || !Pe || !LS || !Pi || !R)
	{
		printf("(ReflectAnimatedPointOnStaticLineSegment)- Error, one or more params undefined.");
		return -1.0f;
	}

	float t = AnimatedPointToStaticLineSegment(Ps, Pe, LS, Pi);

	if (t >= 0.0f && t <= 1.0f) 
	{
		Vector2D i, Br, M;
		Vector2DSub(&i, Pe, Pi);
		float BrDotN = Vector2DDotProduct(&i, &LS->mN);
		Vector2DScale(&Br, &LS->mN, BrDotN);
		Vector2DSub(&M, &i, &Br);

		Vector2DScaleAdd(R, &Br, &M, -1.0f);
		return t;
	}

	return -1.0f;
}

/*
This function reflects an animated circle on a line segment.
It should first make sure that the animated point is intersecting with the line 

 - Parameters
	- Ps:		The center's starting location
	- Pe:		The center's ending location
	- Radius:	The circle's radius
	- LS:		The line segment
	- Pi:		This will be used to store the intersection point's coordinates (In case there's an intersection)
	- R:		Reflected vector R

 - Returned value: Intersection time t
	- -1.0f:				If there's no intersection
	- Intersection time:	If there's an intersection
*/
float ReflectAnimatedCircleOnStaticLineSegment(Vector2D *Ps, Vector2D *Pe, float Radius, LineSegment2D *LS, Vector2D *Pi, Vector2D *R)
{
	if (!Ps || !Pe || !LS || !Pi || !R)
	{
		printf("(ReflectAnimatedCircleOnStaticLineSegment)- Error, one or more params undefined.");
		return -1.0f;
	}

	float t = AnimatedCircleToStaticLineSegment(Ps, Pe, Radius, LS, Pi);

	if (t >= 0.0f && t <= 1.0f) 
	{
		Vector2D i, Br, M;
		Vector2DSub(&i, Pe, Pi);
		float BrDotN = Vector2DDotProduct(&i, &LS->mN);
		Vector2DScale(&Br, &LS->mN, BrDotN);
		Vector2DSub(&M, &i, &Br);

		Vector2DScaleAdd(R, &Br, &M, -1.0f);
		return t;
	}

	return -1.0f;
}


////////////////////
//    PART 02     //
////////////////////


/*
This function checks whether an animated point is colliding with a static circle

 - Parameters
	- Ps:		The point's starting location
	- Pe:		The point's ending location
	- Center:	The circle's center
	- Radius:	The circle's radius
	- Pi:		This will be used to store the intersection point's coordinates (In case there's an intersection)

 - Returned value: Intersection time t
	- -1.0f:		If there's no intersection
	- Intersection time:	If there's an intersection
*/
float AnimatedPointToStaticCircle(Vector2D *Ps, Vector2D *Pe, Vector2D *Center, float Radius, Vector2D *Pi)
{
	if (!Ps || !Pe || !Center || !Pi)
	{
		printf("(AnimatedPointToStaticCircle)- Error, one or more params undefined.");
		OutputDebugString("(AnimatedPointToStaticCircle)- Error, one or more params undefined.");
		return -1.0f;
	}

	Vector2D V;
	float PsdotPs, VdotV, PsdotV, VdotC, PsdotC, CdotC;

	Vector2DSub(&V, Pe, Ps);
	PsdotPs = Vector2DDotProduct(Ps, Ps);
	VdotV = Vector2DDotProduct(&V, &V);
	PsdotV = Vector2DDotProduct(Ps, &V);
	VdotC = Vector2DDotProduct(&V, Center);
	PsdotC = Vector2DDotProduct(Ps, Center);
	CdotC = Vector2DDotProduct(Center, Center);

	float A = VdotV;
	float B = 2 * PsdotV - 2 * VdotC;
	float C = PsdotPs - 2*PsdotC + CdotC - Radius*Radius;
	float disc = B*B - 4 * A * C;

	float epsilon = 0.000001f;
	if (disc < 0.0f || abs(A) <= epsilon)
	{
		return -1.0f;
	}

	float t1 = (-B + (float)sqrt(disc)) / (2 * A);
	float t2 = (-B - (float)sqrt(disc)) / (2 * A);
	float ti = floatMin(t1, t2);

	if (ti > 0 && ti <= 1.0f)
	{
		/*CHECK NO HEAP STACK PROBLEMS*/
		Vector2DScaleAdd(Pi, &V, Ps, ti);
		return ti;
	}

	return -1.0f;
}

/*
This function reflects an animated point on a static circle.
It should first make sure that the animated point is intersecting with the circle 

 - Parameters
	- Ps:		The point's starting location
	- Pe:		The point's ending location
	- Center:	The circle's center
	- Radius:	The circle's radius
	- Pi:		This will be used to store the intersection point's coordinates (In case there's an intersection)
	- R:		Reflected vector R

 - Returned value: Intersection time t
	- -1.0f:		If there's no intersection
	- Intersection time:	If there's an intersection
*/
float ReflectAnimatedPointOnStaticCircle(Vector2D *Ps, Vector2D *Pe, Vector2D *Center, float Radius, Vector2D *Pi, Vector2D *R)
{
	if (!Ps || !Pe || !Center || !Pi || !R)
	{
		printf("(ReflectAnimatedPointOnStaticCircle)- Error, one or more params undefined.");
		return -1.0f;
	}

	float ti = AnimatedPointToStaticCircle(Ps, Pe, Center, Radius, Pi);

	if (ti >= 0.0f && ti <= 1.0f) 
	{
		/*Get the center - pi, and then rotate to get vector parallel to new LS*/
		Vector2D CtoPi, Perp, tgP0, tgP1;
		Vector2DSub(&CtoPi, Pi, Center);
		Vector2DSet(&Perp, CtoPi.y, -CtoPi.x);
		tgP0 = *Pi;
		Vector2DScaleAdd(&tgP1, &Perp, &tgP0, 1.0f);
		
		/*Create the new line segment that we will use for reflection*/
		LineSegment2D LS;
		BuildLineSegment2D(&LS, &tgP0, &tgP1);

		/*Call normal point on line segment reflection*/
		Vector2D i, Br, M;
		Vector2DSub(&i, Pe, Pi);
		float BrDotN = Vector2DDotProduct(&i, &LS.mN);
		Vector2DScale(&Br, &LS.mN, BrDotN);
		Vector2DSub(&M, &i, &Br);
		Vector2DScaleAdd(R, &Br, &M, -1.0f);

		return ti;
	}

	return -1.0f;
}

/*
This function checks whether an animated circle is colliding with a static circle

 - Parameters
	- Center0s:		The starting position of the animated circle's center 
	- Center0e:		The ending position of the animated circle's center 
	- Radius0:		The animated circle's radius
	- Center1:		The static circle's center
	- Radius1:		The static circle's radius
	- Pi:			This will be used to store the intersection point's coordinates (In case there's an intersection)

 - Returned value: Intersection time t
	- -1.0f:		If there's no intersection
	- Intersection time:	If there's an intersection
*/
float AnimatedCircleToStaticCircle(Vector2D *Center0s, Vector2D *Center0e, float Radius0, Vector2D *Center1, float Radius1, Vector2D *Pi)
{
	return AnimatedPointToStaticCircle(Center0s, Center0e, Center1, Radius0 + Radius1, Pi);
}

/*
This function reflects an animated circle on a static circle.
It should first make sure that the animated circle is intersecting with the static one 

 - Parameters
	- Center0s:		The starting position of the animated circle's center 
	- Center0e:		The ending position of the animated circle's center 
	- Radius0:		The animated circle's radius
	- Center1:		The static circle's center
	- Radius1:		The static circle's radius
	- Pi:			This will be used to store the intersection point's coordinates (In case there's an intersection)
	- R:			Reflected vector R

 - Returned value: Intersection time t
	- -1.0f:		If there's no intersection
	- Intersection time:	If there's an intersection
*/
float ReflectAnimatedCircleOnStaticCircle(Vector2D *Center0s, Vector2D *Center0e, float Radius0, Vector2D *Center1, float Radius1, Vector2D *Pi, Vector2D *R)
{
	return ReflectAnimatedPointOnStaticCircle(Center0s, Center0e, Center1, Radius0 + Radius1, Pi, R);
}

/*Gets the sign of the float*/
int sign(float val)
{
	int a = (0.0f < val);
	int b = (val < 0.0f);
	int c = a - b;
	return c;
}

/*Returns the min of the 2 floats*/
float floatMin(float A, float B)
{
	if (A < B)
		return A;
	return B;
}
