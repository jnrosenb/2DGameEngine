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
- Tab for debug mode (show both collision shapes (green) and trigger shapes (blue)
- The program is using openGL instead of software acceleration

EVENTS
- Are serializable
- can do direct events (which is doing right now to handle hit collisions)
- Broadcast
- Suscription (this is the one which is being serialized right now)

SERIALIZATION FILES ARE IN RESOURCES DIRECTORY
- level1.txt 		- (Called from engine.cpp, main). Calls all the archetypes that will be built.
			  After each archetype is created, the instances can be overriden in the following way:
				- position for transform component
				- Event suscription of the instance
				- Event keys for each event (used to see which event affects which object
				- Override event fire key (for trigger component objs only)
- MainPlayer.txt 	- Creates a main player and also the camera. It also has the controller component.
- Enemy.txt 		- Enemy that has the UpDown component.
- Background		- Just a big sprite for background
- Text			- Sprites with instructions
- etc

- This program was written and tested in an Alienware 15R3 with Windows 10 Operative System.