/* Start Header -------------------------------------------------------
*  Copyright (C) 2018 DigiPen Institute of Technology.
*  Reproduction or disclosure of this file or its contents without the prior
*  written consent of DigiPen Institute of Technology is prohibited.

*  File Name: InputManager.h
*  Purpose: Main source file. Contains the main loop, and is in charge of
*  creating and destroying all the managers used in the engine.
*  Project: CS529_JRosenbluth_3
*  Author: Jose Rosenbluth Chiu, J.Rosenbluth, 60003418
*  Creation date: 10/14/2018
-  End Header --------------------------------------------------------*/

#include <iostream>
#include "GL/glew.h"
#include "GL/gl.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"

#include "Managers.h"
#include "GameStateManager.h"
#include "FramerateController.h"
#include "InputManager.h"

#include "GameObject.h"
#include "math/Vector3D.h"
#include "math/Matrix3D.h"


//Pointer to current state manager
Manager *pManager;
GameStateManager *gamestateMgr; //REPLACE IN FUTURE FOR DELEGATE

//Does the whole SDL, GLEW and GL context initialization
int InitAuxiliarLibraries(int width, int height, SDL_Window **pWindow, SDL_GLContext *context);


//Main execution code
int main(int argc, char** argv)
{
	std::cout << "Starting initialization of auxiliar libraries!" << std::endl;
	bool isAppRunning = true; 
	SDL_GLContext context;
	SDL_Window *pWindow;
	int width = 1280, height = 720;
	int result = InitAuxiliarLibraries(width, height, &pWindow, &context);
	


	////////////////////////////////////////////////////////////////////////////////
	//Init gameStateManager, then get the current manager and make
	ResourceManager *resMgr = new ResourceManager();
	GraphicsManager *grMgr = new GraphicsManager();
	InputManager *inputMgr = new InputManager();
	
	gamestateMgr = new GameStateManager();
	gamestateMgr->init(resMgr, grMgr, width, height);

	FrameRateController *frc = new FrameRateController(60);
	/////////////////////////////////////////////////////////////////////////////////



	//Main loop with event pooling loop
	while (isAppRunning)
	{
		///Call frameStart on framerate controller
		frc->FrameStart();
		unsigned int deltaTime = frc->getFrameTime();

		//Event loop
		SDL_Event e;
		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT)
			{
				isAppRunning = false;
			}
		}

		//Input Manager
		pManager->SetInputManager(inputMgr);
		pManager->GetInputManager()->update();

		//Update Event Manager
		pManager->GetEventManager()->Update(deltaTime / 1000.0f);

		//Update function
		pManager->GetGameObjMgr()->Update(deltaTime);

		//Physics Update - Collisions may also be here
		pManager->GetCollisionManager()->Update(deltaTime);
		pManager->GetPhysicsManager()->LateUpdate(deltaTime);

		//Draw function
		pManager->GetGraphicManager()->draw();

		//Swap buffers or something similar
		SDL_GL_SwapWindow(pWindow);

		///Call frameEnd on framerate controller
		frc->FrameEnd();
		//std::cout << 1000.0f/ frc->getFrameTime() << std::endl;

		///GameStateManager check current vs next
		///(THIS HAS TO HAPPEN OUTSIDE OF FRAMERATECONTROLLER frame calculation)
		gamestateMgr->checkStateChangeCondition();
	}



	//Free allocated resources
	delete gamestateMgr;	//CHANGE LATER
	delete inputMgr;		//CHANGE LATER
	delete frc;				//CHANGE LATER
	delete grMgr;			//CHANGE LATER
	delete resMgr;			//CHANGE LATER

	//SDL destroy functions
	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(pWindow);
	IMG_Quit();
	SDL_Quit();

	std::cout << "Finished freeing resources. Exiting application!" << std::endl;
	return 0;
}



//Does the whole SDL, GLEW and GL context initialization
int InitAuxiliarLibraries(int width, int height, SDL_Window **pWindow, SDL_GLContext *context)
{
	//Init SDL
	int status = 0;
	status = SDL_Init(SDL_INIT_VIDEO);
	if (status)
	{
		std::cout << "Error when initializing SDL. Exiting." << std::endl;
		return 1;
	}
	IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF);

	//Create window
	*pWindow = SDL_CreateWindow("FireHose Engine", SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL);
	if ( !(*pWindow) )
	{
		std::cout << "Error when creating the main window. Exiting." << std::endl;
		return 2;
	}

	//Create a GLContext using our SDL window
	*context = SDL_GL_CreateContext(*pWindow);
	if (*context == NULL)
	{
		printf("SDL context creation failed: %s\n", SDL_GetError());
		return 4;
	}

	//OpenGL code init 
	GLenum glewStatus = glewInit();
	if (glewStatus != GLEW_OK)
	{
		std::cout << "GLEW could not be initialized properly: " << glewGetErrorString(glewStatus) << std::endl;
		return 5;
	}
	if (!GLEW_VERSION_2_0)
	{
		std::cout << "Needs version gl 2.0 or newer." << std::endl;
		return 6;
	}
	glEnable(GL_DEPTH_TEST);

	std::cout << "<<---** ALL LIBRARIES LOADED CORRECTLY **--->>" << std::endl;
	return 0;
}