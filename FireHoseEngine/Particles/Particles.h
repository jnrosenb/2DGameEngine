#ifndef PARTICLES_H
#define PARTICLES_H

#include <fstream>
#include "../Math/Vector3D.h"
#include "../Events.h"


class Particle
{
public:
	Particle();
	Particle(float size, float angle, float mass, float speed, 
		char r, char g, char b, char a);
	virtual ~Particle();

	void Update(float dt, int *numberOfActiveParticles);

	Vector3D const& getCenter();
	float getSize();
	bool isAlive();
	void ResetParticle();
	void saveVertexPos(Vector3D position);
	Vector3D const& getVertexPos();

public:
	unsigned char color[4];

private:
	float timeToLive;
	float pSize, pSpeed, pAngle, pMass;
	Vector3D pPos, pVelocity, vertexPos;
};

#endif