#include "BoxCollider2D.h"
#include "../Managers.h"
#include "../GameObject.h"
#include "../Math/Matrix3D.h"
#include "Transform.h"
#include <iostream>

#include "GL/glew.h"
#include "GL/gl.h"

extern Manager *pManager;


BoxCollider2D::BoxCollider2D(GameObject *owner, COMPONENT_TYPE type) : 
	Component(owner, type)
{
	//LATER SEE IF BETTER USE M.I.L.
}

BoxCollider2D::~BoxCollider2D() 
{
}

void BoxCollider2D::Update(unsigned int deltaTime)
{
	if (getOwner() == 0) 
	{
		return;
	}

	Transform *T = static_cast<Transform*>(getOwner()->GetComponent(COMPONENT_TYPE::TRANSFORM));
	if (T != 0) 
	{
		Vector3DSet(&center, T->getPosition().x, T->getPosition().y, T->getPosition().z);
		this->angle = T->getAngle();
	}

	//Get the rotated vertices (for AST)
	Matrix3D ROT;
	Vector3D axis;
	Vector3DSet(&axis, 0, 0, 1); //TO ROTATE IN XY PLANE
	Vector3D TR, TL, BR, BL;
	Vector3DSet(&TR, center.x + width / 2.0f, center.y + height / 2.0f, center.z);
	Vector3DSet(&TL, center.x - width / 2.0f, center.y + height / 2.0f, center.z);
	Vector3DSet(&BR, center.x + width / 2.0f, center.y - height / 2.0f, center.z);
	Vector3DSet(&BL, center.x - width / 2.0f, center.y - height / 2.0f, center.z);
	Matrix3DRotAxisDeg(&ROT, &axis, angle);
	Matrix3DMultVec(&TR, &ROT, &TR);
	Matrix3DMultVec(&TL, &ROT, &TL);
	Matrix3DMultVec(&BR, &ROT, &BR);
	Matrix3DMultVec(&BL, &ROT, &BL);
}

void BoxCollider2D::DrawBoundingBox() 
{
	Transform *T = static_cast<Transform*>(getOwner()->GetComponent(COMPONENT_TYPE::TRANSFORM));
	if (T != 0)
	{
		unsigned int tempvbo;
		glUseProgram(1);
		glGenBuffers(1, &tempvbo);
		glBindBuffer(GL_ARRAY_BUFFER, tempvbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glEnableVertexAttribArray(0);
		glDrawArrays(GL_LINES, 0, 8);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDeleteBuffers(1, &tempvbo);
		glUseProgram(0);
	}
}

Component *BoxCollider2D::createNew(GameObject *owner)
{
	return new BoxCollider2D(owner, COMPONENT_TYPE::BOXCOLLIDER2D);
}

void BoxCollider2D::serialize(std::fstream& stream)
{
}

void BoxCollider2D::deserialize(std::fstream& stream)
{
	std::cout << "DESERIALIZING BOXCOLLIDER2D BEGIN" << std::endl;

	float w, h;

	if (stream >> w >> h)
	{
		this->width = w;
		this->height = h;

		//Add vertices information
		Vector3D TR, TL, BR, BL;
		Vector3DSet(&TR, center.x + width / 2.0f, center.y + height / 2.0f, center.z);
		Vector3DSet(&TL, center.x - width / 2.0f, center.y + height / 2.0f, center.z);
		Vector3DSet(&BR, center.x + width / 2.0f, center.y - height / 2.0f, center.z);
		Vector3DSet(&BL, center.x - width / 2.0f, center.y - height / 2.0f, center.z);

		vertices[0] = TL.x;   vertices[1] = TL.y;   vertices[2] = TL.z;
		vertices[3] = TR.x;   vertices[4] = TR.y;   vertices[5] = TR.z;

		vertices[6] = TR.x;   vertices[7] = TR.y;   vertices[8] = TR.z;
		vertices[9] = BR.x;   vertices[10] = BR.y;  vertices[11] = BR.z;

		vertices[12] = BR.x;  vertices[13] = BR.y;  vertices[14] = BR.z;
		vertices[15] = BL.x;  vertices[16] = BL.y;  vertices[17] = BL.z;

		vertices[18] = BL.x;  vertices[19] = BL.y;  vertices[20] = BL.z;
		vertices[21] = TL.x;  vertices[22] = TL.y;  vertices[23] = TL.z;
	}
	else
	{
		std::cout << "(BoxCollider2D::serialize)- Failed to read stream" << std::endl;
	}

	std::cout << "DESERIALIZING BOXCOLLIDER2D END" << std::endl;
}