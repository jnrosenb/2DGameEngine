#include <iostream>
#include <cstring>
#include "../Managers.h"
#include "../GameObject.h"
#include "Renderer.h"
#include "Sprite.h"
#include "Transform.h"
#include "Camera.h"
#include "Animator.h"
#include "../AnimationClip.h"

#include "RigidBody2D.h" /*DEBUG TEMP DRAW*/
#include "../Collision/Shapes.h" /*DEBUG TEMP DRAW*/

extern Manager *pManager;

Renderer::Renderer(GameObject *owner, COMPONENT_TYPE type) :
	Component(owner, type)
{
	//TODO
	//SendOpenGLInstanceData(); /*TEMPORARY HERE*/
	//SetGlParams();
}

Renderer::~Renderer()
{
	//TODO
}

void Renderer::Update(unsigned int deltaTime)
{
}

void Renderer::Draw()
{
	if (!isEnabled()) 
	{
		return;
	}

	if (this->getOwner() == 0) 
	{
		//There is always gonna be a renderer without owner. We do not draw it
		return;
	}

	//Only draw if owner has a sprite component and a transform component (and there is a camera)
	Sprite * S = static_cast<Sprite*>(this->getOwner()->GetComponent(COMPONENT_TYPE::SPRITE));
	Transform *T = static_cast<Transform*>(this->getOwner()->GetComponent(COMPONENT_TYPE::TRANSFORM));
	Camera *C = pManager->GetCameraManager()->GetMainCamera();
	if (S && T && C)
	{
		//Passs the matrix information
		glUseProgram(mProgram);
		glUniformMatrix4fv(umodel, 1, GL_FALSE, &(T->M.m[0][0]));

		///Texture information and blending
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		//Texture information and blending
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, S->mTexture);

		//Bind VAO
		glBindVertexArray(mVao);

		///////////////////////////////////////////////////////////////////////
		///Check if sprite is animated. If it is, handle the animation here////
		Animator *animator = static_cast<Animator*>(getOwner()->GetComponent(COMPONENT_TYPE::ANIMATOR));
		if (S->isAnimated() && animator) 
		{
			AnimationClip *currentClip = animator->getCurrentClip();
			if (currentClip) 
			{
				int animationOffset = (8 * 4) * currentClip->getBegin() + (8 * 4) * currentClip->getCurrentFrame();

				//DO this only when having a frame change
				//Send new uvs to opengl 
				glBindBuffer(GL_ARRAY_BUFFER, S->getSpriteVboUv());
				glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)animationOffset);
				glEnableVertexAttribArray(1);
			}
		}
		else 
		{
			//CHANGE VBO IN GR_MGR TO PRIVATE
			glBindBuffer(GL_ARRAY_BUFFER, pManager->GetGraphicManager()->vbo[1]);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
			glEnableVertexAttribArray(1);
		}
		///-///////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////

		//Draw the quad and get errors
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		GLenum err = glGetError();
		if (err != GL_NO_ERROR)
		{
			switch (err)
			{
			case GL_INVALID_ENUM:
				std::cout << "ERROR: GL_INVALID_ENUM" << std::endl;
				break;
			case GL_INVALID_VALUE:
				std::cout << "ERROR: GL_INVALID_VALUE" << std::endl;
				break;
			case GL_INVALID_OPERATION:
				std::cout << "ERROR: GL_INVALID_OPERATION" << std::endl;
				break;
			default:
				std::cout << "ERROR: None of the previous" << std::endl;
				break;
			}
		}

		glBindVertexArray(0);
		glUseProgram(0);
	}
}

void Renderer::SetGlParams(GLuint pgm, GLuint vao)
{
	mProgram = pgm;
	mVao = vao;
	umodel = glGetUniformLocation(mProgram, "model");
}

void Renderer::SetGlParams() 
{
	mProgram = pManager->GetGraphicManager()->getProgram(0);
	mVao = pManager->GetGraphicManager()->getVao();
	umodel = glGetUniformLocation(mProgram, "model");
}

Component *Renderer::createNew(GameObject *owner)
{
	return new Renderer(owner, COMPONENT_TYPE::RENDERER);
}

void Renderer::serialize(std::fstream& stream)
{
}

void Renderer::deserialize(std::fstream& stream)
{
	std::cout << "DESERIALIZING RENDERER BEGIN" << std::endl;

	bool  enable;
	if (stream >> enable) 
	{
		SetGlParams();
		setEnabled(enable);

		//Pass himself to the graphic manager in order to be called when drawing
		if (pManager->GetGraphicManager() != 0)
			pManager->GetGraphicManager()->AddRendererComponent(this);
	}
	else 
	{
		std::cout << "\r(Renderer::deserialize)- Error, stream failed" << std::endl;
	}

	std::cout << "DESERIALIZING RENDERER END (ORDER IT ALL)" << std::endl;
}
