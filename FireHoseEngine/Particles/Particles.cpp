#include "Particles.h"
#include <cstdlib>
#include <ctime>


Particle::Particle() : 
	timeToLive(-1.0f), pSize(0.125f), pAngle(0.0f), pMass(1.0f), pSpeed(1.0f)
{
	for (int i = 0; i < 4; ++i)
		color[i] = 0;

	Vector3DSet(&pPos, 0, 0, 0);
	Vector3DSet(&pVelocity, 0, 0, 0);
}


Particle::Particle(float size, float angle, float mass, float speed, 
	char r, char g, char b, char a) :
		timeToLive(-1.0f), pSize(size), pAngle(angle), pMass(mass), pSpeed(speed)
{
	color[0] = r;
	color[1] = g;
	color[2] = b;
	color[3] = a;

	Vector3DSet(&pPos, 0, 0, 0);
	Vector3DSet(&pVelocity, 0, 0, 0);
}


Particle::~Particle()
{
}


void Particle::Update(float dt, int *numberOfActiveParticles)
{
	if (timeToLive > 0.0f)
	{
		timeToLive -= dt;

		//GRAVITY AND FRICTION
		float gravityPull = -1.0f;
		Vector3D gravity;
		Vector3DSet(&gravity, 0.0f, gravityPull * pMass * dt, 0.0f);
		Vector3DAdd(&pVelocity, &pVelocity, &gravity);

		//TEMPORAL VELOCITY CAP
		float p = 0.01f;
		Vector3D AirResistance;
		Vector3DSet(&AirResistance, -pVelocity.x * p, -pVelocity.y * p, -pVelocity.z * p);
		Vector3DAdd(&pVelocity, &pVelocity, &AirResistance);

		//To position
		Vector3DScaleAdd(&pPos, &pVelocity, &pPos, pSpeed * dt);

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

	timeToLive = 3.0f;
	pSpeed = 2.0f + ( (rand() % 100 ) / 100.0f );
	pMass =  1.0f + ( (rand() % 1000) / 100.0f );
	pSize =  0.25f * ( (rand() % 100 ) / 100.0f );

	color[0] = rand() % 256;
	color[1] = rand() % 256;
	color[2] = rand() % 256;
	color[3] = 255;

	int angleDegrees = 45 + (rand() % 90);
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