#include <iostream>
#include "AnimationClip.h"


AnimationClip::AnimationClip(std::string ptag, unsigned int fps,
	unsigned int begin, unsigned int end, bool ploops) :
	FPS(fps), tag(ptag), beginIndex(begin), endIndex(end),
	loops(ploops), animCallback(0)
{
	this->framesNum = end - begin + 1;
	this->secondsPerFrame = 1.0f / FPS;
	currentFrame = 0;
	currentFrameTimeElapsed = 0.0f;
}

AnimationClip::~AnimationClip()
{
	if (animCallback && animCallback->IsAllocated())
	{
		delete animCallback;
	}
}

void AnimationClip::Update(float dt)
{
	this->currentFrameTimeElapsed += dt;
	if (currentFrameTimeElapsed >= secondsPerFrame) 
	{
		if (loops) 
		{
			currentFrame = (currentFrame + 1) % framesNum;
			currentFrameTimeElapsed = 0.0f;
		}
		else 
		{
			if (currentFrame == framesNum - 1)
			{
				//Reached animation end, callback
				if (animCallback)
				{
					// TODO -TEMPORARY, ERASE LATER : 
					// reset clip for next time
					currentFrame = 0;
					currentFrameTimeElapsed = 0.0f;

					animCallback->callback();
				}
			}
			else 
			{
				currentFrame += 1;
				currentFrameTimeElapsed = 0.0f;
			}
		}

		//std::cout << "ANIM FRAME CHANGE, current: " << currentFrame << ". DeltaTime: " << dt << std::endl;
	}
}

unsigned int AnimationClip::getFPS()
{
	return FPS;
}

unsigned int AnimationClip::getNumberOfFrames()
{
	return framesNum;
}

unsigned int AnimationClip::getCurrentFrame()
{
	return currentFrame;
}

void AnimationClip::setCurrentFrame(unsigned int frame)
{
	if (frame < framesNum)
		currentFrame = frame;
}

std::string AnimationClip::animationTag() 
{
	return tag;
}
