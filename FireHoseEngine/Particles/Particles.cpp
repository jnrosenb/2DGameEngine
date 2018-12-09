#include "Particles.h"
#include <cstdlib>
#include <ctime>


#define PI 3.14159265359


Particle::Particle() : 
	timeToLive(-1.0f), initialSize(0.25f), initialAngle(0.0f), 
	initialMass(1.0f), initialSpeed(1.0f), initialRotSpeed(5.0f),
	pSpread(2.0f), useGravity(true), animated(false), currentFrame(0), looping(true)
{
	for (int i = 0; i < 4; ++i)
		color[i] = 0;

	Vector3DSet(&pPos, 0, 0, 0);
	Vector3DSet(&pVelocity, 0, 0, 0);
}


Particle::Particle(float size, float angle, float mass, 
	float speed, float rotSpeed, float spread, bool gravity) :
	timeToLive(-1.0f), initialSize(size), initialAngle(angle),
	initialMass(mass), initialSpeed(speed), initialRotSpeed(rotSpeed),
	pSpread(spread), useGravity(gravity), animated(false), currentFrame(0), looping(true)
{
	for (int i = 0; i < 4; ++i)
		color[i] = 0;

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
		if (useGravity) 
		{
			float gravityPull = -1.0f;
			Vector3D gravity;
			Vector3DSet(&gravity, 0.0f, gravityPull * pMass * dt, 0.0f);
			Vector3DAdd(&pVelocity, &pVelocity, &gravity);
		}

		//TEMPORAL VELOCITY CAP
		float p = 0.01f;
		Vector3D AirResistance;
		Vector3DSet(&AirResistance, -pVelocity.x * p, -pVelocity.y * p, -pVelocity.z * p);
		Vector3DAdd(&pVelocity, &pVelocity, &AirResistance);

		//TEMPORAL ROTATION EXPERIMENT
		pAngle += pRotSpeed * dt;

		//UPDATE POSITION
		Vector3DScaleAdd(&pPos, &pVelocity, &pPos, pSpeed * dt);

		//UPDATE ANIMATION CURRENT FRAME
		timeInCurrentFrame += dt;
		if (animated && timeInCurrentFrame >= secondsPerFrame) 
		{
			int pastFinalFrame = end - begin + 1;
			++currentFrame;
			//currentFrame = (currentFrame + 1) % pastFinalFrame;

			//timeInCurrentFrame = 0.0f;
			timeInCurrentFrame = (timeInCurrentFrame - secondsPerFrame);

			//If at the end of the animation, kill it unless looping
			if (!looping && currentFrame == pastFinalFrame) 
			{
				timeToLive = -1.0f;
			}
			else if (looping && currentFrame == pastFinalFrame)
			{
				currentFrame = 0;
			}
		}

		//Kill the ones that are with negative time
		if (timeToLive < 0.0f)
		{
			timeToLive = -1.0f;
			--(*numberOfActiveParticles);
		}
	}
}


void Particle::SetAnimationParameters(bool anim, int r, int c, 
	int rBeg, int rEnd, int fps, bool loops)
{
	if (anim) 
	{
		animated = true;
		rows = r;
		cols = c;
		begin = rBeg;
		end = rEnd;
		FPS = fps;
		secondsPerFrame = 1.0f / fps;
		looping = loops;
	}
	else 
	{
		animated = false;
		rows = r;
		cols = c;
		begin = 0;
		end = 0;
		FPS = 0;
		secondsPerFrame = 0.0f;
	}
}


int Particle::getBegin()
{
	return begin;
}


int Particle::getEnd()
{
	return end;
}


int Particle::getCurrentFrame()
{
	return currentFrame;
}


int Particle::getAnimationSheetDimention()
{
	return rows;
}


bool Particle::isAlive() 
{
	return timeToLive > 0.0f;
}


float Particle::GetAngleInRadian()
{
	return static_cast<float>( (PI * pAngle) / 180.0f );
}


