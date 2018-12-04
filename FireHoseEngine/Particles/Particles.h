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
		float rotSpeed, float spread, bool gravity);
	virtual ~Particle();

	void Update(float dt, int *numberOfActiveParticles);

	Vector3D const& getCenter();
	float getSize();
	bool isAlive();
	void ResetParticle();
	void saveVertexPos(Vector3D position);
	Vector3D const& getVertexPos();
	float GetAngleInRadian();
	void SetAnimationParameters(bool anim, int r, int c,
		int rBeg, int rEnd, int fps, bool loops);
	int getBegin();
	int getEnd();
	int getCurrentFrame();
	int getAnimationSheetDimention();

public:
	unsigned char color[4];

private:
	float timeToLive;
	bool useGravity;
	float pSize, pSpeed, pRotSpeed, pAngle, pMass;
	Vector3D pPos, pVelocity, vertexPos;

	//Constant initial parameters
	float const pSpread;
	float const initialSize;
	float const initialSpeed;
	float const initialRotSpeed;
	float const initialMass;
	float const initialAngle;

	//Animation parameters
	bool animated, looping;
	int rows, cols, begin, end, FPS;
	int currentFrame;
	float timeInCurrentFrame, secondsPerFrame;
};

#endif