/* Start Header -------------------------------------------------------
   Copyright (C) 2018 DigiPen Institute of Technology.
   Reproduction or disclosure of this file or its contents without the prior
   written consent of DigiPen Institute of Technology is prohibited.

   File Name: README.txt
   Purpose: Containts instructions for building and running the program.
   Project: CS529_JRosenbluth_3
   Author: Jose Rosenbluth Chiu, J.Rosenbluth, 60003418
   Creation date: 10/14/2018
- End Header --------------------------------------------------------*/


INSTRUCTIONS:

- Move sprite using up, down, left, right
- The program is using openGL instead of software acceleration

SERIALIZATION FILES ARE IN RESOURCES DIRECTORY
- level1.txt 		- (Called from engine.cpp, main). Calls all the archetypes that will be built.
			  After each archetype, three numbers are given to override the transform position
- MainPlayer.txt 	- Creates a main player and also the camera. It also has the controller component.
- Enemy.txt 		- Enemy that has the UpDown component.
- Background		- Just a big sprite for background

- This program was written and tested in an Alienware 15R3 with Windows 10 Operative System.