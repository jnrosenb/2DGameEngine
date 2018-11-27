#include <iostream>
#include "Animator.h"
#include "../GameObject.h"


Animator::Animator(GameObject *owner, COMPONENT_TYPE type) : 
	Component(owner, type)
{
	//See who fills clips and with what information
	currentAnimation = "";
}

Animator::~Animator()
{
	for (auto node : clips)
	{
		delete node.second;
	}
	clips.clear();
}


AnimationClip *Animator::getCurrentClip()
{
	return clips[currentAnimation];
}

void Animator::Update(unsigned int deltaTime)
{
	//Update current animation
	AnimationClip *current = clips[currentAnimation];
	if (current)
	{
		current->Update(deltaTime  / 1000.0f);
	}
	else 
	{
		std::cout << "(Animator::Update)- Null clip pointer" << std::endl;
	}
}

Component *Animator::createNew(GameObject *owner)
{
	return new Animator(owner, COMPONENT_TYPE::ANIMATOR);
}

void Animator::serialize(std::fstream& stream)
{
}

void Animator::deserialize(std::fstream& stream)
{
	std::cout << "DESERIALIZING ANIMATOR BEGIN" << std::endl;

	std::string line;
	if (stream >> line)
	{
		if (line == "ANIMATIONS") 
		{
			while (stream >> line) 
			{
				if (line == "ANIMATIONS_END")
					break;

				int fps, begin, end;
				bool loops;
				if (stream >> fps >> begin >> end >> loops)
				{
					AnimationClip *clip = new AnimationClip(line, fps, begin, end, loops);
					if (clip)
					{
						AnimationClip *exists = clips[clip->animationTag()];
						if (!exists)
							clips[clip->animationTag()] = clip;

						//CURRENT ANIMATION. HARDCODED FOR NOW
						if (currentAnimation.empty()) 
						{
							currentAnimation = clip->animationTag();
						}

						//Pass the clip a method for callback (EXPERIMENT)
						//WHO DELETES THIS GUY???
						///callbackEvent *callback = new AnimatorCallback(this, &Animator::onAnimationEndCallback);
						///clip->animCallback = callback;
					}
				}
				else
				{
					std::cout << "\r(Animator::deserialize)- Error, stream failed 2" << std::endl;
				}
			}

			//Create the none animationClip (returns a nullptr )
			clips["NONE"] = 0;
		}
	}
	else
	{
		std::cout << "\r(Animator::deserialize)- Error, stream failed 1" << std::endl;
	}

	std::cout << "DESERIALIZING ANIMATOR END" << std::endl;
}

std::string Animator::getCurrentAnimationTag()
{
	return currentAnimation;
}

void Animator::Play(std::string animation) 
{
	currentAnimation = animation;
}

void Animator::Play(std::string animation, callbackEvent *ev) 
{
	currentAnimation = animation;
	AnimationClip *current = clips[currentAnimation];
	if (current)
	{
		current->animCallback = ev;
	}
}

void Animator::handleEvent(Event *pEvent)
{
	if (pEvent->type == EventType::ANIMATION_SWITCH) 
	{
		OnAnimationSwitch *animSwitch = static_cast<OnAnimationSwitch*>(pEvent);
		if (animSwitch) 
		{
			this->Play(animSwitch->animTag);
		}
	}
}

//EXPERIMENT FOR ANIMATOR CALLBACK
void Animator::onAnimationEndCallback()
{
	//TODO
}