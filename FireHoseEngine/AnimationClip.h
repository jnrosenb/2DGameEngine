#ifndef ANIMATIONCLIP_H
#define ANIMATIONCLIP_H

#include <string>

class AnimationClip 
{
public:
	AnimationClip(std::string ptag, unsigned int fps,
		unsigned int begin, unsigned int end, bool ploops);
	~AnimationClip();

	unsigned int getFPS();
	unsigned int getNumberOfFrames();
	unsigned int getCurrentFrame();
	void setCurrentFrame(unsigned int frame);
	std::string animationTag();

	void Update(float dt);

	int getBegin()
	{
		return beginIndex;
	}

private:
	std::string tag;
	unsigned int FPS;
	unsigned int beginIndex;
	unsigned int endIndex;
	unsigned int framesNum;
	unsigned int currentFrame;
	float currentFrameTimeElapsed;
	float secondsPerFrame;
	bool loops;
};

#endif