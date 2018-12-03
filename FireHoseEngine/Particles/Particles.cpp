#include "Particles.h"


Particle::Particle() :
	timeToLive(-1.0f), pSize(1.0f), pAngle(0.0f),
	pMass(1.0f), pSpeed(1.0f)
{
	Vector3DSet(&pPos, 0, 0, 0);
	Vector3DSet(&pVelocity, 0, 1, 0);

	for (int i = 0; i < 4; ++i)
		color[i] = 0;

	//TEMPORARY
	int angleDegrees = rand() % 360;
	Vector3D dir;
	Vector3DFromAngle2DDeg(&dir, angleDegrees);
	Vector3DSet(&pVelocity, dir.x * pSpeed, dir.y * pSpeed, dir.z * pSpeed);
}

Particle::Particle(float ttl, float size, float angle,
	float mass, float speed, char r, char g, char b, char a,
	Vector3D pos, Vector3D vel) :
		timeToLive(-1.0f), pSize(size), pAngle(angle),
		pMass(mass), pSpeed(speed), pPos(pos), pVelocity(vel)
{
	color[0] = r;
	color[1] = g;
	color[2] = b;
	color[3] = a;

	//TEMPORARY
	int angleDegrees = rand() % 360;
	Vector3D dir;
	Vector3DFromAngle2DDeg(&dir, angleDegrees);
	Vector3DSet(&pVelocity, dir.x * pSpeed, dir.y * pSpeed, dir.z * pSpeed);
}

Particle::~Particle()
{
}

void Particle::Update(float dt, int *numberOfActiveParticles)
{
	if (timeToLive > 0.0f)
	{
		timeToLive -= dt;
		Vector3DScaleAdd(&pPos, &pVelocity, &pPos, dt);

		if (timeToLive < 0.0f) 
		{
			timeToLive = -1.0f;
			--(*numberOfActiveParticles);
		}
	}
}

bool Particle::isAlive() 
{
	return timeToLive > 0.0f;
}

void Particle::ResetParticle()
{
	Vector3DSet(&pPos, 0, 0, 0);
	timeToLive = 2.0f;//rand() % 5; //TODO - REPLACE

	////TODO - REPLACE TEMPORARY
	int angleDegrees = rand() % 360;
	Vector3D dir;
	Vector3DFromAngle2DDeg(&dir, angleDegrees);
	Vector3DSet(&pVelocity, dir.x * pSpeed, dir.y * pSpeed, dir.z * pSpeed);
}

void Particle::saveVertexPos(Vector3D position)
{
	Vector3DSet(&vertexPos, position.x, position.y, position.z);
}

Vector3D const& Particle::getVertexPos()
{
	return vertexPos;
}

Vector3D const& Particle::getCenter()
{
	return pPos;
}

float Particle::getSize() 
{
	return pSize;
}