void Particle::ResetParticle()
{
	//For now, all particles start at center
	Vector3DSet(&pPos, 0, 0, 0);

	//SPECIAL CASE
	timeToLive = 5.0f;

	//Setting the parameters using the const plus spread (INNEFFICIENT)
	pSpeed    = initialSpeed    + pSpread * ( (rand() % 1000 ) / 1000.0f );
	pRotSpeed = initialRotSpeed + pSpread * ( (rand() % 1000 ) / 1000.0f );
	pMass     = initialMass     + pSpread * ( (rand() % 1000 ) / 1000.0f );
	pSize     = initialSize     + pSpread * ( (rand() % 1000 ) / 1000.0f );
	pAngle    = initialAngle    + pSpread * ( (rand() % 1000 ) / 1000.0f );

	//Color of particle will be white if animated
	if (animated) 
	{
		for (int i = 0; i < 4; ++i)
			color[i] = 255;
	}
	else 
	{
		color[0] = rand() % 256;
		color[1] = rand() % 256;
		color[2] = rand() % 256;
		color[3] = 255;
	}

	//Speed direction (TODO serialize this)
	float angleDegrees = 45.0f + (rand() % 90);
	Vector3D dir;
	Vector3DFromAngle2DDeg(&dir, angleDegrees);
	Vector3DSet(&pVelocity, dir.x * pSpeed, dir.y * pSpeed, dir.z * pSpeed);

	//Animation reset parameters
	currentFrame = 0;
	timeInCurrentFrame = 0.0f;
}


void Particle::ResetParticle(EmissionShape shape, int begin, int end, int fps)
{
	//For now, all particles start at center
	Vector3DSet(&pPos, 0, 0, 0);

	//SPECIAL CASE
	timeToLive = 5.0f;

	//Setting the parameters using the const plus spread (INNEFFICIENT)
	pSpeed = initialSpeed + pSpread * ((rand() % 1000) / 1000.0f);
	pRotSpeed = initialRotSpeed + pSpread * ((rand() % 1000) / 1000.0f);
	pMass = initialMass + pSpread * ((rand() % 1000) / 1000.0f);
	pSize = initialSize + pSpread * ((rand() % 1000) / 1000.0f);
	pAngle = initialAngle + pSpread * ((rand() % 1000) / 1000.0f);

	//Color of particle will be white if animated
	if (animated)
	{
		for (int i = 0; i < 4; ++i)
			color[i] = 255;
	}
	else
	{
		color[0] = rand() % 256;
		color[1] = rand() % 256;
		color[2] = rand() % 256;
		color[3] = 255;
	}

	//Speed direction (TODO serialize this)
	if (shape == EmissionShape::CIRCLE) 
	{
		float angleDegrees = 0.0f + rand() % 360;
		Vector3D dir;
		Vector3DFromAngle2DDeg(&dir, angleDegrees);
		Vector3DSet(&pVelocity, dir.x * pSpeed, dir.y * pSpeed, dir.z * pSpeed);
	}
	else if (shape == EmissionShape::CONE_DOWN)
	{
		float angleDegrees = 225.0f + (rand() % 90);
		Vector3D dir;
		Vector3DFromAngle2DDeg(&dir, angleDegrees);
		Vector3DSet(&pVelocity, dir.x * pSpeed, dir.y * pSpeed, dir.z * pSpeed);
	}
	else if (shape == EmissionShape::CONE_LEFT)
	{
		float angleDegrees = 135.0f + (rand() % 90);
		Vector3D dir;
		Vector3DFromAngle2DDeg(&dir, angleDegrees);
		Vector3DSet(&pVelocity, dir.x * pSpeed, dir.y * pSpeed, dir.z * pSpeed);
	}
	else if (shape == EmissionShape::CONE_RIGHT)
	{
		float angleDegrees = 0.0f + ( 315 + (rand() % 90) ) % 360;
		Vector3D dir;
		Vector3DFromAngle2DDeg(&dir, angleDegrees);
		Vector3DSet(&pVelocity, dir.x * pSpeed, dir.y * pSpeed, dir.z * pSpeed);
	}
	else if (shape == EmissionShape::CONE_UP)
	{
		float angleDegrees = 45.0f + (rand() % 90);
		Vector3D dir;
		Vector3DFromAngle2DDeg(&dir, angleDegrees);
		Vector3DSet(&pVelocity, dir.x * pSpeed, dir.y * pSpeed, dir.z * pSpeed);
	}

	//Animation reset parameters
	this->begin = begin;
	this->end =end;
	this->FPS = fps;
	currentFrame = 0;
	timeInCurrentFrame = 0.0f;
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